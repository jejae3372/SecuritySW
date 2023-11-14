// Struct_GF_Matrix.cpp : 구조체를 이용한 행렬 연산
// 6주차 소스코드 - GF(2^8)원소를 갖는 행렬의 기본연산 구현/역행렬 구하기
/* [수업시간에 사용할 미완성 코드임]
   실습 - Struct_R_Matrix.cpp를 참고하여   
         함수 GF_Matrix GF_Mat_inverse(GF_Matrix A) 만들기
*/

#include <stdio.h>

typedef unsigned char byte;
#define MATRIX_MAX_SIZE 8 


//====================================
//  GF(2^8) 함수들 (수업시간 작성)
//====================================
// GF(2^8) xtime    다항식에 x 곱하기
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

// GF(2^8) 곱셈: h(x) = g(x)f(x) mod m(x)
byte GF_mul(byte f, byte g) {
    byte h; // 결과저장용 h = g*f
    int coef; //  현재 계산할 계수
    h = 0x00; // 초기값 h(x) = 0
    for (int i = 7; i >= 0; i--) {  // 7차부터 상수항까지
        coef = (f >> i) & 0x01; // a7, a6, a5, .. ,a0
        h = xtime(h);
        if (coef == 1) {  //계수가 1이면 g(x)를 더하기, 0이면 그대로 두기
            h ^= g; //h = GF_add(h, g);
        }
    }
    return h;
}

// GF(2^8) 덧셈: h(x) = g(x) + f(x) mod m(x)
byte GF_add(byte f, byte g) {
    return f ^ g;
}

// GF(2^8) 뺄셈: a - b = a + (-b) = a + b => 덧셈
// GF(2^8) 나눗셈 : a/b = a * (b의 역원) = a * GF_inv(b)

// 거듭제곱  a^254 = a^(-1) : 곰셈의 역원 
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
//====================================

//유한체 원소를 갖는 행렬 구조체 GF_Matrix 정의
typedef struct {
    byte M[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE];
    int row;
    int col;
} GF_Matrix;


// 빈 초기화 (특별히 지정하지 않은 경우)
GF_Matrix GF_Mat_init() { // 오버로드(overload) 나중에 오버라이드(override)와 비교
    GF_Matrix result;
    result.row = 1;
    result.col = 1;
    result.M[0][0] = 0x00; 

    return result;
}

//배열로 행렬 구조체 초기화하기
GF_Matrix GF_Mat_init(byte A[][4], int row, int col) {   // 4x4 행렬을 기준으로 초기화하기
    GF_Matrix result;
    result.row = row;
    result.col = col;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            result.M[i][j] = A[i][j];
        }
    }
    return result;
}

// 행렬 출력하기
void GF_Mat_print(GF_Matrix Mat) {
    for (int i = 0; i < Mat.row; i++) {
        printf("[");
        for (int j = 0; j < Mat.col; j++) {
            printf(" %02x", Mat.M[i][j]);   
               
            if((j + 1) == (Mat.col / 2)){
                printf(" |");
            }        
        }
        printf(" ]\n");
    }
    printf("\n");
}

