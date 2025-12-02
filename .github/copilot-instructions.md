# copilot-instructions.md — Caesar

## 1. Mindset

- Caesar is a **real language + toolchain**, not a toy project.
- Priorities: **correctness → clarity → performance → convenience** (in that order).
- When something fails, **stop and reason**. Do not “hack until green”.

---

## 2. Testing & Debugging

- When creating tests:
  - Test **every feature** as fully as reasonably possible.
  - Cover **edge cases, invalid inputs, and weird combinations**, not only happy paths.
- On any failing test:
  1. Think: _Is the bug in the test or in Caesar (compiler/interpreter/runtime)?_
  2. If the test is wrong → **fix the test**.
  3. If Caesar is wrong → **fix Caesar** (compiler, interpreter, runtime, stdlib, etc.).
- **NEVER “fix” a test just to hide a real bug in Caesar.**
  - **DON’T JUST MODIFY THE TESTCASE SO WE DON’T ENCOUNTER THAT BUG.**

---

## 3. Data & Placeholders

- **Never create real mock data.**
- Only use **placeholders clearly marked with `TODO`**, for example:
  - `// TODO: inject real input source`
  - `// TODO: replace with real project path`
- Placeholders must be:
  - Easy to search for (`TODO`, `FIXME`).
  - Documented enough that a human knows how to replace them later.

---

## 4. Code Quality & Structure

- Follow **established best practices** for the language being written (C/C++, Rust, Python, etc.).
- Keep the codebase:
  - **Well-organized** (clear modules, layers, and responsibilities).
  - **Sorted and consistent** (naming, file layout, imports, formatting).
  - **Documented where needed** (short, focused comments; clear function/module docs).
- Prefer:
  - Small, composable functions over “god functions”.
  - Clear error handling over silent failures.
  - Refactoring duplicated logic instead of copy–paste.

---

## 5. Changes & New Files

- Only create new files / directories when they **clearly improve structure**.
- Don’t introduce extra “meta-docs” (e.g. explaining what Copilot did) unless explicitly asked.
- Keep diffs as **minimal and focused** as possible: one concern per change.

--

> Ensure everything you do is production-ready. No quick hacks or shortcuts that would only work temporarily/locally. Aim for high quality, maintainable, and robust code that fits seamlessly into the Caesar project.

