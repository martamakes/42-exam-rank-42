# Exam 05 Practice Environment

Holds the C/C++ exercises for 42's Exam Rank 05 (bigint, vect2, polyset, bsq, game_of_life), organized so `make` (via `05/exam.sh`) can quiz on a random exercise and grade the submission.

## Language

**Given**:
The subject statement plus any code 42 hands you at exam start — interfaces you may not modify, and the fixed `main.cpp`/driver. Lives in `05/level-{1,2}/<exercise>/`.
_Avoid_: skeleton, template

**Expected Files**:
The exact file list the subject asks you to write yourself (e.g. for polyset: `searchable_array_bag.*`, `searchable_tree_bag.*`, `set.*`; for bigint: `bigint.cpp`, `bigint.hpp`). Never overlaps with **Given**.
_Avoid_: solution files — ambiguous, since some exercises (bigint, vect2) are 100% student-written and have no other "given" implementation to contrast with.

**rendu**:
The single, shared, root-level (`/Volumes/BIWIN/42-exam-rank-42/rendu/`) workspace `exam.sh` reads from during a live practice run. You write your **Expected Files** here from a blank state each time `exam.sh` picks an exercise; it stays empty between sessions and is never pre-filled.
_Avoid_: submission — accurate, but "rendu" is the term already used consistently by `exam.sh` and by the root `rendu2`/`rendu4` folders.

**rendu5**:
A root-level (`/Volumes/BIWIN/42-exam-rank-42/rendu5/`) archive of your own polished solutions for exam 05, kept for study/reference only — never read by `exam.sh`. Mirrors the naming convention already used for exam 02 (`rendu2`) and exam 04 (`rendu4`).
_Avoid_: backup — it isn't a backup of `rendu`, it's separate curated reference material.

## Relationships

- Each exercise has exactly one **Given** folder, under `05/level-1/<exercise>/` or `05/level-2/<exercise>/`.
- Each exercise's **Expected Files** get written from scratch into **rendu** during practice, and are kept separately, already-working, in **rendu5**.
- **rendu** is shared across exam numbers — `02/exam.sh`, `03/exam.sh`, `04/exam.sh`, `05/exam.sh` all resolve to the same root `rendu/`. **rendu5** is exam-05-specific, one of a per-exam-number family (`rendu2`, `rendu4`, ...).

## Example dialogue

> **Dev:** "Why did we delete the `.cpp`/`.hpp` files that were inside `05/level-1/polyset/`?"
> **Domain expert:** "Those weren't **Given** material — they were another student's (fatkeski's) complete solution, sitting exactly where the **Expected Files** should be written fresh each practice run. Removing them restores `polyset/` to a clean **Given** state."

> **Dev:** "So where did our own working solution go?"
> **Domain expert:** "Into `rendu5/polyset/` — reference material, not what `make` grades. When you run `make`, choose exam 05, and it picks polyset, you write into `rendu/polyset/` from scratch; `rendu5` is only there if you get stuck and want to check."

## Flagged ambiguities / decisions

- `05/level-1/polyset/subject/` turned out to be a full external reference solution (compared against ours in a prior session), not official 42-provided material — renamed to `reference/` so it's never mistaken for **Given**.
- `bigint`, `vect2`, `polyset` under `05/level-1/` originally held another student's (fatkeski) complete solutions in place of the **Expected Files** — removed. Only genuine **Given** material remains in each folder.
- `bsq` and `game_of_life` under `05/level-2/` never had this problem — only **Given** material (`bsq.h` + `subject.en.txt`; `life.h` + `subject.txt`) was ever present there.
- Study documents from the original `5exam` project (`STUDY_EXAM05.md`, `POLYSET_COMPLIANCE.md`, `NOTES.md`, `STRUCT.md`, `test_*.sh`) were intentionally **not** migrated — they stay in the original project, kept as a backup at `/Volumes/BIWIN/42 - subjects/5exam`. Only the per-exercise `GUIA.md`/`README.md` pattern already established inside `rendu5` was carried over.
- `X5-imp1`/`X5-imp2` (older alternative attempts, not the Level_01/Level_02 work) were left behind, not migrated.
- No `grademe/test.sh` exists yet for any of the 5 exercises — `exam.sh` currently falls back to its own auto-generated placeholder ("tests not created") when validating. Building real graders from `test_polyset.sh`/`test_bsq.sh`/`test_life.sh` is deferred to a future session.
