#pragma once

#include <stdio.h>

typedef unsigned char byte;  // byte라는 데이터 타입 만들기 GF(2^8)
typedef unsigned char u8;
typedef unsigned int u32;

//==== from (rijndael-alg-fst.h)
#define GETU32(pt) (((u32)(pt)[0] << 24) ^ ((u32)(pt)[1] << 16) ^ ((u32)(pt)[2] <<  8) ^ ((u32)(pt)[3]))
#define PUTU32(ct, st) { (ct)[0] = (u8)((st) >> 24); (ct)[1] = (u8)((st) >> 16); (ct)[2] = (u8)((st) >>  8); (ct)[3] = (u8)(st); }


void AddRoundKey(byte state[16], byte rkey[16]);
void SubBytes(byte state[16]);
void ShiftRows(byte state[16]);
void MixCol(byte in[4], byte out[4]);
void MixColumns(byte state[16]);
void AES_Encrypt(byte PT[16], byte RK[11][16], byte CT[16]);

void InvSubBytes(byte state[16]);
void InvShiftRows(byte state[16]);
void InvMixCol(byte in[4], byte out[4]);
void InvMixColumns(byte state[16]);
void AES_Decrypt(byte CT[16], byte RK[11][16], byte PT[16]);

void AES_Eq_Decrypt(byte CT[16], byte RK[11][16], byte PT[16]);
void AES8_EqInvKey(byte rk[11][16], byte eqrk[11][16]);


//=== Key schedule 용
u32 RotWord(u32 w32);
u32 SubWord(u32 w32);
void AES32_Enc_KeySchedule(byte k[16], u32 rk[11][4]);
void AES32_Dec_KeySchedule(byte k[16], u32 rk[11][4]);
void AES8_KeySchedule(byte k[16], byte rk[11][16]);








