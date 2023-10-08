#include <iostream>
#include <stdio.h>
typedef unsigned char byte;   //unsignde char 8비트 0000 0000

//GF(2^8) 덧셈
byte GF_add(byte a, byte b){
    
    return a ^ b;
}


//GF(2^8) xtime 다항식에 x 곱하기
byte xtime(byte a){
    byte result;
    int msb; //최고차항의 계수 (a7)
    msb = (a >> 7) & 0x01;  //0x01 = 0000 0001  (비트 마스크) 여기선 굳이 할 필요가 없음
    if(msb == 1){       //a7 = 1  a8이 등장하므로, 쉬프트 후 x^8(mod m(x)) = x^4 + x^3 + x + 1 이용
        result = (a << 1) ^ 0x1b;   //a(x) * x + (x^4 + x^3 + x + 1)
    }else{      //a7 = 0 
        result = a << 1;
    }
    return result;

}

//GF(2^8) xtime = 한 줄 버전 채우기
byte GF_xtime(byte f){
    return (((f >> 7) & 0x01) == 1 ? (f << 1) ^ 0x1b : f << 1);
}

//이진수로 출력하기
void print_bin(byte x){
    // x = [x7 x6 x5 ... x0]
    //     --> x_bin[8] = {x7, x6, x5, .... , x0}
    byte x_bin[8];
    for (int i = 0; i < 8; i++){
        x_bin[i] = (x >> (7-i)) & 0x01;
    }
    for (int i=0;  i< 8; i++){
        printf("%x", x_bin[i]);
    }
}
//다항식으로 출력하기
void print_poly(byte f){
    //x  = [a7, a6, a5, a4 ... a0]
    // --> a7 x^7 + a6 x^7 + .... + a0

    byte f_bin[8];
    for(int i = 0; i < 8; i++){
        f_bin[i] = (f >> (7-i)) & 0x01;        
    }
    int coef;
    int sum = 0;
    for(int i = 0; i< 7; i++){
        coef = f_bin[i];
        if (coef == 1){
            if(sum == 0){
                printf("x^%1d", 7 - i);
            }else{
                printf(" + x^%1d", 7 - i);
            }
            sum+= 1;
        }
    }
    coef = f_bin[7];
    if (coef == 1){
        if(sum != 0){
            printf(" + 1\n");
        }else{
            printf("1\n");
        }
    }else{
        printf("\n");
    }
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
            h = GF_add(h, g);       //계수가 1이면 더하기, 0이면 그대로 두기
        }
    }
    return h;
}

//거듭제곱 a^254
//254 = 255 - 1 = 1111 1110    a*a^254 = a^255 = 1
// = (1000 0000) + (0100 0000) + (0010 0000) + .... + (0000 0010)
// 지수법칙: a^(m+n) = a^m + a^n
// a^254 = a^(255 - 1) = a^((1000 0000) + (0100 0000) + (0010 0000) + .... + (0000 0010)) 
//       = a^(1000 0000) * a^(0100 0000) * a^(0010 0000) * .... * a^(0000 0010) xtime으로 하면 a^2 에 두배로 거듭제곱후 모두 xtime 하므로 13번 실행

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

// GF(2^8) 원소로 이루어진 행렬을 위한 데이터 타입 준비
#define MATRIX_MAX_SIZE 4 //pre-process 가 처리하는 매크로
// GF_Matrix 구조체 정의
struct GF_Matrix {
    byte M[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE];
    int row;
    int col;

};
//GF vector 구조체 정의
struct GF_vector{
    byte v[MATRIX_MAX_SIZE];
    int n; // 벡터의 크기
};
//GF Matrix 초기화 하는 함수
GF_Matrix GF_Mat_init(){
    GF_Matrix M;

    M.row = 1;
    M.col = 1;
    M.M[0][0] = 0x00;
    return M;
}
//GF Matrix 출력
void GF_Mat_print(GF_Matrix X){
    for(int r = 0; r < X.row; r++){
        printf("[");
        for(int c = 0; c< X.col; c++){
            printf(" %02x", X.M[r][c]);
        }
        printf("]\n");
    }
    printf("\n");
}

//GF_vector 출력
void GF_Vec_print(GF_vector GV){
    printf("[");
    for (int i = 0; i< GV.n; i++){
        printf(" %02x", GV.v[i]);
    }
    printf("]\n");
}

