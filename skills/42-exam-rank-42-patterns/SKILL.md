---
name: 42-exam-rank-42-patterns
description: Coding and workflow patterns extracted from 42-exam-rank-42 (personal 42 School exam-practice repo)
version: 1.0.0
source: local-git-analysis
analyzed_commits: 200
---

# 42-exam-rank-42 Patterns

## What This Repo Is

A personal practice environment for 42 School's C/C++ oral exams (Rank 02–05, plus
`new-exams/exam-rank-03` and `legacy/`). Each exam rank has a menu-driven `exam.sh`
(and a top-level `exam_master.sh`) that picks a random exercise, stages it into a
shared `rendu/` workspace, and grades the submission with a `grademe/test.sh`
script. There is no build system beyond per-exercise `gcc`/`clang` invocations —
the repo *is* the curriculum.

## Commit Conventions

Two eras, both still valid — check recent `git log` before assuming which applies:

1. **Free-form (bulk of history)**: short imperative Spanish/English sentences,
   no prefix — `"corregido picoshell y argo"`, `"añadida variable gloal para el
   control de señales"`, `"official map for bsq exam rank 05 level 2"`.
2. **Conventional commits (recent, last ~15 commits)**: `type(scope): description`,
   description in Spanish, scope is usually the exam number and/or exercise path —
   `test(05): grademe/test.sh real para bsq`, `fix(rendu5/bsq): validar caracteres
   imprimibles y salto de linea final`, `docs(rendu5/vect2): actualizar guia con
   operator*= y operator- unario`, `feat(05): trasladar polyset/bigint/vect2/bsq
   /gameoflife desde 5exam`.

Types observed: `feat`, `fix`, `test`, `docs`, `chore`, `refactor`. Prefer the
conventional form for new commits in `05/` and `rendu5/` — it's what the last
several sessions settled into.

## Directory Architecture

```
02/, 03/, 04/, 05/            # exam ranks, each with exam.sh + exam_progress/
  Level1|level-1/<exercise>/  # "given" material for that exercise
    grademe/test.sh           # the grader (see Testing Patterns below)
new-exams/exam-rank-03/       # newer/alternate exam-03 content, same shape
legacy/03-old/, legacy/04-old/  # retired exercise versions, kept for reference
rendu/                        # SHARED scratch dir all exam.sh scripts write/read
                               # from during a live practice run — always empty
                               # between sessions, never pre-filled
rendu2/, rendu4/, rendu5/     # per-exam-number ARCHIVES of your own polished,
                               # working solutions — reference only, never read
                               # by exam.sh. Each exercise subfolder there tends
                               # to carry a pedagogical GUIA.md (see below).
skills/estudio42/             # unrelated Node.js tool (exam study-mode skill)
```

Vocabulary is deliberately controlled — see `05/CONTEXT.md` for the canonical
definitions of **Given**, **Expected Files**, **rendu** vs **renduN**. When
writing docs or commit messages for this repo, reuse those exact terms instead
of synonyms like "skeleton", "template", "solution", or "backup" — a prior
session flagged those as ambiguous and standardized on the CONTEXT.md wording.

## Workflow: Adding/Fixing a `grademe/test.sh` Grader

This is the most repeated workflow in the recent history (bsq, bigint, vect2,
polyset all got "real" test.sh graders added in sequence). The established shape:

1. **Contract**: no arguments; `exit 0` = pass, `exit != 0` = fail. Called by the
   parent `exam.sh` as `cd .../<exercise>/grademe && ./test.sh`.
2. **Path resolution**: always resolve from `$(cd "$(dirname "${BASH_SOURCE[0]}")"
   && pwd)`, never from `$PWD` — the script is invoked from varying cwd.
3. **Scratch dir**: `mktemp -d`, `trap cleanup EXIT` to remove it — never write
   into the given/student dirs directly.
4. **Compile strictness**: `-std=c98` (C exercises) or the subject-mandated C++
   standard, always `-Wall -Wextra -Werror`, 0 warnings tolerated.
5. **Golden-output diffs**: when the subject's tie-break/spec is fully
   deterministic (e.g. bsq's top-left rule), assert exact `diff` against a
   golden string embedded in the test script rather than a fuzzy check.
6. **Sanitizers**: an optional AddressSanitizer+UBSan pass at the end, portable
   across Linux/macOS — detect `uname`, skip (not fail) on known ASan/macOS
   startup flakiness (`rc 137`/`124`), never block the grade on sanitizer
   infra issues.
7. **Output style**: colored `[PASS]`/`[FAIL]`/`[SKIP]` helpers with running
   counters, a `summary_and_exit` that exits non-zero iff any FAIL. Comment
   header at the top of the file cross-references what part of the subject
   each check section covers (see `05/level-2/bsq/grademe/test.sh:20-42` for
   the pattern to copy).

When porting this pattern to a new exercise, grep an existing grader in the same
exam rank first (`05/level-1/bigint/grademe/test.sh`, `.../vect2/...`,
`.../polyset/...`) — they intentionally share structure ("Layout dividido (igual
que polyset/vect2/bigint)").

## Testing Patterns

- 77 `grademe/test.sh` files exist, one per exercise, across `02/`, `04/`, `05/`,
  `new-exams/exam-rank-03/`, and `legacy/`. `03/` (the non-"new-exams" copy) is
  the one rank currently missing full graders — worth checking before assuming
  coverage.
- No unit test framework — tests are bash scripts driving the compiled binary
  and diffing stdout/stderr against expected strings.

## Documentation Patterns

- **`GUIA.md`** (pedagogical, per exercise, lives in `renduN/<exercise>/`):
  written for "alguien que sabe C pero no conoce el ejercicio" — starts with
  "QUÉ ES EL EJERCICIO", walks through the required struct/files, then the
  core algorithm. Written entirely in Spanish. Add one whenever a `renduN`
  reference solution is added without one (a prior commit,
  `docs(rendu5/bigint): añadir GUIA.md pedagogica (faltaba)`, exists
  specifically to backfill a missed one).
- **`CONTEXT.md`** (one per exam rank that needs it, e.g. `05/CONTEXT.md`):
  domain-glossary style — Language section defining ambiguous terms, a
  Relationships section, and a "Flagged ambiguities / decisions" section that
  logs judgment calls made during reorganization. Update this file, don't
  create a parallel doc, when a new naming ambiguity gets resolved in that rank.

## Language Note

Code comments, commit messages, and docs are predominantly Spanish; a few
older READMEs and one PR added English variants (`README.en.md`). Match
whatever language the file/directory you're editing already uses rather than
defaulting to English.
