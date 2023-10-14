#include <stdio.h>

typedef unsigned char byte;


//GF(2^8) xtime = 한 줄 버전 채우기
byte GF_xtime(byte f){
    return (((f >> 7) & 0x01) == 1 ? (f << 1) ^ 0x1b : f << 1);
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


// 3 - (a) 문제
void order_print(){ 
    int ord;
    int sum;
    byte temp_x;
    sum = 0;
    for(int x = 1; x < 256; x ++){ 
        temp_x = 0x01;         // x^0 = 1
        for(int i = 1; i < 256; i ++){
            temp_x = GF_mul(temp_x, x);
            if(temp_x == 0x01){
                ord = i;
                break;
            }
        }
        printf("\n0x%02x's order is %d\n", x, ord); 
        //위수가 255인 x는 generater
        if(ord == 255){
            sum += 1;
        }

    }
    printf("\n generator는 %d개 이다.", sum);
}

//3 - (b) 문제
void gen_graph(){       
    byte temp_x;
    temp_x = 0x01;
    printf("x = 0x03 \n");
    printf("0x%02x ", temp_x);
    for(int x = 1; x < 16; x ++){  //0x03^(0) ~ 0x03^(15) 출력
        temp_x = GF_mul(temp_x, 0x03);
        printf("0x%02x ", temp_x); 
    }
    printf("\n");
    for(int x = 1; x < 240; x ++){ //0x03^(16) ~ 0x03^(254)
        temp_x = GF_mul(temp_x, 0x03);
        printf("0x%02x ", temp_x);        
        if( x % 16 == 0){
        printf("\n");
        }  
        if(temp_x == 0x01){
            break; 
        }      
    }

    printf("\nx = 0x05 \n");
    temp_x = 0x01;
    printf("0x%02x ", temp_x);
    for(int x = 1; x < 16; x ++){  //0x05^(0) ~ 0x05^(15) 출력
        temp_x = GF_mul(temp_x, 0x05);
        printf("0x%02x ", temp_x);
        
    }
    printf("\n");
    for(int x = 1; x < 240; x ++){ //0x05^(16) ~ 0x05^(254)
        temp_x = GF_mul(temp_x, 0x05);
        printf("0x%02x ", temp_x);        
        if( x % 16 == 0){
        printf("\n");
        }  
        if(temp_x == 0x01){
            break; 
        }      
    }
    
}

//3 - (d) 문제

//ExpTable_generator(0x03)  
void ExpTable(byte ExpT[255]){  //index) 0 ~ 254 -> k) 1 ~ 255
    byte temp_x;                        //ExpTable[0] = 0x03, [255] = 0x01
    temp_x = 0x01;         // x^0 = 1
    for(int x = 0; x < 255; x ++){ 
        temp_x = GF_mul(temp_x, 0x03);
        ExpT[x] = temp_x; 
    }
  

}
//LogTable_generator(0x03)
void LogTable(byte LogT[256]){      
    byte temp_x;
    temp_x = 0x01;
    LogT[0] = 0x00; //0의 자리는 필요없으므로 사용 X 
    for(int x = 1; x < 256; x ++){ 
        temp_x = GF_mul(temp_x, 0x03);
        LogT[temp_x] = x;           //LogT[0x03] = 1, LogT[0x05] = 2,
    }
    
}

void Table_Test(){      //Table 출력
    byte ExpT[255], LogT[256];
    ExpTable(ExpT);
    /*printf("\nExpTable = \n[ ");
    for(int i = 0; i < 255; i ++){
        printf("%02x ", ExpT[i]);
        if(i % 16 == 15){
            printf("\n");
        }
    }
    printf("]\n");  
    printf("\nLogTable = \n [");    
    */
    LogTable(LogT);
    /*for(int i = 1; i < 256; i ++){
        printf("%02x ", LogT[i]);
        if((i % 16 == 0)&&(i != 0)){
            printf("\n");
        }   
    }
    printf("]\n"); 
    */
}

//3 - (e) 문제
//a^(x) * a^(y) = a^(x + y)
//xy? 
//log(x) ^ log(y) = log(xy) -> exp(log(xy)) = xy
byte GF_mul_Exp(byte x, byte y){
    byte ExpT[255], LogT[256];
    ExpTable(ExpT);
    LogTable(LogT);
    byte xy, xy_mul;
    xy = LogT[x] + LogT[y];

    xy_mul = ExpT[xy - 1];  //ExpT 는 index 0 -> a^(1)
    
    return xy_mul;
}

int main(){
    byte ExpT[255], LogT[256];
    byte GF_mul_result, GF_mul_co;
    //order_print();
    //gen_graph();
    Table_Test();
    GF_mul_result = GF_mul_Exp(0x05, 0x05);
    printf("%02x\n", GF_mul_result);
        //print ExpTable
    GF_mul_co = GF_mul(0x05, 0x05);
    printf("%02x", GF_mul_co);      //GF_mul의 결과와 일치함.
}
 
