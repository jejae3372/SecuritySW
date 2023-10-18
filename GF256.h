
typedef unsigned char byte;

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
