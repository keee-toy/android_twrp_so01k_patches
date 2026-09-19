#!/usr/bin/env bash
set -euo pipefail

if [[ $# -ne 1 ]]; then
    echo "usage: $0 /path/to/fresh/twrp-12.1" >&2
    exit 2
fi
checkout=$(cd "$1" && pwd -P)
self=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)
device="$checkout/device/sony/poplar_docomo"
vold="$checkout/system/vold"
qcom="$checkout/device/qcom/twrp-common"
recovery="$checkout/bootable/recovery"

fail() { echo "P4.51 public setup: $*" >&2; exit 1; }
hash_file() { sha256sum "$1" | awk '{print $1}'; }
check_hash() {
    [[ -f "$2" && $(hash_file "$2") == "$1" ]] || fail "hash mismatch: $2"
}
check_project() {
    local path=$1 expected=$2 actual
    [[ -d "$path/.git" || -f "$path/.git" ]] || fail "missing Git project: $path"
    actual=$(git -C "$path" rev-parse HEAD)
    [[ $actual == "$expected" ]] || fail "unexpected base HEAD at $path: $actual"
    [[ -z $(git -C "$path" status --porcelain --untracked-files=all) ]] || fail "dirty project: $path"
}

check_project "$device" ee171c098acbc770127778f20d3e210f5494ac15
check_project "$vold" a164ba05c5fef288059774a776b2e6e1119957cf
check_project "$qcom" 98506f7919102378c8d52ee7d6a94a867f1b4c55
check_project "$recovery" 5c3d206a5eeb3d446bcda8248a405a4b278bab5c

check_hash d970bae62fcb161749d5268d26feef8923e3eabfcb096bf8b4e7d9972f2538f7 "$self/patches/p446-vold-keymaster.patch"
check_hash 21df5278bad01121f13dd62afcf6d50471f4729e6d729dbbd980c0f62b4a9339 "$self/patches/p449-vold-fscrypt.patch"
check_hash 9a40ce03d3b1ccf018ce1b728f54992a9733089e455ee3c38855a7d33203d463 "$self/patches/prepdecrypt-nonab.patch"
check_hash ea52d71429d1cfd744a604b0498eabb2561cb1ceb9182f1d2f18a9f00de3ec0a "$self/vintf/p447-framework-manifest.xml"
check_hash c579edbb131dd7e7e2f2a3b8abfafe30b3172883b2af34fedf986e4c317f2c39 "$device/recovery/root/system/etc/twrp.flags"
[[ ! -e "$device/recovery/crypto/stock" ]] || fail "private crypto material is present in the public device checkout"
cmp -s "$self/vintf/p447-framework-manifest.xml" "$device/recovery/root/system/etc/vintf/manifest.xml" || fail "P4.47 device manifest mismatch"
grep -Fq 'recovery/root/system/etc/vintf/manifest.xml:$(TARGET_COPY_OUT_RECOVERY)/root/system/etc/vintf/manifest.xml' "$device/device.mk" || fail "P4.47 manifest copy rule missing"
grep -Fq 'TARGET_PREBUILT_KERNEL := $(DEVICE_PATH)/prebuilt/Image.gz-dtb' "$device/BoardConfig.mk" || fail "SO-01K kernel configuration missing"
grep -Fq 'AB_OTA_UPDATER := false' "$device/BoardConfig.mk" || fail "SO-01K non-A/B configuration missing"
grep -Fq '/dev/block/bootdevice/by-name/FOTAKernel' "$device/recovery/root/system/etc/recovery.fstab" || fail "FOTAKernel mapping missing"
grep -Fq 'TW_EXCLUDE_DEFAULT_USB_INIT := true' "$device/BoardConfig.mk" || fail "ConfigFS USB configuration missing"

# Check every patch before changing any project.
git -C "$vold" apply --check "$self/patches/p446-vold-keymaster.patch"
git -C "$vold" apply --check "$self/patches/p449-vold-fscrypt.patch"
git -C "$qcom" apply --check "$self/patches/prepdecrypt-nonab.patch"

# Reinstall the pinned P4.47 source into its device-tree ramdisk input. The
# matching device.mk rule copies it to /system/etc/vintf/manifest.xml.
install -m 0644 "$self/vintf/p447-framework-manifest.xml" "$device/recovery/root/system/etc/vintf/manifest.xml"
git -C "$vold" apply "$self/patches/p446-vold-keymaster.patch"
git -C "$vold" apply "$self/patches/p449-vold-fscrypt.patch"
git -C "$qcom" apply "$self/patches/prepdecrypt-nonab.patch"

git -C "$vold" diff --check
git -C "$qcom" diff --check
git -C "$vold" apply --reverse --check "$self/patches/p446-vold-keymaster.patch"
git -C "$vold" apply --reverse --check "$self/patches/p449-vold-fscrypt.patch"
git -C "$qcom" apply --reverse --check "$self/patches/prepdecrypt-nonab.patch"
cmp -s "$self/vintf/p447-framework-manifest.xml" "$device/recovery/root/system/etc/vintf/manifest.xml"
check_hash c579edbb131dd7e7e2f2a3b8abfafe30b3172883b2af34fedf986e4c317f2c39 "$device/recovery/root/system/etc/twrp.flags"
echo "P4.51 public source state applied: P4.46, P4.47, P4.49, prepdecrypt, and /system_root verified."
