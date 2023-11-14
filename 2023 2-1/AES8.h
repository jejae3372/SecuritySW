
typedef unsigned char byte;

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
void AES_Eq_Decrypt(byte CT[16], byte eq_RK[11][16], byte PT[16]);
void AES8_EqInvKey(byte rk[11][16], byte eqrk[11][16]);

void AES8_print_state(byte state[16]);