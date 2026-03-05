# Contributing to LibraTrack

Thank you for participating! This document explains how to fix an issue and submit a pull request.

---

## Workflow

### 1. Set up the repository

```bash
git clone <repo-url>
cd libratrack
cmake -B build
cmake --build build
```

### 2. Pick an issue

Browse the open [Issues](../../issues). Each issue describes:
- Which file and function contains the bug
- What the current (incorrect) behaviour is
- What the expected behaviour should be
- The public test you can run to verify your fix

### 3. Create a branch

```bash
git checkout -b fix/issue-NN-short-description
# example: fix/issue-05-book-full-title
```

### 4. Fix the bug

Open the relevant file, find the bug, and fix it. The issue description tells you exactly where to look.

### 5. Verify with the public test

```bash
cmake --build build
ctest --test-dir build -R IssueNN_Public --output-on-failure
# example: ctest --test-dir build -R Issue05_Public --output-on-failure
```

The test must **pass** before you submit.

### 6. Commit your change

```bash
git add src/SomeFile.cpp          # only the file(s) you changed
git commit -m "Fix #NN: brief description of the fix"
```

### 7. Open a pull request

Push your branch and open a PR against `main`:

```bash
git push origin fix/issue-NN-short-description
```

PR title format: `Fix #NN: short description`  
Example: `Fix #05: Book::getFullTitle() returns author and title in wrong order`

---

## Rules

- Fix **one issue per pull request**. Do not bundle multiple fixes.
- Do not modify test files or CMakeLists.txt.
- Do not hard-code return values to match the test — the submission will also be checked against a private test suite with different inputs.
- Keep your diff minimal. Only change what is necessary to fix the described bug.

---

## Building just the tests

```bash
cmake --build build --target libratrack_tests
ctest --test-dir build --output-on-failure
```

---

## Questions?

Open a discussion thread or mention it in the issue comments.
