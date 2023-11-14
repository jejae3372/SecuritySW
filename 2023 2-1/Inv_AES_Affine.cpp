//GF(2)유한체에서의 A^(-1) 역행렬 구하기

#include <stdio.h>

typedef unsigned char byte;
#define MATRIX_MAX_SIZE 20

typedef struct{
    byte M[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE];
    int row;
    int col;
}GF_Matrix;

//유한체 GF 연산 모음
//======================================
byte xtime(byte a) {
    int msb; //최고차항의 계수 (a7)
    byte result;
    msb = (a >> 7) & 0x01;  // 0x01 = 0000 0001 (비트 마스크)
    if (msb == 1) { //a7=1
        result = (a << 1) ^ 0x1b;  // a(x)*x + (x^4+x^3+x+1)
    }
    else { //a7=0
        result = a << 1;
    }
    return result;
}

//GF(2) 곱셈: 1 * 0 = 0, 0 * 0 = 0
byte GF_mul(byte f, byte g) {     
    byte h; // 결과저장용 h = g*f
    h = f * g;
    return h;
}

// GF(2) 덧셈: 1 + 1 = 0, 1 + 0 = 1 --> xor 
byte GF_add(byte f, byte g) {
    return f ^ g;
}

//곱셈의 역원 구하기
byte GF_inv(byte a) {  // 곱셈의 역원
    byte result;  // 결과값
    byte temp;    // 중간계산 a^2, a^4, a^8, a^16, ... , a^128
    result = 1;
    temp = a;
    for (int i = 0; i < 7; i++) {
        temp = GF_mul(temp, temp); // a^2, a^4, a^8, ... , a^128
        result = GF_mul(result, temp);
    }
    return result;
}
//======================================


//배열로 행렬 구조체 초기화하기
GF_Matrix GF_Mat_init(byte A[][8], int row, int col){       //8X8 행렬을 기준으로 초기화 하기
    GF_Matrix result;
    result.row = row;
    result.col = col;
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            result.M[i][j] = A[i][j];
        }
    }
    return result;
}

//빈 초기화 (특별히 지정하지 않은 경우),
GF_Matrix GF_Mat_init(){      //오버로드 된 상태. 파라미터에 의해서 동일한 이름의 함수들 중 무엇을 사용할지 결정됨
    GF_Matrix result;
    result.row = 1;
    result.col = 1;
    result.M[0][0] = 0;

    return result;
}


//행렬 함수들
void GF_Mat_print(GF_Matrix Mat){
    for(int i = 0; i < Mat.row; i++){
        printf("[");
        for(int j = 0; j < Mat.col; j++){
            printf(" %d", Mat.M[i][j]);        //GF(2)의 원소이므로 0 또는 1의 정수만 출력
        }
        printf("]\n");
    }
    printf("\n");
}

void GF_Mat_Inv_print(GF_Matrix Mat){
    for(int i = 0; i < Mat.row; i++){
        printf("[");
        for(int j = 0; j < Mat.col; j++){
            
            printf(" %d", Mat.M[i][j]);
            
            if((j + 1) == (Mat.col / 2)){
                printf(" |");
            }
        }
        printf("]\n");
    }
    printf("\n");
}

// 행렬 곱  AB
GF_Matrix GF_Mat_Mul(GF_Matrix A, GF_Matrix B) {
    GF_Matrix AB;
    AB = GF_Mat_init();
    if (A.col != B.row) {
        printf("Matrix size error!\n");
        return AB;
    }

    byte temp;
    AB.row = A.row;
    AB.col = B.col;
    for (int i = 0; i < AB.row; i++) {
        for (int j = 0; j < AB.col; j++) {
            AB.M[i][j] = 0x00;
            for (int k = 0; k < A.col; k++) {
                temp = GF_mul(A.M[i][k], B.M[k][j]);
                AB.M[i][j] = GF_add(AB.M[i][j], temp);
            }
        }
    }
    return AB;
}


//역행렬 구하기에 필요한 연산들...
//두 행을 바꾸기 (call by reference)
void Mat_Exchange_Row(GF_Matrix &A, int row1, int row2){
    byte temp;
    for(int j = 0; j < A.col; j++){
        temp = A.M[row1][j];
        A.M[row1][j] = A.M[row2][j];
        A.M[row2][j] = temp;
    }
}

//한 행의 상수배를 다른 행에 더하기
void Mat_Row_Add(GF_Matrix &A, int row_src, int row_target){
    for(int j = 0; j < A.col; j++){
        A.M[row_target][j] = GF_add(A.M[row_target][j], A.M[row_src][j]);
    }
}

