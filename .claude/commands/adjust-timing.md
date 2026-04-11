Adjust hold-tap timing parameters based on the user's description of the problem.

## Steps

1. Ask the user to describe the symptom if not already stated (false triggers, mods not firing, key repeat broken, etc.).
2. If we're on `main`, create a feature branch first — never commit timing changes directly to `main`.
3. Read `config/eyelash_sofle.keymap` — find the `behaviors {}` block with `hml`, `hmr`, and `ralt_mt`.
4. Apply the appropriate change (see table below).
5. Run `mise run draw` to regenerate the SVG.
6. Commit all three files (`keymap`, `yaml`, `svg`) and push.
7. Create or update a PR with `gh pr create --assignee rdlu --label "keymap"` — never leave the PR unassigned or unlabeled.
8. After merge, wait for the `[Draw Keymap]` workflow to finish on main, then `git checkout main && git pull --ff-only` before starting more work.

## Parameters and what they control

| Parameter | Location in file | Effect of increasing | Effect of decreasing |
|-----------|-----------------|----------------------|----------------------|
| `tapping-term-ms` | all three behaviors | harder to trigger hold accidentally | hold fires sooner / easier to trigger |
| `require-prior-idle-ms` | all three behaviors | fewer false mods during fast typing | mods respond faster after adjacent keys |
| `quick-tap-ms` | all three behaviors | longer window for key repeat | shorter repeat window |
| `flavor` | all three behaviors | see flavors below | — |

## Flavors

- `balanced` *(current)*: hold fires if another key is **pressed and released** inside tapping-term. Good balance for typing.
- `tap-unless-interrupted`: hold fires only if no other key is pressed during tapping-term. More permissive for mods.
- `hold-preferred`: hold fires if any key is pressed during tapping-term. Most aggressive — not recommended for home row.

## Symptom → fix

| Symptom | Fix |
|---------|-----|
| Mod fires while typing fast (false trigger) | Increase `require-prior-idle-ms` (+25 ms steps) |
| Mod doesn't fire when I want it | Decrease `tapping-term-ms` (-20 ms steps) or switch to `tap-unless-interrupted` |
| Key repeat doesn't work | Increase `quick-tap-ms` to ≥ your fastest double-tap speed |
| Mod fires too late / laggy | Decrease `tapping-term-ms` |
| Only same-hand combos fail | Check `hold-trigger-key-positions` includes all intended keys |
| AltGr accent not working | Verify `ralt_mt` has **no** `hold-trigger-key-positions` |

## Behaviors and their keys

```
hml  → D(LCTRL)  F(LSHFT)  G(LALT)     [left home row]
hmr  → H(LALT)   J(RSHFT)  K(RCTRL)    [right home row]
ralt_mt → T(RALT)  Y(RALT)              [AltGr, no positional restriction]
```

All three share the same timing block — change all three consistently unless intentionally differentiating.
Current values: `tapping-term-ms=280`, `require-prior-idle-ms=150`, `quick-tap-ms=175`.

## Important

Always update KEYMAP.md "Hold-tap timing reference" table if you change the current values, so the docs stay accurate.

## Local iteration (no GH Actions)

Timing tuning is almost always faster to test locally than through the PR/CI cycle — the feel of a mod or tap can only be judged by using the keyboard, and waiting for a full GH Actions build between each attempt is painful. Use this loop:

```bash
# Edit config/eyelash_sofle.keymap — change tapping-term-ms, require-prior-idle-ms, etc.
mise run draw           # regenerate SVG
mise run build-left     # build left half (only the left needs new timing)
mise run flash-left     # prompts for double-tap, waits for NICENANO mount, flashes
# Test on hardware — type, check mod triggers, check repeat behavior
# Repeat
```

Only once the values feel right, commit + push + open a PR (with `--assignee rdlu --label "keymap"`). No need to open a PR for every tweak attempt.

**Studio overlay warning:** if the user has ever used ZMK Studio and the new timing doesn't seem to take effect, the stored overlay is overriding the compiled firmware. Run `mise run flash-reset` on the left half first, then `mise run flash-left`.
