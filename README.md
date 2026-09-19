# SO-01K TWRP 12.1 public source patches

Target: Sony Xperia XZ1 SO-01K (`poplar_docomo`). Recovery uses the custom
kernel and the logical recovery target backed by FOTAKernel. P4.51 is the
**current frozen SO-01K recovery baseline**, functionally ready for ROM handoff.

This repository preserves three minimal changes to upstream TWRP 12.1 source
and the P4.47 framework VINTF manifest. The companion public device tree has
the P4.51 `/system_root` backup mapping and the VINTF copy rule. See
`PATCHES.md` for exact bases, causes, commands, and evidence.

From a compatible fresh checkout with the pinned device tree, run:

```sh
./apply-p451-public.sh /path/to/twrp-12.1
```

The script checks all base commits, verifies the device source and manifest,
then applies the three small upstream patches. It does not obtain stock crypto
components, build an image, or reproduce the frozen image byte for byte.
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
