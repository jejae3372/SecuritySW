// 파일 암호화: AES-ECB 모드


#include <iostream>
#include <fstream>
using namespace std;

#include "AES32.cpp"

//state[] xor value[] --> state[]  (업데이트)
void Xor_state(byte state[16], byte value[16]){
    for(int i = 0; i < 16; i++){
        state[i] ^= value[i];
    }
}
//state[] 복사하기
void Copy_state(byte src[16], byte dest[16]){
    for(int i = 0; i < 16; i ++){
        dest[i] = src[i];
    }
}

//AES_CRT는 별도의 암복호화 함수를 구현 X -> 암복호화가 양쪽으로 동일한 과정
void AES_CRT(const char* filePT, byte key[16], byte IV[16], const char* fileCT) {
    ifstream fin;
    ofstream fout;
    char ch;

    fin.open(filePT, ios::binary); // 입력 파일을 연다
    if (fin.fail()) {
        cout << "Input file Open Error!" << endl;
        return;
    }
    
    // 파일 크기를 저장한다
    int file_len;
    fin.seekg(0, fin.end);
    file_len = fin.tellg();
    cout << "file size(plaintext) = " << file_len << "bytes" << endl;
    fin.seekg(0, fin.beg);

    fout.open(fileCT, ios::binary); // 출력 파일을 만든다
    if (fout.fail()) {
        cout << "Output file Open Error!" << endl;
        return;
    }

    int num_block, remainder;
    num_block = file_len / 16 ; //나누어 떨어지면 한블록 더
    remainder = file_len - 16 * (num_block); // 마지막 블록에 포함될 평문 바이트(0~15)

    cout << "file size(ciphertext) = " << file_len << "bytes" << endl;

    //AES32 키스케줄
    u32 rk[11][4];
    AES32_Enc_KeySchedule(key, rk);

    //AES32-CTR 암복호화
    
    byte buffer[16];  // 평문을 읽어오는 변수
    byte ct[16];      // 암호문을 저장할 변수
    //(1) Iv -> AES_Enc(Iv)
    //(2) xor PT -> CT
    byte iv_enc[16], iv[16]; //iv(CTR) 을 암호화 한 값, IV를 복사할 값
    Copy_state(IV, iv);
    for (int i = 0; i < num_block; i++) {
        fin.read((char*)buffer, 16); //buffer 에 평문 16byte 저장
        AES32_Encrypt(iv, rk, iv_enc); // iv -> AES 암호화
        Xor_state(iv_enc, buffer); // PT xor AES(Iv)
        fout.write((char*)iv_enc, 16); //마지막 블록 전까지 CT
        for (int j = 15; j > 0; j--){
            if (iv[j] != 0x0f){
                iv[j] ^= 0x01;
                break;
            }else{
                iv[j-1] ^= 0x01;
                break;
            }
        }
    }
    //마지막블록은 평문의 길이만큼 까지만 xor 연산
    AES32_Encrypt(iv, rk, iv_enc);
    fin.read((char*)buffer, remainder);
    Xor_state(iv_enc, buffer); // PT xor AES(Iv)
    fout.write((char*)iv_enc, remainder);
    fin.close();
    fout.close();
}
 

void File_CBC_test() {
    const char* pPT = "PT.bin";
    const char* pCT = "CT.bin";
    const char* pDecPT = "DecPT.bin";

    byte key[16], IV[16];
    for (int i = 0; i < 16; i++) {
        key[i] = i;
        IV[i] = i;
    }

    // ECB : Electronic CodeBook  --> 좋지 않은 운영모드
    
    cout << "AES ECB Encrypt..." << endl;
    AES_CRT(pPT, key, IV, pCT);

    cout << "AES ECB Decrypt..." << endl;
    AES_CRT(pCT, key, IV, pDecPT);

}


int main()
{
    File_CBC_test(); //ECB 암호화/복호화 테스트
}
