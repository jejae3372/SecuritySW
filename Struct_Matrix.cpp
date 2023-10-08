/*
구조체로 행렬 정의하기
행렬 연산
역행렬 구하기

유한체 행렬 정의하기
유한체 행렬 연산
유한체 역행렬 구하기 --> InvMixCol()연산

*/

#include <stdio.h>

typedef unsigned char type;
#define MATRIX_MAX_SIZE 20
#define NEARLY_ZERO 1e-10 // 10^(-10) 보다 절대값이 작으면 0으로 간주한다.

typedef struct{
    double M[MATRIX_MAX_SIZE][MATRIX_MAX_SIZE];
    int row;
    int col;
}Matrix;

//배열로 행렬 구조체 초기화하기
Matrix Mat_init(double A[][3], int row, int col){       //4x4 행렬을 기준으로 초기화 하기
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
    result.M[0][0] = 0.0;

    return result;
}

//행렬 출력하기
void Mat_print(Matrix Mat){
    for(int i = 0; i < Mat.row; i++){
        printf("[");
        for(int j = 0; j < Mat.col; j++){
            if((Mat.M[i][j] < NEARLY_ZERO) && (Mat.M[i][j] > -NEARLY_ZERO)){
                printf("%7.2f", 0.0);
            }
            else{
                printf("%7.2f", Mat.M[i][j]);
            }
        }
        printf("]\n");
    }
    printf("\n");
}

void Mat_Inv_print(Matrix Mat){
    for(int i = 0; i < Mat.row; i++){
        printf("[");
        for(int j = 0; j < Mat.col; j++){
            if((Mat.M[i][j] < NEARLY_ZERO) && (Mat.M[i][j] > -NEARLY_ZERO)){
                printf("%7.2f", 0.0);
            }
            else{
                printf("%7.2f", Mat.M[i][j]);
            }
            if((j + 1) == (Mat.col / 2)){
                printf(" |");
            }
        }
        printf("]\n");
    }
    printf("\n");
}

//행렬 곱
Matrix Mat_Mul(Matrix A, Matrix B){
    Matrix AB;
    AB = Mat_init();
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
Matrix Mat_Add(Matrix A, Matrix B){
    Matrix result;
    result = Mat_init();
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

//행렬 상수배
Matrix Mat_scalar(Matrix A, double k){
    Matrix kA;
    kA.row = A.row;
    kA.col = A.col;
    for(int i = 0; i < kA.row; i++){
        for(int j = 0; j< kA.col; j++){
            kA.M[i][j] = k*A.M[i][j];
        }
    }
    return kA;
}

//역행렬 구하기에 필요한 연산들...
//==================================

//두 행을 바꾸기 (call by reference)
void Mat_Exchange_Row(Matrix &A, int row1, int row2){
    double temp;
    for(int j = 0; j < A.col; j++){
        temp = A.M[row1][j];
        A.M[row1][j] = A.M[row2][j];
        A.M[row2][j] = temp;
    }
}

//한 행에 상수배 하기
void Mat_Scalar_Mul_Row(Matrix &A, double k, int row){
    for(int j = 0; j < A.col; j++){
        A.M[row][j] *= k;
    }
}

//한 행의 상수배를 다른 행에 더하기
void Mat_Row_Add(Matrix &A, double k, int row_src, int row_target){
    for(int j = 0; j < A.col; j++){
        A.M[row_target][j] += k*A.M[row_src][j];
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
            AA.M[i][A.col + j] = (i==j)? 1.0: 0.0;  //A.col + j번째 열 부터 단위 행렬 만들기
        }
    }
    //Reduced REF(Row Echelm Form) 사다리꼴 만들기
    int pivot_row;
    for(int j = 0; j < A.col; j++){
        pivot_row = -1; //초기에 -1로 설정하고, 0이 아닌 값이 나오는 열로 지정
        for(int i = j; i < A.row; i++){
            if((AA.M[i][j]) > NEARLY_ZERO || (-AA.M[i][j]) > NEARLY_ZERO){      //절댓값이 NEARLY_ZERO(0으로 간주) 를 포함한 0이 아니면.
                pivot_row = i;
            }
        }

        if(pivot_row == -1){    //해당 열의 모든 원소가 0인 경우 역행렬을 구할 수 없다.
            printf("A is not invertable.\n");   
            return InvA;    //의미없는 리턴
        }
        if(pivot_row != j){             //해당 row에 0이 아닌 값이 아니라면, 행을 pivot_row 와 바꾼다
            Mat_Exchange_Row(AA, j, pivot_row);
        }
        Mat_Scalar_Mul_Row(AA, 1. / AA.M[j][j], j);     //해당 행에 1/pivot을 곱해준다 전부 (pivot을 1로 만들어준다)
        for(int i = 0; i< AA.row; i++){
            if((i != j) && (((AA.M[i][j]) > NEARLY_ZERO || (-AA.M[i][j]) > NEARLY_ZERO))){      //자기자신의 행은 건들이지 않으며 0이 아닌 행
                Mat_Row_Add(AA, -AA.M[i][j], j, i);
            }
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
/*
void Matrix_practice(){
    Matrix MA, MB;
    double arrA[4][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}};
    MA = Mat_init(arrA, 4, 4);
    Mat_print(MA);
    //MB = Mat_Mul(MA, MA);
    //Mat_print(MB);

    Mat_Exchange_Row(MA, 0, 1);
    Mat_print(MA);
}
*/
void Matrix_inverse_test(){
    Matrix MA, MInvA;
    double arrA[3][3] = {{0.0, -0.25, 0.5}, {0.5, -0.25, 0.0}, {-0.25, 0.5, -0.25}};
    MA = Mat_init(arrA, 3, 3);
    Mat_print(MA);

    MInvA = Mat_inverse(MA);
    Mat_print(MInvA);
}


int main(){
    Matrix_inverse_test();
    //Matrix_practice();
}