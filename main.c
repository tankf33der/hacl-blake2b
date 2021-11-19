#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include "monocypher.h"
#include "monocypher-ed25519.h"
#include "Hacl_Hash_Blake2.h"


#define ARRAY(name, size) \
    uint8_t name[size]; \
    for(size_t i = 0; i < size; i++) name[i] = i;

//@ ensures \result == 0;
int blake2b(void) {
    ARRAY(hash1, 64);
    ARRAY(hash2, 64);
    ARRAY(key,  64);
    ARRAY(in,   64);
	int status = 0;

    for(size_t h = 1; h <= 64; h += 8)
        for(size_t k = 0; k <= 64; k += 8)
            for(size_t i = 0; i <= 64; i += 8) {
            	hash1[1] = 123;
                crypto_blake2b_general(hash1, h, key, k, in, i);
                Hacl_Blake2b_32_blake2b(h, hash2, i, in, k, key);
                status |= crypto_verify64(hash1, hash2);
    }

	// hash1[0] = 123;
    // crypto_blake2b_general(hash1, 1, key, 8, in, 0);
    // Hacl_Blake2b_32_blake2b(1, hash2, 0, in, 8, key);
    // status |= crypto_verify64(hash1, hash2);

    return status;
}



int main(void) {
	int status = 0;

	status |= blake2b();

	printf("%s\n", status != 0 ? "FAIL" : "OK");
	return status;
}
