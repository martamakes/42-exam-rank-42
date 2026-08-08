---
name: estudio42
description: Interactive Socratic tutor mode for 42 School (École 42) exam-practice repos. Detects which exercise the user is currently working on, loads or generates progressive hints for it, and guides them with questions instead of writing code for them. Use when the user invokes /estudio42 or /study42, says "modo estudio 42" / "activa estudio42" / "study mode", or asks to be quizzed or tutored Socratically on a 42 exercise in a repo that has 42 exam-rank folders (02/, 03/, 04/, 05/, ...).
---

# Estudio42 — 42 School Socratic Tutor

## How to run it

From the project root, without changing directory first:

    node skills/estudio42/cli.js

This is a thin CLI wrapper (`cli.js`) around the existing `index.js` module,
which in turn uses `detect.js` (exercise detection), `generator.js` (tip
generation from the subject file), and `prompts.js` (es/en strings). Running
it:

- Reads or creates `.estudio42/config.json` (language, username) at the
  project root — the nearest ancestor of the cwd that contains `.git`.
- Detects the exercise the user is currently working on from the cwd.
- Loads `.estudio42/tips/rank-<NN>/level-<L>/<exercise>.md` if it already
  exists, or generates it from the exercise's subject file and saves it there.

It prints **one self-contained block of instructions** to stdout. Read it and
follow it for the rest of the session — it already states whether an exercise
was detected, the tutoring rules, the available session commands, and the
tips/guide content for that exercise. Don't re-derive or paraphrase this
behavior yourself; treat the printed block as authoritative for how to act,
in whichever language (`es`/`en`) it comes back in.

Re-run it whenever the user says they've switched exercises ("siguiente
ejercicio" / "next exercise"), since detection is cwd-based.

## Adding a user tip

The script does not perform "add tip" edits itself — `generator.js` exports
an `addUserTip` helper, but nothing currently calls it; treat it as unused.
When the user says "añade mi tip: ..." / "add my tip: ...", make the edit
yourself with your file-editing tools: open the tips file the script
reported, find (or create) a `### @<username> (<YYYY-MM-DD>)` heading under
the `## 👤 Tips de Usuarios` / `## 👤 User Tips` section — username comes
from `.estudio42/config.json` — and append the tip as a bullet under it.
Never edit another user's subsection.

## Known limitations (not yet fixed — work around, don't silently trust)

- **Detection is unreliable inside `rendu/<exercise>/`** — the shared,
  root-level live-practice workspace where the student actually writes code
  (see `05/CONTEXT.md` for the `rendu` vs `rank/level` layout). `detect.js`'s
  heuristics assume `rendu` is nested under a rank folder, which it isn't in
  this repo. If the script reports the wrong exercise, or falls back to
  asking for manual input while the user is clearly mid-exercise in
  `rendu/`, ask them to confirm or correct it — don't trust the detection
  blindly.
- **Subject lookup misses `subject.en.txt`**, used by several Rank 05
  exercises — `generator.js::findSubjectFile` only checks `README.md` and
  `subject.txt`. If tip generation reports it can't find the subject but one
  exists under a different filename in the exercise's given directory, read
  it yourself and use it, or ask the user to point you at it.
- **English mode is partially untranslated** — some of the final prompt
  assembly in `index.js` hardcodes Spanish labels regardless of
  `config.json`'s `language`. If `language` is `"en"`, respond in English
  yourself even where the printed block slips into Spanish.