//역행렬 구하기
GF_Matrix GF_Mat_inverse(GF_Matrix A){
    GF_Matrix InvA;
    InvA = GF_Mat_init();

    //행렬 A와 단위행렬 I 를 붙여 행렬 AA를 만든다. AA = [A][I]
    GF_Matrix AA;
    AA.row = A.row;
    AA.col = A.col * 2;
    for(int i = 0; i < A.row; i++){
        for(int j = 0; j < A.col; j++){
            AA.M[i][j] = A.M[i][j];
            AA.M[i][A.col + j] = (i==j)? 1:0;  //A.col + j번째 열 부터 단위 행렬 만들기
        }
    }
    //Reduced REF(Row Echelm Form) 사다리꼴 만들기
    int pivot_row;
    for(int j = 0; j < A.col; j++){
        pivot_row = -1; //초기에 -1로 설정하고, 처음 0이 아닌 값이 나오는 열 탐색
        for(int i = j; i < A.row; i++){
            if(AA.M[i][j] == 1){      //1이 나오는 열이 있으면.
                pivot_row = i;
            }
        }
        if(pivot_row != j){             //해당 row가 pivot_row가 아니라면, pivot_row 와 바꾼다
            Mat_Exchange_Row(AA, j, pivot_row);
        }

        for(int i = 0; i< AA.row; i++){
            if((i != j) && (AA.M[i][j] == 0x01)){      //자기자신의 행은 건들이지 않으며 0이 아닌 행 + pivot_row 
                Mat_Row_Add(AA, j, i);
            }
        }     
        if(pivot_row == -1){    //해당 열의 모든 원소가 0인 경우 역행렬을 구할 수 없다.
                printf("A is not invertable.\n");
                printf("%d", j);
                return InvA;    //의미없는 리턴
        }

    }

    InvA.row = A.row;
    InvA.col = A.col;
    for(int i = 0; i < A.row; i++){
        for(int j = 0; j < A.col; j++){
            InvA.M[i][j] = AA.M[i][A.col + j];      //[I|InvA]
        }
    }
    return InvA;
}

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
    for(int i = 0; i < 8; i++){
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

GF_Matrix AES_Inv_Affine(){
    GF_Matrix MA, MInvA;
    byte arrA[8][8] = {
        {1, 0, 0, 0, 1, 1, 1, 1},
        {1, 1, 0, 0, 0, 1, 1, 1},
        {1, 1, 1, 0, 0, 0, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 0, 0, 0},
        {0, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 0},
        {0, 0, 0, 1, 1, 1, 1, 1}
    };
    MA = GF_Mat_init(arrA, 8, 8);
    MInvA = GF_Mat_inverse(MA);

    return MInvA;
}
byte Get_AES_ISbox(byte y){
    GF_Matrix MInvA;
    byte x_vec[8], y_vec[8], x;

    for (int i = 0; i < 8; i++){
        y_vec[i] = (y >> i) & 0x01;
    }

    MInvA = AES_Inv_Affine(); //A^(-1) 가져오기

    byte b_vec[8] = {1, 1, 0, 0, 0, 1, 1, 0};
    byte y_add_b[8] ={0, };
    
    for (int i = 0; i < 8; i++){
        y_add_b[i] = y_vec[i] ^ b_vec[i];   //(y+b)
        for(int j = 0; j < 8; j++){
        x_vec[i] ^= MInvA.M[i][j] * y_add_b[j]; // A^(-1) * (y + b)
        } 
    }
    
    byte temp_bit;
    byte x_inv;
    x = 0;
    for (int i = 0; i < 8; i++){
        temp_bit = x_vec[i] << i;
        x ^= temp_bit;
    }
    if(x != 0x00){
        x_inv = GF_inv(x);
    }else{
        x_inv = 0x00;
    }
    
    return x_inv;
}


//벡터 -> 비트
    
//Sbox -> ISbox
//ISbox -> x = (A^(-1) * (y + b))^(-1)
//Affine 변환 w --> Ax + b


//AES의 Sbox 만들기 --> sbox[] 저장
void Get_AES_Sbox(byte sbox[256]){
    byte temp;
    //0의 역원 0^(-1) = 0으로 간주
    sbox[0] = AES_Affine(0);
    for(int x = 1; x < 256 ; x++){
        temp = GF_inv(x); //temp = x^(-1) 곱의 역원 
        sbox[x] = AES_Affine(temp); // A*x^(-1) + b
    }
}

int main(){

    byte sbox[256], isbox[256];
    Get_AES_Sbox(sbox);
    printf("[");
    for(int j = 0; j < 256; j++){
        if(j % 16 == 0) printf("\n");
        printf("%02x, ", sbox[j]);
    }
    for(int i = 0; i < 256; i++){
        isbox[i] = Get_AES_ISbox(sbox[i]);
    }

}