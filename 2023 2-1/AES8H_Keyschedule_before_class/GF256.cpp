#include "GF256.h"


// GF(2^8) xtime    ���׽Ŀ� x ���ϱ�
byte xtime(byte a) {
    int msb; //�ְ������� ��� (a7)
    byte result;
    msb = (a >> 7) & 0x01;  // 0x01 = 0000 0001 (��Ʈ ����ũ)
    if (msb == 1) { //a7=1
        result = (a << 1) ^ 0x1b;  // a(x)*x + (x^4+x^3+x+1)
    }
    else { //a7=0
        result = a << 1;
    }
    return result;
}

// GF(2^8) ����: h(x) = g(x)f(x) mod m(x)
byte GF_mul(byte f, byte g) {
    byte h; // �������� h = g*f
    int coef; //  ���� ����� ���
    h = 0x00; // �ʱⰪ h(x) = 0
    for (int i = 7; i >= 0; i--) {  // 7������ ����ױ���
        coef = (f >> i) & 0x01; // a7, a6, a5, .. ,a0
        h = xtime(h);
        if (coef == 1) {  //����� 1�̸� g(x)�� ���ϱ�, 0�̸� �״�� �α�
            h ^= g; //h = GF_add(h, g);
        }
    }
    return h;
}

// GF(2^8) ����: h(x) = g(x) + f(x) mod m(x)
byte GF_add(byte f, byte g) {
    return f ^ g;
}

// �ŵ�����  a^254 = a^(-1) : ������ ���� 
byte GF_inv(byte a) {  // ������ ����
    byte result;  // �����
    byte temp;    // �߰���� a^2, a^4, a^8, a^16, ... , a^128
    result = 1;
    temp = a;
    for (int i = 0; i < 7; i++) {
        temp = GF_mul(temp, temp); // a^2, a^4, a^8, ... , a^128
        result = GF_mul(result, temp);
    }
    return result;
}