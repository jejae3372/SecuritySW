//GF(2)유한체에서의 A^(-1) 역행렬 구하기

#include <stdio.h>

typedef unsigned char byte;
#define MATRIX_MAX_SIZE 20

typedef struct{
    int M[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE];
    int row;
    int col;
}GF_Matrix;

//유한체 연산 
//======================================

// GF(2) 덧셈: 1 + 1 = 0, 1 + 0 = 1 --> xor 
int GF_add(int f, int g) {
    return f ^ g;
}

byte GF_xtime(byte f){
    return (((f >> 7) & 0x01) == 1 ? (f << 1) ^ 0x1b : f << 1);
}



//GF(2) 곱셈: 1 * 0 = 0, 0 * 0 = 0
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

//======================================



//배열로 행렬 구조체 초기화하기
GF_Matrix GF_Mat_init(int A[][8], int row, int col){       //8X8 행렬을 기준으로 초기화 하기
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

//행렬 출력하기
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

void GF_Mat_Inv_print(GF_Matrix Mat){       // 가운데 | 를 출력해주는 함수
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

GF_Matrix GF_Mat_Mul(GF_Matrix A, GF_Matrix B){
    GF_Matrix AB;
    AB = GF_Mat_init();
    if(A.col != B.row){     //행과 열의 사이즈가 맞지 않으면 곱을 할 수 없음
        printf("Matrix Size Error!\n");
        return AB;
    }

    AB.row = A.row;
    AB.col = B.col;
    for(int i = 0; i < AB.row; i++){
        for(int j = 0; j < AB.col; j++){
            AB.M[i][j] = 0.0;
            for(int k = 0; k < AB.col; k++){
                AB.M[i][j] += A.M[i][k] * B.M[k][j];
            }
        }
    }
    return AB;
}

// 행렬 덧셈
GF_Matrix GF_Mat_Add(GF_Matrix A, GF_Matrix B){
    GF_Matrix result;
    result = GF_Mat_init();
    if ((A.col != B.col)||(A.row != B.row)){
        printf("Matrix size error!\n");
        return result;
    }
    result.row = A.row;
    result.col = A.col;
    for (int i = 0; i < result.row; i++){
        for(int j = 0; j < result.col; j++){
            result.M[i][j] = A.M[i][j] + B.M[i][j];
        }
    }
    return result;

}


//역행렬 구하기에 필요한 연산들...
//==================================

//두 행을 바꾸기 (call by reference)
void GF_Mat_Exchange_Row(GF_Matrix &A, int row1, int row2){
    int temp;
    for(int j = 0; j < A.col; j++){
        temp = A.M[row1][j];
        A.M[row1][j] = A.M[row2][j];
        A.M[row2][j] = temp;
    }
}

//한 행의 상수배를 다른 행에 더하기
void GF_Mat_Row_Add(GF_Matrix &A, int row_src, int row_target){
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
            GF_Mat_Exchange_Row(AA, j, pivot_row);
        }

        for(int i = 0; i< AA.row; i++){
            if((i != j) && (AA.M[i][j] == 1)){      //자기자신의 행은 건들이지 않으며 0이 아닌 행 + pivot_row 
                GF_Mat_Row_Add(AA, j, i);
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

//AES_inverse 반환함수
GF_Matrix GF_Matrix_inverse(){
    GF_Matrix MA, MInvA;
    int arrA[8][8] = {
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

//x = (A^(-1)*(y+b))^(-1)

byte AES_Affine_inverse(byte y){     //Sbox(x^(-1)) 1byte 를 받아옴
    byte y_vec[8], x_vec[8], add_vec[8], x;
    GF_Matrix InvA;
    InvA = GF_Matrix_inverse();    //A^(-1) 가져옴

    byte b_vec[8] = {1, 1, 0, 0, 0, 1, 1, 0};
    for (int i = 0; i < 8; i++){
        y_vec[i] = (y >> i) & 0x01;  //[y0, y1, y2, y3, ... , y7]
        
    }

//A^(-1) * (y + b)
    for(int i = 0; i < 8 ; i ++){
        x_vec[i] = InvA.M[i][0] * (y_vec[0]^b_vec[0]);
        for(int j = 1; j < 8; j++){
            x_vec[i] ^= InvA.M[i][j] * (y_vec[j]^b_vec[j]);
        }
    }


//벡터 -> 비트
    byte temp_bit;
    x = 0;
    for (int i = 0; i < 8; i++){
        temp_bit = x_vec[i] << i;
        x ^= temp_bit;
    }
    byte x_inv;         //(A^(-1) * (y + b))^(-1)
    
    x_inv = (x == 0)? 0x00 : GF_inv(x);

    return x_inv;       //x = 0x00, 0x01, 0x02 .... 

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

//ISbox
// x = IS(y) : A*x^(-1) = y + b in GF(2^8)
//            --> x^(-1)  = A^(-1)*(y+b) --> x = (A^(-1)*(y+b))^(-1)

void Get_AES_ISbox(byte isbox[256]){
    byte sbox[256];
    Get_AES_Sbox(sbox);
    for (int i = 0; i < 256; i++){
        isbox[sbox[i]] = AES_Affine_inverse(sbox[i]);   //AES_Affine_inverse(sbox[i]) = 0x00, 0x01, ...
    }
    
}

void Sbox_test(){
    byte sbox[256], isbox[256];

    Get_AES_ISbox(isbox);

    printf("{");
    for(int i = 0; i < 256; i++){
        if(((i % 16) == 0) && (i != 0))   printf("\n");
        if((i == 255)){
            printf("%02x", isbox[i]);
        }else{
            printf("%02x, ", isbox[i]);
        }
    }
    printf("}");
}

int main(){
    Sbox_test();
    
}