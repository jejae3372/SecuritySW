﻿
#include <stdio.h>
#include "AES32.cpp"

void Run_AES32_test() {
    byte pt[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };

    byte key[16] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                         0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };

    // ciphertext = "69c4e0d86a7b0430d8cdb78070b4c55a"
    byte correct_ct[16] = { 0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30,
                            0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a };

    byte ct[16];

    u32 enc_rk[11][4], dec_rk[11][4];
    
    printf("=== AES32 Enc/Dec Test ===\n\n");

    AES32_Enc_KeySchedule(key, enc_rk);

    AES32_Encrypt(pt, enc_rk, ct);

    printf("Encrypted CT = ");
    for (int i = 0; i < 16; i++) {
        printf("0x%02x ", ct[i]);
    }
    printf("\n");

    byte dec_pt[16];

    AES32_Dec_KeySchedule(key, dec_rk);
    AES32_EqDecrypt(ct, dec_rk, dec_pt);
    printf("Decrypted PT = ");
    for (int i = 0; i < 16; i++) {
        printf("0x%02x ", dec_pt[i]);
    }
    printf("\n");
}

int main()
{
    Run_AES32_test();
}
