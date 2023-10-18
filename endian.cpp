#include <stdio.h>

typedef unsigned char byte;
typedef unsigned int u32;
//Endian 문제 = little endian, big endian (HW 구분 -> SW 반영)

//출처 : rijndael-alg-fst.h
//pt의 위치부터 4바이트 읽어오기 
//{pt[0], pt[1], pt[2], pt[3]} --> u32 x = pt[0]pt[1]pt[2]pt[3]
//함수처럼 pt를 받아서 extension (함수아님 매크로) 매크로는 컴파일러 단계에서 실행되므로 함수처럼 기능하지못함
#define GETU32(pt)  ((u32)((pt)[0] << 24)) ^  \
 ((u32)((pt)[1] << 16)) ^ ((u32)((pt)[2] << 8)) \
^ ((u32)((pt)[3]));

//byte ct[4];   u32 st;
#define PUTU32(ct, st)  {\
    (ct)[0] = (byte)((st) >> 24); \
    (ct)[1] = (byte)((st) >> 16); \
    (ct)[2] = (byte)((st) >> 8); \
    (ct)[3] = (byte)((st)); \
}

void endian_test1(){
    byte b[4] = { 0, 1, 2, 3};
    u32 x;   //4바이트 변수
    u32* px; //u32 변수를 가리키는 포인터 (4바이트 포인터)

    px = (u32*)b; //배열의 이름은 첫 원소를 가리키는 포인터와 같다
    px = (u32*)&b[0];       //(u32*) type casting 강제 형 변환
    x = *px;    //px 포인터가 가리키는 곳의 값 (u32)

    u32 y;
    y = ((u32)(b[0] << 24)) ^ ((u32)(b[1] << 16)) ^ ((u32)(b[2] << 8)) ^ ((u32)(b[3]));

    u32 z;
    z = GETU32(b);  //매크로를 이용한 big endian 표현법
    
    printf("B = { %02x, %02x, %02x, %02x}\n",
            b[0], b[1], b[2], b[3]);           //00, 01, 02, 03
    printf(" x = %08x\n", x);     //0x00010203(big) or 0x03020100(little)
    printf(" y = %08x\n", y);
    printf(" z = %08x\n", z);
}


void endian_test2(){        //big endian 으로 나오게 수동으로 shift 해보자!
    u32 x = 0x00010203;
    byte b[4];      //메모리만 확보     b[4] ={00, 01, 02, 03} 으로 만들고 싶다...
    //byte *b;        //포인터만 선언

    b[0] = (byte)(x >> 24);     //3byte shift 후 00만 남기고 byte자료형으로 b[0]에 저장
    b[1] = (byte)(x >> 16);     // (x>>16) & 0xff = 0001 & 0x00ff 하지만 byte 자료형으로 고치면 앞의 과정없이 맨 오른쪽 8비트만 가져옴
    b[2] = (byte)(x >> 8);
    b[3] = (byte) x;

    printf("B = { %02x, %02x, %02x, %02x}\n",
            b[0], b[1], b[2], b[3]);           //00, 01, 02, 03
    printf(" x = %08x\n", x);

}

void endian_test3(){
    byte b[16];
    u32 x0, x1, x2, x3;
    
    for(int i = 0; i < 16; i++) b[i] = (byte)i;
    x0 = GETU32(b);      //b[0], b[1], b[2], b[3]
    //x1 = GETU32(&b[4]);       //b[4], b[5], b[6], b[7]
    x1 = GETU32(b + 4); // +는 포인터에 대한 연산 +4마다 4바이트씩
    x2 = GETU32(b + 8);
    x3 = GETU32(b + 12);

    u32 y[4];   //y[0] = b[0]b[1]b[2]b[3]
    for(int i = 0; i < 4; i++){
        y[i] = GETU32(b + i*4);
    }
    printf("y[4] = ");
    for (int i = 0; i < 4; i++) printf("%08x", y[i]);

    printf("\nb = {");
    for(int i = 0; i < 16; i++) printf("%02x", b[i]);
    printf("} \n");
    printf("x0 = %08x\n", x0);
    printf("x1 = %08x\n", x1);
    printf("x2 = %08x\n", x2);
    printf("x3 = %08x\n", x3);
    
    u32 z[4];
    z[0] = 0x00010203;
    z[1] = 0x04050607;
    z[2] = 0x08090a0b;
    z[3] = 0x0c0d0e0f;

    byte s[16];

    PUTU32(s, z[0]);
    PUTU32(s + 4, z[1]);
    PUTU32(s + 8, z[2]);
    PUTU32(s + 12, z[3]);

    printf("s = {");
    for (int i = 0; i < 16; i++) printf("%02x", s[i]);
    printf(" }\n");
}

int main(){
    //endian_test1();
    //endian_test2();
    endian_test3();
}