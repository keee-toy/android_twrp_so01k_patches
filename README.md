# SO-01K TWRP 12.1 public source patches

Target: Sony Xperia XZ1 SO-01K (`poplar_docomo`). Recovery uses the custom
kernel and the logical recovery target backed by FOTAKernel. P4.51 is the
**current frozen SO-01K recovery baseline**, functionally ready for ROM handoff.

This repository preserves three minimal changes to upstream TWRP 12.1 source
and the P4.47 framework VINTF manifest. The companion public device tree has
the P4.51 `/system_root` backup mapping and the VINTF copy rule. See
`PATCHES.md` for exact bases, causes, commands, and evidence.

The exact custom kernel binary is in the
[`keee-toy/android_device_sony_poplar_docomo-twrp`](https://github.com/keee-toy/android_device_sony_poplar_docomo-twrp)
device repository. `kernel-source/` records the pinned public upstream kernel
base, the two SO-01K source deltas, and the frozen kernel configuration.
The checkout inputs are in
[`keee-toy/android_manifest_so01k_twrp`](https://github.com/keee-toy/android_manifest_so01k_twrp).

From a compatible fresh checkout with the pinned device tree, run:

```sh
./apply-p451-public.sh /path/to/twrp-12.1
```

The public manifest pins the final normalized device commit
`5829c6eacb6bbe96df7017f9ac1c18e735fde2b2`. Its P4.47 framework VINTF
manifest and P4.51 `/system_root` mapping are already committed. The script
checks those files in place, then applies only the three small upstream patches.
It checks exact project HEADs, hashes, and clean states before changing files.

Phase 2's first independent GitHub clone found that the original setup script
expected earlier device commit `ee171c098acbc770127778f20d3e210f5494ac15`
while the public manifest pinned `5829c6e`. This forward fix accepts the final
device commit and avoids reinstalling the already committed P4.47 source.

The script does not obtain stock crypto components, build an image, or reproduce
the frozen image byte for byte.
`device.mk` records the private stock component paths used in the physical
configuration; those binaries are absent from all public repositories.

P4.51 physical testing proved `/data` mount, plaintext internal storage, and
a real decrypted-file read. System appeared in Backup; Boot, System, and Vendor
backups and digests succeeded. A Boot-only restore succeeded and Android booted
afterward. TWRP still reports a formal FBE user-0 decrypt failure and a
default-password warning. Credential/PIN decrypt, System restore, Vendor
restore, and Format Data were not tested.

`diagnostics/` preserves earlier bootable/recovery instrumentation as history.
The default setup does not apply it. This repository contains no recovery
image, Sony/QTI blob, FTF, device dump, firmware, or private credential.
