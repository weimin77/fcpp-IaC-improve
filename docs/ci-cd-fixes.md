# CI/CD 修复说明

> 本文档记录本次会话中对 CI/CD 流水线的排查与修复，以及相关的触发机制速查。
> 对应提交：`d80a91b`、`63a7c96`、`cb80d82`。

## 概述

| # | 问题 | 根因 | 修复 | 提交 |
|---|------|------|------|------|
| 1 | `Install Conan` 失败：找不到 `conan==2.24.0` | 版本 pin 已从 PyPI 下架 | 升到 `conan==2.31.2` | `d80a91b` |
| 2 | `Build and test with conanfile` 失败：`lcov` 找不到 | Build workflow 没装覆盖率依赖 | 补装 `lcov` | `63a7c96` |
| 3 | `Documentation` job 被跳过 | `workflow_triggers.docs` 为 false | 开关改为 `true` | `cb80d82` |

---

## 问题 1：Conan 版本 pin 过期

**现象**

`Tests / Auto Testing` 与 `Build` 的 `Install Conan` 步骤失败：

```
ERROR: Could not find a version that satisfies the requirement conan==2.24.0
       (from versions: 2.28.0, 2.28.1, 2.29.0, 2.29.1, 2.30.0, 2.31.0, 2.31.1, 2.31.2)
ERROR: No matching distribution found for conan==2.24.0
```

**根因**

`conan==2.24.0` 已不再可用，pip 当前只能安装 `2.28.0` ~ `2.31.2`。

**修复**

两个 workflow 的安装步骤从 `conan==2.24.0` 改为 `conan==2.31.2`：

- `.github/workflows/ci-build-test.yml`（Build）
- `.github/workflows/full-test-automation.yml`（Tests）

```bash
pip install --only-binary :all: conan==2.31.2
```

---

## 问题 2：Build workflow 缺少 lcov

**现象**

`Build and test with conanfile` 步骤在测试通过后、覆盖率阶段失败：

```
FileNotFoundError: [Errno 2] No such file or directory: 'lcov'
```

**根因**

`metadata.json` 里 `activate_code_coverage=true`，`test_package/conanfile.py` 的 `test()` 会在测试后无条件执行覆盖率采集（`_code_coverage_auto()`），需要 `lcov`/`genhtml`。

Tests workflow 的 `System dependencies` 装了 `libnsl-dev lcov`，但 **Build workflow 只装了 `libnsl-dev`**，缺 `lcov`。

**修复**

`.github/workflows/ci-build-test.yml` 的 Ubuntu 依赖步骤补上 `lcov`：

```bash
sudo apt install -y libnsl-dev lcov
```

---

## 问题 3：Documentation 流程被跳过

**现象**

PR 中 `Documentation` job 显示 "This job was skipped"（其余 job 均通过）。

**根因**

`docs` job 的触发条件是三个条件**同时满足**（`metadata-controller.yml`）：

```bash
if [[ "$EVENT_NAME" == "push" && "$has_docs_emoji" == "true" && "$WF_DOCS" == "true" ]]; then
    trigger_docs="true"
fi
```

即：push 事件 + commit message 含 `:book:` + `workflow_triggers.docs=true`。

当时 `metadata.json` 里 `workflow_triggers.docs = false`，所以即便 push 且带 `:book:` 也不会触发。

**修复**

`metadata.json` 的 `workflow_triggers.docs` 从 `false` 改为 `true`。

---

## 附录 A：CI 触发机制速查

总入口 `ci-orchestrator.yml` 在 push 到 `main` 时**总是**运行，并固定执行 Commit Lint 与 Metadata Schema 校验；下游流水线是否运行由 **commit message 中的 emoji + `metadata.json` 开关** 共同决定：

| 流水线 | 触发 emoji | 额外条件 |
|--------|-----------|---------|
| Build | `:building_construction:` | `workflow_triggers.build=true` |
| Tests | `:beer:` | `build_type=Debug` + `trigger_tests=true` + `workflow_triggers.tests=true` |
| Docs | `:book:` | `workflow_triggers.docs=true` |
| Security | `:shield:` | `workflow_triggers.security_scan=true`（PR 事件必跑） |
| Release | 无 emoji | `workflow_triggers.release=true` + 存在 `feat/fix/perf` 提交（semantic-release 独立 workflow） |
| Board/cross-compile | 无 emoji | push 且改动命中特定路径，或手动 `workflow_dispatch` |

要点：

- 控制器对**整次 push 的所有 commit message** 做 grep（`join(github.event.commits.*.message)`），emoji 出现在 message 任意位置即可触发，可拆多个 commit、一次 push。
- 版本号由 conventional 前缀（`feat/fix/perf`）驱动，与 emoji 正交。

---

## 附录 B：已知未决 —— include/ 覆盖率 0%

**现象**

LCOV 报告中 `include/` 目录函数覆盖率为 0%（4 个函数 0 命中），`src/` 为 100%。

**根因**

覆盖率采集函数 `test_package/conanfile.py` 的 `_code_coverage_gcc()` 只从**主包 fcpp 的 build 目录**收集 `.gcda`：

```python
_gcda = [str(_) for _ in Path(_main_pkg_build_fd).rglob('*.gcda')]
```

`include/cpptest.hpp` 里的头文件模板函数（`test_sum`、`Color::set/print/components`）只在 **test_package 的可执行程序**里被实例化执行，其 `.gcda` 落在 test_package 的 build 目录，未被采集；报告里那条 0% 记录来自主包 `src/cpptest.cpp`（包含该头文件但从不执行这些模板）。

**结论**

仅靠增加测试用例无法改变该数字——必须修改 `_code_coverage_gcc()`，将 test_package 的 build 目录也纳入 `.gcda` 收集，并处理随之而来的 GCC 15 下 lcov 的 `inconsistent` 与 `source` 两类报错。当前该修复尚未落地。
