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

## Known limitations

None currently tracked. `subject.en.txt` lookup and the `es`/`en` translation
gaps (in both `index.js`'s prompt assembly and `generator.js`'s auto-generated
concepts/complexity/resources) were fixed and verified end-to-end in both
languages — if you spot Spanish leaking through while `config.json`'s
`language` is `"en"` (or vice versa), that's a regression, not a known gap;
flag it.
