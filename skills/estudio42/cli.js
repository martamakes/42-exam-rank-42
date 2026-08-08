#!/usr/bin/env node
// Entry point for the estudio42 SKILL.md. Runs execute() against the
// caller's cwd (process.cwd(), NOT this file's directory — do not `cd`
// before running this) and prints the single resulting instructional
// block to stdout for Claude to read and act on.

const { execute } = require('./index');

execute()
  .then((output) => {
    console.log(output);
  })
  .catch((err) => {
    console.error('estudio42: unexpected error —', err.message);
    process.exit(1);
  });
