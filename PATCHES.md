# P4.51 public patch inventory

All upstream revisions below are exact local TWRP 12.1 checkout HEADs captured
by Phase 1A on 2026-09-19. The earlier P4.x changes remained uncommitted there;
these files were preserved during final P4.51 normalization. Apply the patches
only to the listed project directories. `apply-p451-public.sh` performs the
checks and application in order.

| Phase | Upstream project | Base HEAD | Affected path | Purpose and root cause |
| --- | --- | --- | --- | --- |
| P4.46 | `system/vold` | `a164ba05c5fef288059774a776b2e6e1119957cf` | `Keymaster.cpp` | Bound keystore2 service wait; an absent/crashing keystore2 service made `waitForService` block recovery startup indefinitely. |
| P4.47 | SO-01K device tree / recovery ramdisk framework VINTF | `118efe51246177d2b287680bd499747a35541668` before normalization | `recovery/root/system/etc/vintf/manifest.xml` copied to `/system/etc/vintf/manifest.xml` | Add the main framework manifest; without it libvintf failed to load fragments and keystore2 registration was rejected. |
| P4.49 | `system/vold` | `a164ba05c5fef288059774a776b2e6e1119957cf` | `FsCrypt.cpp` | Prepare `/data/vendor_de`; recovery init lacks the framework init step that creates this parent, so user storage preparation failed with ENOENT. |
| PREPDECRYPT | `device/qcom/twrp-common` | `98506f7919102378c8d52ee7d6a94a867f1b4c55` | `crypto/system/bin/prepdecrypt.sh` | Treat only literal `true` as A/B; the old nonempty check misread `ro.build.ab_update=false` as A/B on SO-01K. |

## Exact manual commands

Run from this repository root, with `TWRP` set to a compatible checkout path.
Check the bases and clean state before applying; the script does this and is
the recommended entry point.

| Phase | Apply/install command | Verification command | Validation evidence |
| --- | --- | --- | --- |
| P4.46 | `git -C "$TWRP/system/vold" apply "$PWD/patches/p446-vold-keymaster.patch"` | `git -C "$TWRP/system/vold" diff --check; git -C "$TWRP/system/vold" diff -- Keymaster.cpp` | P4.46 bounded-wait investigation and P4.51 physical recovery boot; this does not prove credential decrypt. |
| P4.47 | `install -m 0644 "$PWD/vintf/p447-framework-manifest.xml" "$TWRP/device/sony/poplar_docomo/recovery/root/system/etc/vintf/manifest.xml"` | `cmp "$PWD/vintf/p447-framework-manifest.xml" "$TWRP/device/sony/poplar_docomo/recovery/root/system/etc/vintf/manifest.xml"` | `SO01K_V5_PHASE4_47_KEYMASTER_DONOR_DIFFERENTIAL_REPORT.md`; P4.51 ramdisk reference SHA-256 `ea52d71429d1cfd744a604b0498eabb2561cb1ceb9182f1d2f18a9f00de3ec0a`. |
| P4.49 | `git -C "$TWRP/system/vold" apply "$PWD/patches/p449-vold-fscrypt.patch"` | `git -C "$TWRP/system/vold" diff --check; git -C "$TWRP/system/vold" diff -- FsCrypt.cpp` | `SO01K_V5_PHASE4_49_VENDOR_DE_PARENT_FIX_REPORT.md`; P4.51 `/data` mount and plaintext read. |
| PREPDECRYPT | `git -C "$TWRP/device/qcom/twrp-common" apply "$PWD/patches/prepdecrypt-nonab.patch"` | `git -C "$TWRP/device/qcom/twrp-common" diff --check; git -C "$TWRP/device/qcom/twrp-common" diff -- crypto/system/bin/prepdecrypt.sh` | Phase 1A captured diff and SO-01K non-A/B configuration; P4.51 recovery operation. |

P4.51 itself is a dedicated device-tree commit: `/system` to `/system_root`
in `twrp.flags`. Frozen reference SHA-256:
`c579edbb131dd7e7e2f2a3b8abfafe30b3172883b2af34fedf986e4c317f2c39`.
P4.51 changed only this System backup key in the frozen image. System was
physically visible in Backup and backed up successfully; System and Vendor
restore were not tested.

These are source-state preservation records. No build was performed in Phase
1B and bit-for-bit image reproduction is not claimed. Private crypto binaries
and the frozen image are not included.
