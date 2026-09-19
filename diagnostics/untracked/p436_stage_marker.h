#pragma once

#include <stdint.h>

enum So01kP436Stage : uint32_t {
    SO01K_P436_SERVICE_START_ENTERED = 1,
    SO01K_P436_CHILD_ENTERED = 2,
    SO01K_P436_EXEC_IMMINENT = 3,
    SO01K_P436_EXEC_FAILED = 4,
    SO01K_P436_RUNTIME_PRE_MAIN = 5,
    SO01K_P436_MAIN_ENTERED = 6,
};

struct So01kP436StageRecord {
    uint32_t magic;
    uint32_t version;
    uint32_t stage;
    uint32_t stage_inverse;
    uint64_t boottime_ns;
    uint64_t checksum;
};
static_assert(sizeof(So01kP436StageRecord) == 32, "P4.36 record must remain 32 bytes");

static constexpr uint32_t kSo01kP436Magic = 0x50343653U;
static constexpr uint32_t kSo01kP436Version = 1U;
static constexpr uint64_t kSo01kP436ChecksumSeed = 0x534f30314b503436ULL;
static constexpr char kSo01kP436StagePath[] = "/tmp/p436-recovery-start-stage";

#if defined(__aarch64__)
static inline __attribute__((always_inline)) long so01k_p436_syscall6(
        long number, long a0, long a1, long a2, long a3, long a4, long a5) {
    register long x8 __asm__("x8") = number;
    register long x0 __asm__("x0") = a0;
    register long x1 __asm__("x1") = a1;
    register long x2 __asm__("x2") = a2;
    register long x3 __asm__("x3") = a3;
    register long x4 __asm__("x4") = a4;
    register long x5 __asm__("x5") = a5;
    __asm__ volatile("svc 0"
                     : "+r"(x0)
                     : "r"(x8), "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5)
                     : "memory", "cc");
    return x0;
}

static __attribute__((noinline)) void so01k_p436_publish_stage(uint32_t stage) {
    if (stage < SO01K_P436_SERVICE_START_ENTERED || stage > SO01K_P436_MAIN_ENTERED) return;
    So01kP436StageRecord previous = {};
    long previous_fd = so01k_p436_syscall6(56, -100,
                                          reinterpret_cast<long>(kSo01kP436StagePath),
                                          0x80000, 0, 0, 0);
    if (previous_fd >= 0) {
        long got = so01k_p436_syscall6(63, previous_fd, reinterpret_cast<long>(&previous),
                                      sizeof(previous), 0, 0, 0);
        (void)so01k_p436_syscall6(57, previous_fd, 0, 0, 0, 0, 0);
        uint64_t expected = kSo01kP436ChecksumSeed ^ previous.magic ^
                            (static_cast<uint64_t>(previous.version) << 32) ^ previous.stage ^
                            (static_cast<uint64_t>(previous.stage_inverse) << 32) ^
                            previous.boottime_ns;
        if (got == static_cast<long>(sizeof(previous)) && previous.magic == kSo01kP436Magic &&
            previous.version == kSo01kP436Version && previous.stage >= stage &&
            previous.stage <= SO01K_P436_MAIN_ENTERED &&
            previous.stage_inverse == ~previous.stage && previous.checksum == expected) return;
    }
    struct KernelTimespec { int64_t tv_sec; int64_t tv_nsec; } now = {};
    if (so01k_p436_syscall6(113, 7, reinterpret_cast<long>(&now), 0, 0, 0, 0) != 0 ||
        now.tv_sec < 0 || now.tv_nsec < 0 || now.tv_nsec >= 1000000000LL) return;
    So01kP436StageRecord record = {};
    record.magic = kSo01kP436Magic;
    record.version = kSo01kP436Version;
    record.stage = stage;
    record.stage_inverse = ~stage;
    record.boottime_ns = static_cast<uint64_t>(now.tv_sec) * 1000000000ULL +
                         static_cast<uint64_t>(now.tv_nsec);
    record.checksum = kSo01kP436ChecksumSeed ^ record.magic ^
                      (static_cast<uint64_t>(record.version) << 32) ^ record.stage ^
                      (static_cast<uint64_t>(record.stage_inverse) << 32) ^ record.boottime_ns;
    long fd = so01k_p436_syscall6(56, -100, reinterpret_cast<long>(kSo01kP436StagePath),
                                 0x80041, 0600, 0, 0);
    if (fd < 0) return;
    (void)so01k_p436_syscall6(64, fd, reinterpret_cast<long>(&record), sizeof(record), 0, 0, 0);
    (void)so01k_p436_syscall6(57, fd, 0, 0, 0, 0, 0);
}
#else
static inline void so01k_p436_publish_stage(uint32_t) {}
#endif
