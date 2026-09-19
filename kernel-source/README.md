# Frozen P4.51 custom kernel source

The SO-01K device repository tracks the exact P4.51 kernel payload at
`prebuilt/Image.gz-dtb`, SHA-256
`c6d8affd56de7e72dc9729feb3215b6faacff87b91cc04504851ce5f1379752c`.
It is a custom kernel with appended SO-01K DTBs, not a stock boot or
FOTAKernel image. The P4.51 image retained this same kernel byte for byte.

The corresponding public source base is
[`ATI-Experiments/android_kernel_sony_msm8998`](https://github.com/ATI-Experiments/android_kernel_sony_msm8998),
branch `lineage-22.1`, commit
`7a4c7d73f8ec401b89bd1ab815d2b4ccf801ec3f`. The upstream branch HEAD
was checked read-only against that SHA during the Phase 1C audit. Two local
commits add the SO-01K source:

| Historical commit | Preserved delta | Purpose |
| --- | --- | --- |
| `8fd4a2c52c6dbec7ca6ea201547c95c581ae1180` | `0001-so01k-device-tree-and-config.patch` | SO-01K DTS, DTB Makefile/Kconfig wiring, and defconfig. |
| `a735ff340c85518cc72e2f129988430c05617258` | `0002-so01k-cpu-cluster-masks.patch` | Correct Yoshino cluster masks in the SO-01K defconfig. |

These diffs preserve source content without publishing the unrelated local
diagnostic edits now present in the disposable kernel worktree. They were
generated from the two genuine commits; the original kernel source was not
modified. Apply to a clean upstream checkout:

```sh
git checkout 7a4c7d73f8ec401b89bd1ab815d2b4ccf801ec3f
git apply --check /path/to/android_twrp_so01k_patches/kernel-source/0001-so01k-device-tree-and-config.patch
git apply /path/to/android_twrp_so01k_patches/kernel-source/0001-so01k-device-tree-and-config.patch
git apply --check /path/to/android_twrp_so01k_patches/kernel-source/0002-so01k-cpu-cluster-masks.patch
git apply /path/to/android_twrp_so01k_patches/kernel-source/0002-so01k-cpu-cluster-masks.patch
```

The five affected files were checked byte for byte against local commit
`a735ff340c85518cc72e2f129988430c05617258` after applying the patches
to a fixture from the pinned base. The frozen prebuilt's extracted IKCONFIG is
also preserved as `p451-ikconfig`, SHA-256
`5171a10a5c31390a7a6269543282d17a8ab2b172b201978ae409e7c22d8bb63c`.
It matched the saved original build `.config` byte for byte. The active source
defconfig is `arch/arm64/configs/lineage-msm8998-yoshino-poplar_docomo_defconfig`.

The source tree's `COPYING` is GPL version 2, and the original kernel Makefiles
and build scripts come from the pinned upstream source. The saved build used
Android Clang r522817 (Clang 18.0.1) and AArch64/ARM32 GCC 4.9 cross-prefixes.
Its custom `KBUILD_BUILD_*` labels and source paths remain embedded in the
immutable prebuilt; they are build provenance, not credentials or device
serials. P4.14 validation matched a rebuilt decompressed control against the
earlier packed kernel using pinned baseline objects, but a fresh full-source
bit-for-bit rebuild of the frozen kernel has **not** been performed. No build
is run by this repository's public setup script.

The exact kernel bytes come from the device repository. The upstream source,
these two patches, the extracted config, and the original Makefiles provide
the public source path. A separate SO-01K kernel GitHub fork is therefore not
required for this source-state preservation plan. Publish this source patch
repository before publishing the device repository that carries the binary.
