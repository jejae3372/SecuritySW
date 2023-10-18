//함수호출 연습

#include <stdio.h>

void pointer1(){
    printf("== pointer practice 1 == \n");

    int a;          //stack에 저장되는 변수
    int* pa;        //정수형 타입의 포인터 변수

    a = 5;
    pa = &a;        //변수 a의 주소

    printf("a=%d, &a=%p\n", a, &a);
    printf("pa=%p, *pa=%d, &pa=%p\n", pa, *pa, &pa);
}

void swap1(int a, int b){   //일반적인 swap 방식
    int temp;

    temp = a;
    a = b;
    b = temp;
}

void swap2(int* pa, int* pb){   //fun-call 할 때, stack에 a, b의 주소
    int temp;
    printf("a=%d, b=%d \n", *pa, *pb);
    temp = *pa;     //temp = a
    *pa = *pb;      //a = b         
    *pb = temp;
    printf("a=%d, b=%d \n", *pa, *pb);

}

void swap3(int& a, int& b){
    int temp;

    printf("a=%d, b=%d \n", a, b);
    temp = a;
    a = b;
    b = temp;
    printf("a=%d, b=%d \n", a, b);
}

void swap_practice(){
    int x = 1;
    int y = 2;
    
    printf("x = %d, y = %d \n", x, y);
    //swap1(x, y);
    //swap2(&x, &y);
    swap3(x, y);
    printf("x = %d, y = %d \n", x, y);

}

int main(){
    pointer1();
    swap_practice();

}