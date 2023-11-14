#pragma once

typedef unsigned char byte;  // byte¶ó´Â µ¥ÀÌÅÍ Å¸ÀÔ ¸¸µé±â GF(2^8)

// GF(2^8) xtime    ´ÙÇ×½Ä¿¡ x °öÇÏ±â
byte xtime(byte a);

// GF(2^8) °ö¼À: h(x) = g(x)f(x) mod m(x)
byte GF_mul(byte f, byte g);

// GF(2^8) µ¡¼À: h(x) = g(x) + f(x) mod m(x)
byte GF_add(byte f, byte g);

// °ÅµìÁ¦°ö  a^254 = a^(-1) : °õ¼ÀÀÇ ¿ª¿ø 
byte GF_inv(byte a);