GF_Matrix GF_Mat_MixCol(){
    GF_Matrix MC;
    MC.row = 4;
    MC.col = 4;
    byte MC_array[4][4] = {{ 2, 3, 1, 1}, {1, 2, 3, 1}, {1, 1, 2, 3}, {3, 1, 1, 2}};
    for(int r = 0; r < MC.row; r++){
        for(int c = 0; c < MC.col; c++){
            MC.M[r][c] = MC_array[r][c];
        }
    }
    return MC;
}
//행렬과 벡터의 곱 (A, x) --> y = Ax
GF_vector GF_Mat_Vec_mul(GF_Matrix A, GF_vector x){
    GF_vector y;
    // A[m x n] x[n] --> y[m]
    y.n = A.row;
    if(A.col != x.n){
        printf("Matrix-vector size error.\n");
        return y; // 의미없는 데이터
    }
    for(int i = 0; i < y.n; i++){
        y.v[i] = 0x00;
        for(int j = 0; j < A.col; j++){
            //y.v[i] = y.v[i] + A.M[i][j] * x.v[j];
            y.v[i] = GF_add(y.v[i], GF_mul(A.M[i][j], x.v[j]));    //GF field 에서 정의한 덧셈 곱셈으로 표현 해야함
        }
    }
    return y;
}

//행렬과 행렬의 곱
GF_Matrix GF_Mat_mul(GF_Matrix A, GF_Matrix B){
    GF_Matrix C;
    C = GF_Mat_init();
    if(A.col != B.row){
        printf("Matrix size error.\n");
        return C; //의미없는 데이터 반환
    }
    C.row = A.row;
    C.col = B.col;
    for(int i = 0; i < C.row; i++){
        for(int j = 0; j < C.col; j++){
            C.M[i][j] = 0x00;
            //C[i][j] = a[i][1] * b[1][j] + .... + a[i][n]*b[n][j]
            for(int k = 0; k < A.col; k++){
                //C.M[i][j] = C.M[i][j] + A.M[i][k] * B.M[k][j];
                C.M[i][j] = GF_add(C.M[i][j], GF_mul(A.M[i][k], B.M[k][j]));
            }
        }
    }
    return C;
}


// b --> d = MixColumn(b)
GF_vector GF_MixCol(GF_vector b){
     GF_Matrix MixCol;
     MixCol = GF_Mat_MixCol();
     GF_vector d;
     d = GF_Mat_Vec_mul(MixCol, b);
     return d;
}
void MixCol_test(){
    /* GF_Matrix MixColMatrix;

    MixColMatrix = GF_Mat_MixCol();
    GF_Mat_print(MixColMatrix);
    
   byte x[4] = {1, 0, 0, 0};
   GF_vector b;
   b.n = 4;
   for(int i = 0; i< b.n; i++){
        b.v[i] = x[i];
    }
    GF_vector d;
    d = GF_MixCol(b);

    GF_Vec_print(b);
    GF_Vec_print(d);
*/
    GF_Matrix MixColMatrix;
    MixColMatrix = GF_Mat_MixCol();

    printf("MixColumn Matrix... \n");
    printf("MC = \n");
    GF_Mat_print(MixColMatrix);

    GF_Matrix MC2;
    MC2 = GF_Mat_mul(MixColMatrix, MixColMatrix);
    printf("MC^2 = \n");
    GF_Mat_print(MC2);

    GF_Matrix MC3;     
    MC3 = GF_Mat_mul(MC2, MixColMatrix);
    printf("MC^3 = \n");
    GF_Mat_print(MC3);

    GF_Matrix MC4;  //단위 행렬
    MC4 = GF_Mat_mul(MC3, MixColMatrix); //MixColMatrix 과 MC^3 하면 단위행렬 --> MC^3이 역행렬
    printf("MC^4 = \n");
    GF_Mat_print(MC4);
}

//Sbox 출력
void Sbox_test(){
    byte sbox[256], isbox[256];
    Get_AES_Sbox(sbox);
    Get_AES_ISbox(isbox);
    printf("Sbox = \n");

    for(int i = 0; i < 256; i++){
        if((i % 16) == 0)   printf("\n");
        printf("%02x, ", sbox[i]);
    }
    printf("\n\n");
    printf("ISbox = \n");

    for(int i = 0; i < 256; i++){
        if((i % 16) == 0)   printf("\n");
        printf("%02x, ", isbox[i]);
    }
}
int main(){
    MixCol_test();
    /*
    byte a, b, c, d;
    a = 0x03;   
    b = GF_inv(a);   
    c = GF_mul(a, b);

    printf("a = %02x = ", a);
    print_bin(a);
    printf(" = ");
    print_poly(a);

    printf("b = %02x = ", b);
    print_bin(b);
    printf(" = ");
    print_poly(b);

    printf("c = %02x = ", c);
    print_bin(c);
    printf(" = ");
    print_poly(c);

    c = GF_add(a, b);
    printf("a= %02x, b= %02x, c= %02x\n", a, b, c);
    d = xtime(a);
    printf("a = %02x, d= %02x\n", a, d);
    
    
    c = GF_mul(a, b);
    print_bin(c);
    printf(" ");
    print_poly(c);
    */


//정기수정완료333
}