void GF_Mat_inv_print(GF_Matrix Mat) {
    for (int i = 0; i < Mat.row; i++) {
        printf("[");
        for (int j = 0; j < Mat.col; j++) {
            printf(" %02x", Mat.M[i][j]); 
                   
        }
        printf(" ]\n");
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

// 행렬 덧셈 A+B
GF_Matrix GF_Mat_Add(GF_Matrix A, GF_Matrix B) {
    GF_Matrix result;
    result = GF_Mat_init();
    if ((A.col != B.col) || (A.row != B.row)) {
        printf("Matrix size error!\n");
        return result;
    }

    result.row = A.row;
    result.col = A.col;
    for (int i = 0; i < result.row; i++) {
        for (int j = 0; j < result.col; j++) {
            result.M[i][j] = GF_add( A.M[i][j], B.M[i][j]);
        }
    }
    return result;
}

// 행렬 상수배 kA
GF_Matrix GF_Mat_scalar(GF_Matrix A, byte k) {
    GF_Matrix kA;
    kA.row = A.row;
    kA.col = A.col;
    for (int i = 0; i < kA.row; i++) {
        for (int j = 0; j < kA.col; j++) {
            kA.M[i][j] = GF_mul(A.M[i][j], k);
        }
    }
    return kA;
}

// 역행렬 구하기에 필요한 연산들....
//==============================

//두 행을 바꾸기 (call by reference)
void GF_Mat_Exchange_Row(GF_Matrix& A, int row1, int row2) {
    byte temp;
    for (int j = 0; j < A.col; j++) {
        //A.M[row1][j] <--> A.M[row2][j]
        temp = A.M[row1][j];
        A.M[row1][j] = A.M[row2][j];
        A.M[row2][j] = temp;
    }
}

//한 행에 상수배 하기
void GF_Mat_scalar_Mul_Row(GF_Matrix& A, byte k, int row) {
    for (int j = 0; j < A.col; j++) {
        A.M[row][j] = GF_mul(A.M[row][j], k);
    }
}

//한 행의 상수배를 다른 행에 더하기
void GF_Mat_Row_Add(GF_Matrix& A, byte k, int row_src, int row_target) {
    byte temp;
    for (int j = 0; j < A.col; j++) {
        temp = GF_mul(A.M[row_src][j], k);
        A.M[row_target][j] = GF_add(A.M[row_target][j], temp);
    }
}


// [실습] 역행렬 구하기
// GF_Matrix GF_Mat_inverse(GF_Matrix A) 
// 실행렬의 역행력 구하는 함수 Matrix Mat_inverse(Matrix A)를 수정하여 완성하기


GF_Matrix GF_Mat_inverse(GF_Matrix A) {
    GF_Matrix InvA;
    byte temp;
    InvA = GF_Mat_init();

    // 행렬 A와 단위행렬 I를 붙여 행렬 AA를 만든다. AA = [A|I] --> [I | A^{-1}]
    GF_Matrix AA;
    AA.row = A.row;
    AA.col = A.col * 2;
    for (int i = 0; i < A.row; i++) {
        for (int j = 0; j < A.col; j++) {
            AA.M[i][j] = A.M[i][j];
            AA.M[i][A.col + j] = (i == j) ? 0x01 : 0x00;    //바이트 연산 강조하기 위해(컴파일러는 영향 X)
        }
    }

    // Reduced-REF(Row Echelon Form) 사다리꼴 만들기
    int pivot_row;
    for (int j = 0; j < A.col; j++) { // 한 열씩 완성하기
        pivot_row = -1; // 초기에 -1로 설정하고, 0이 아닌 값이 나오는 행으로 지정
        for (int i = j; i < A.row; i++) {
            if (AA.M[i][j] != 0x00) { // 0이 아닌 행을 찾았으면,
                pivot_row = i;
            }
        }

        if (pivot_row == -1) { // 해당 열의 모든 원소가 0인 경우
            printf("A is not invertible.\n");
            return InvA; //의미 없는 결과 리턴
        }
        if (pivot_row != j) { // pivot_row와 바꾼다
            GF_Mat_Exchange_Row(AA, j, pivot_row);
        }
        temp = GF_inv(AA.M[j][j]);
        GF_Mat_scalar_Mul_Row(AA, temp, j);
        for (int i = 0; i < AA.row; i++) {
            if (i != j) {
                GF_Mat_Row_Add(AA, AA.M[i][j], j, i);   //같은 값 두개 더하면 0

            }
        }

        GF_Mat_print(AA);

    }

    InvA.row = A.row;
    InvA.col = A.col;
    for (int i = 0; i < A.row; i++) {
        for (int j = 0; j < A.col; j++) {
            InvA.M[i][j] = AA.M[i][A.col + j];  // [I|InvA]
        }
    }

    return InvA;
}


//==============================


// 역행렬 함수의 동작 확인하기
void GF_Matrix_inverse_test() {
    GF_Matrix MA, MInvA;
    byte arrA[4][4] = { {0x02, 0x03, 0x01, 0x01}, {0x01, 0x02, 0x03, 0x01}, {0x01, 0x01, 0x02, 0x03}, {0x03, 0x01, 0x01, 0x02} };
    MA = GF_Mat_init(arrA, 4, 4);
    GF_Mat_print(MA);

    MInvA = GF_Mat_inverse(MA);

    GF_Mat_inv_print(MInvA);
}


void GF_Matrix_practice() {
    GF_Matrix MA, MB, MC;
    byte arrA[4][4] = { {0x02, 0x03, 0x01, 0x01}, {0x01, 0x02, 0x03, 0x01}, {0x01, 0x01, 0x02, 0x03}, {0x03, 0x01, 0x01, 0x02} };
    MA = GF_Mat_init(arrA, 4, 4);
    GF_Mat_print(MA);
    MB = GF_Mat_Mul(MA, MA);
    GF_Mat_print(MB);
    MC = GF_Mat_Mul(MA, MB);
    GF_Mat_print(MC);
}


int main()
{
    //GF_Matrix_practice();
    GF_Matrix_inverse_test();
}
