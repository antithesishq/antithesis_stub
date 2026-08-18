// libvoidstar.so, the Antithesis link-time stub.
//
// Software that uses the Antithesis SDKs or LLVM coverage instrumentation 
// links against a library named libvoidstar.so. Inside Antithesis, that 
// name resolves to a library which serves as the runtime interface between
// your program and Antithesis. Outside Antithesis, it resolves to this stub,
// so that the same binary runs anywhere.
//
// This file is self-contained. Build it with any C compiler:
//
//     cc -O2 -shared -fPIC -o libvoidstar.so libvoidstar.c

// For syscall and _exit under strict -std= modes.
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

// The interface generation this stub implements: which functions it exports
// and what their return values mean.
#define VOIDSTAR_STUB_ABI_VERSION 2

#ifdef __cplusplus
extern "C" {
#endif

uint64_t fuzz_get_random(void) {
    uint64_t rand;
    if (syscall(SYS_getrandom, &rand, sizeof(rand), 0) != (long)sizeof(rand)) {
        fprintf(stderr, "Unable to get random number.\n");
        _exit(1);
    }
    return rand;
}

void fuzz_json_data(const char *message, size_t length) {
    (void)message;
    (void)length;
}

void fuzz_flush(void) {}

// The following symbols are indeed reserved identifiers, since we're
// implementing functions defined in the compiler runtime. Not clear how to
// get Clang on board with that besides narrowly suppressing the warning in
// this case. The sample code on the CoverageSanitizer documentation page
// fails this warning!
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-identifier"
#endif

// Deliberately leaves the guards zeroed; see the comment in the README.
void __sanitizer_cov_trace_pc_guard_init(uint32_t *start, uint32_t *stop) {
    (void)start;
    (void)stop;
}

void __sanitizer_cov_trace_pc_guard_internal(uint32_t *guard, uint64_t edge) {
    (void)guard;
    (void)edge;
}

void __sanitizer_cov_trace_pc_guard(uint32_t *guard) {
    (void)guard;
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

size_t init_coverage_module(size_t edge_count, const char *symbol_file_name) {
    (void)edge_count;
    (void)symbol_file_name;
    return (size_t)1 << 32;
}

bool notify_coverage(size_t edge_plus_module_id) {
    (void)edge_plus_module_id;
    return false;
}

uint64_t notify_coverage_v2(size_t edge_plus_module_id,
                            uint64_t hit_since_last_call) {
    (void)edge_plus_module_id;
    (void)hit_since_last_call;
    return (0xFFFFFull << 20) | 0xFFFFFull;
}

const uint64_t *coverage_lease_generation_addr(void) {
    static const uint64_t never_changes = 0;
    return &never_changes;
}

uint64_t instrumentation_max_abi_version(void) {
    return VOIDSTAR_STUB_ABI_VERSION;
}

uint64_t instrumentation_request_abi_version(uint64_t requested) {
    (void)requested;
    // Stub library always returns the max, no matter what you request.
    return VOIDSTAR_STUB_ABI_VERSION;
}

#ifdef __cplusplus
}
#endif
