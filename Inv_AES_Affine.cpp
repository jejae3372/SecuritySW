//GF(2)유한체에서의 A^(-1) 역행렬 구하기

#include <stdio.h>

typedef int byte;
#define MATRIX_MAX_SIZE 20

typedef struct{
    byte M[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE];
    int row;
    int col;
}Matrix;

//유한체 GF(2) 연산 모음
//======================================


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


//======================================
//배열로 행렬 구조체 초기화하기
Matrix Mat_init(byte A[][8], int row, int col){       //8X8 행렬을 기준으로 초기화 하기
    Matrix result;
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
Matrix Mat_init(){      //오버로드 된 상태. 파라미터에 의해서 동일한 이름의 함수들 중 무엇을 사용할지 결정됨
    Matrix result;
    result.row = 1;
    result.col = 1;
    result.M[0][0] = 0;

    return result;
}

//행렬 출력하기
void Mat_print(Matrix Mat){
    for(int i = 0; i < Mat.row; i++){
        printf("[");
        for(int j = 0; j < Mat.col; j++){
            printf(" %d", Mat.M[i][j]);        //GF(2)의 원소이므로 0 또는 1의 정수만 출력
        }
        printf("]\n");
    }
    printf("\n");
}

void Mat_Inv_print(Matrix Mat){
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


//역행렬 구하기에 필요한 연산들...
//==================================

//두 행을 바꾸기 (call by reference)
void Mat_Exchange_Row(Matrix &A, int row1, int row2){
    byte temp;
    for(int j = 0; j < A.col; j++){
        temp = A.M[row1][j];
        A.M[row1][j] = A.M[row2][j];
        A.M[row2][j] = temp;
    }
}

//한 행의 상수배를 다른 행에 더하기
void Mat_Row_Add(Matrix &A, int row_src, int row_target){
    for(int j = 0; j < A.col; j++){
        A.M[row_target][j] = GF_add(A.M[row_target][j], A.M[row_src][j]);
    }
}

//역행렬 구하기
Matrix Mat_inverse(Matrix A){
    Matrix InvA;
    InvA = Mat_init();

    //행렬 A와 단위행렬 I 를 붙여 행렬 AA를 만든다. AA = [A][I]
    Matrix AA;
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
    Mat_Inv_print(AA);
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
            if((i != j) && (AA.M[i][j] == 1)){      //자기자신의 행은 건들이지 않으며 0이 아닌 행 + pivot_row 
                Mat_Row_Add(AA, j, i);
            }
        }     
        if(pivot_row == -1){    //해당 열의 모든 원소가 0인 경우 역행렬을 구할 수 없다.
                printf("A is not invertable.\n");
                printf("%d", j);
                return InvA;    //의미없는 리턴
        }

        Mat_Inv_print(AA);
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

void Matrix_inverse_test(){
    Matrix MA, MInvA;
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
    MA = Mat_init(arrA, 8, 8);
    Mat_print(MA);

    MInvA = Mat_inverse(MA);
    Mat_print(MInvA);
}

int main(){
    Matrix_inverse_test();
}