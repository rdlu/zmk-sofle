Adjust hold-tap timing parameters based on the user's description of the problem.

## Steps

1. Ask the user to describe the symptom if not already stated (false triggers, mods not firing, key repeat broken, etc.).
2. Read `config/eyelash_sofle.keymap` — find the `behaviors {}` block with `hml`, `hmr`, and `ralt_mt`.
3. Apply the appropriate change (see table below).
4. Run `mise run draw` to regenerate the SVG.
5. Commit all three files (`keymap`, `yaml`, `svg`) and push.

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
