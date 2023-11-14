#include <stdio.h>
typedef unsigned char byte;

//========================================


byte Sbox[256] = {
 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
 0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

byte ISbox[256] = {
 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};


//수업시간에 작성한 코드
byte GF_xtime(byte f){
    return (((f >> 7) & 0x01) == 1 ? (f << 1) ^ 0x1b : f << 1);
}
// GF(2^8) 곱셈 : h(x) = g(x)h(x) mod m(X)
byte GF_mul(byte f, byte g){
    byte h;         //결과 저장 h(x) = f(x)g(x)
    int coef;       //현재 계산할 계수
    h = 0x00;       //초기값
    for(int i = 7; i >=0; i--){     //7차부터 상수항까지
        coef = (f >> i) & 0x01;
        h = GF_xtime(h);
        if(coef == 1){
            h ^= g;           //GF_add(h, g)       //계수가 1이면 더하기, 0이면 그대로 두기
        }
    }
    return h;
}

//거듭제곱 a^254 : a의 곱셈의 역원

byte GF_inv(byte a){        //곱셈의 역원을 계산하여 돌려준다.
    byte result;  //결과값
    byte temp; //중간값    a^2, a^4, a^8, .... , a^128
    result = 1;
    temp = a;
    for(int i = 0; i < 7; i++){
        temp = GF_mul(temp, temp); //거듭제곱. a^2, a^4, a^8, .... , a^128
        result = GF_mul(result, temp);
    }
    return result;
}

//Affine 변환 w --> Ax + b
byte AES_Affine(byte w){
    byte A[8][8] = {
        {1, 0, 0, 0, 1, 1, 1, 1},
        {1, 1, 0, 0, 0, 1, 1, 1},
        {1, 1, 1, 0, 0, 0, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 1, 1, 1, 1, 1}
    };
    byte b_vec[8] = {1, 1, 0, 0, 0, 1, 1, 0};
    byte w_vec[8], y_vec[8], y;
// w = [w7, w6, w5 ... w0]
// --> w_vec[8] = [w0, w1, w2, .... ,w7]
    for (int i = 0; i < 8; i++){
        w_vec[i] = (w >> i) & 0x01;
    }

// Aw+b
    for (int i = 0; i < 8; i++){
        y_vec[i] = b_vec[i];
        for(int j = 0; j < 8; j++){
            y_vec[i] ^= A[i][j] * w_vec[j]; //y_vec[i] = y_vec[i]^A[i][j] * w_vec[j]
        }
    }
//벡터 -> 비트
    byte temp_bit;
    y = 0;
    for (int i = 0; i < 8; i++){
        temp_bit = y_vec[i] << i;
        y ^= temp_bit;
    }
    return y;
}

//AES의 Sbox 만들기 --> sbox[] 저장
void Get_AES_Sbox(byte sbox[256]){
    byte temp;
    //0의 역원 0^(-1) = 0으로 간주
    sbox[0] = AES_Affine(0);
    for(int x = 1; x < 256; x++){
        temp = GF_inv(x); //temp = x^(-1) 곱의 역원 
        sbox[x] = AES_Affine(temp); // A*x^(-1) + b
    }
}

// ISbox : Sbox의 역함수
//  y = S(x) : y = A*x^(-1) + b
// x = IS(y) : A*x^(-1) = y + b in GF(2^8)
//            --> x^(-1)  = A^(-1)*(y+b) --> x = (A^(-1)*(y+b))^(-1)
void Get_AES_ISbox(byte isbox[256]){
    byte sbox[256];
    Get_AES_Sbox(sbox);
    for (int i = 0; i < 256; i++){
        isbox[sbox[i]] = i;
    }
}

//===============================

void Generate_Sbox(){
    byte Sbox[256];
    byte ISbox[256];

    Get_AES_Sbox(Sbox);

    printf("byte Sbox[256] = {\n");
    for (int i = 0; i < 256; i++){
        printf(" 0x%02x,", Sbox[i]);        //ex) 0xff,
        if(i % 16 == 15) printf("\n");
    }
    printf("   };\n\n");

    Get_AES_ISbox(ISbox);

    printf("byte ISbox[256] = {\n");
    for (int i = 0; i < 256; i++){
        printf(" 0x%02x,", ISbox[i]);        //ex) 0xff,
        if(i % 16 == 15) printf("\n");
    }
    printf("   };\n\n");
}
/*
AES Encryption

    AddRoundKey
    
     (AES round function) : 9번 반복
        SubBytes
        ShiftRows
        MixColumns
        AddRoundKey

     (Last round)
        SubBytes
        ShiftRows
        AddRoundKey


*/
void AddRoundKey(byte state[16], byte rkey[16]){
    for(int i = 0; i < 16; i++){
        //state[i] = state[i] ^ rkey[i];
        state[i] ^= rkey[i];
    }
}
void SubBytes(byte state[16]){
    for(int i = 0; i < 16; i++){
        state[i] ^= Sbox[state[i]];
    }
}

/*
    0   4    8  12              0   4    8  12                  
    1   5    9  13     -->      5   9   13   1  1칸씩 왼쪽
    2   6   10  14             10  14    2   6  2칸씩 왼쪽
    3   7   11  15             15   3    7  11  3칸씩 왼쪽
*/

void ShiftRows(byte state[16]){
    byte temp;

    temp = state[1]; state[1] = state[5]; state[5] = state[9];
    state[9] = state[13]; state[13] = temp; // 2번째 row

    temp = state[2]; state[2] = state[10]; state[10] = temp;
    temp = state[6]; state[6] = state[14]; state[14] = temp; //3번째 row 두개씩 서로 자리를 바꾸는 것 과 같다
    
    temp = state[3]; state[3] = state[15]; state[15] = state[11];
    state[11] = state[7]; state[7] = temp;//4번째 row 
}

void MixCol(byte in[4], byte out[4]){
    byte MC[4][4] = {{0x02, 0x03, 0x01, 0x01,}, {0x01, 0x02, 0x03, 0x01},
                        {0x01, 0x01, 0x02, 0x03}, {0x03, 0x01, 0x01, 0x02}  };
     for(int i = 0; i < 4; i++){
        out[i] = 0;
        for (int j = 0; j < 4; j++){
            //out[i] = out[i] + MC[i][j] * in[j];
            // 또는 
            //temp = MC[i][j] * in[j];
            //out[i] ^= temp;
            out[i] ^= GF_mul(MC[i][j], in[j]);
        }
    }
}
   
void MixColumns(byte state[16]){
    byte in[4], out[4];
    for(int col = 0; col < 4; col++){
        for(int i = 0; i < 4; i++) in[i] = state[col * 4 + i];
        MixCol(in, out);
        for(int i = 0; i < 4; i++) state[col * 4 + i] = out[i];     //state 를 in 으로 사용 후 out을 다시 저장
    }
}


int main(){
    Generate_Sbox();

}
