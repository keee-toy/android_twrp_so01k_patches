# Optional bootable/recovery development history

Phase 1A captured `bootable_recovery-unstaged.diff` at upstream HEAD
`5c3d206a5eeb3d446bcda8248a405a4b278bab5c`. The exact tracked diff is
`bootable-recovery-tracked.diff`. Six untracked source/config snapshots are in
`untracked/` under their original relative paths. This material is public-safe
diagnostic history and is **not applied** by `apply-p451-public.sh`.

| Path | Classification | Reason |
| --- | --- | --- |
| `gui/gui.cpp` | DIAGNOSTIC_ONLY | P4.31 splash flip observer and P4.35 stage marker. |
| `minuitwrp/graphics_fbdev.cpp` | DIAGNOSTIC_ONLY | Framebuffer ioctl/single-copy classifier and property/file result publication. |
| `partitionmanager.cpp` | DIAGNOSTIC_ONLY | P4.35 pre-fstab stage record publisher. |
| `twrp.cpp` | DIAGNOSTIC_ONLY | P4.35/P4.36 startup and pre-main markers. |
| `gui/p434_stage.h`, `gui/p435_stage.h`, `p434_stage.h`, `p435_stage.h`, `p436_stage_marker.h` | DIAGNOSTIC_ONLY | Support headers for the above markers. |
| `etc/init.recovery.service22.rc.before-sh-linker-diag` | DIAGNOSTIC_ONLY | Historical pre-diagnostic init snapshot; not an active P4.51 configuration. |

`REQUIRED_FOR_P451`: none of these bootable/recovery changes. `UNCERTAIN`: none
after comparison with the P4.44 clean recovery baseline and P4.49/P4.51 report
that the recovery binary was retained byte-identically. This historical set
is incomplete as an automatically applicable patch because the six untracked
files are snapshots, so a future experiment must review and integrate it
explicitly. It is not evidence of the frozen P4.51 binary's source contents.
