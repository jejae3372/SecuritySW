#pragma once

typedef unsigned char byte;  // byte��� ������ Ÿ�� ����� GF(2^8)

// GF(2^8) xtime    ���׽Ŀ� x ���ϱ�
byte xtime(byte a);

// GF(2^8) ����: h(x) = g(x)f(x) mod m(x)
byte GF_mul(byte f, byte g);

// GF(2^8) ����: h(x) = g(x) + f(x) mod m(x)
byte GF_add(byte f, byte g);

// �ŵ�����  a^254 = a^(-1) : ������ ���� 
byte GF_inv(byte a);