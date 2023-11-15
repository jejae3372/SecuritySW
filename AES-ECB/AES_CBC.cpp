// 파일 암호화: AES-ECB 모드


#include <iostream>
#include <fstream>
using namespace std;

//AES32 구현을 포함시킨다 (7주차에 업로드된 파일)
#include "AES32.h"


//=[11주차 코드에 정의된 함수]=======================
// in[] : 16바이트 이하의 바이트열
// out[16] : 16바이트 (0x80 패딩) 0x80 = 1000 0000
// in_length : { 0, 1, 2, ... , 15}
void padding(byte in[], int in_length, byte out[16]) {
    byte pad_byte = 0x80;
    for (int i = 0; i < in_length; i++) {
        out[i] = in[i];
    }
    out[in_length] = 0x80;
    for (int i = in_length + 1; i < 16; i++) {
        out[i] = 0x00;
    }
}

//=[패딩을 제외한 데이터 길이(바이트)]=======================
// 패딩을 제외한 바이트 수 구하기
// 출력값: 0, 1, 2, ... , 15, (-1: 오류)
int pt_length(byte padded[16]) {
    int position80;
    position80 = 15;
    for (int i = 15; i > 0; i--) {
        if (padded[i] != 0x00) break;
        position80--;
    }
    if (padded[position80] != 0x80) {
        cout << "Padding error: 0x80 not found." << endl;
        // (1) 0x00이 끝나는 지점에서 0x80이 없을 때,
        // (2) 0x00이 계속 나올 때
        return -1; // (1) or (2)
    }
    else {
        return position80;
    }
}

//==[CBC용 함수추가]=========
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

//=[11주차 코드 padding_test() 함수를 수정하여 완성함]=================
//void padding_test() {
void AES_Enc_CBC(const char* filePT, byte key[16], byte IV[16], const char* fileCT) {
    ifstream fin;
    ofstream fout;
    char ch;

    fin.open(filePT, ios::binary); // 입력 평문파일을 연다
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

    fout.open(fileCT, ios::binary); // 출력 암호문파일을 만든다
    if (fout.fail()) {
        cout << "Output file Open Error!" << endl;
        return;
    }

    int num_block, remainder;
    num_block = file_len / 16 + 1; //나누어 떨어지면 한블록 더
    remainder = file_len - 16 * (num_block - 1); // 마지막 블록에 포함될 평문 바이트(0~15)

    cout << "file size(ciphertext) = " << num_block * 16 << "bytes" << endl;

    //AES32 키스케줄
    u32 rk[11][4];
    AES32_Enc_KeySchedule(key, rk);

    //AES32-CBC 암호화
    
    byte buffer[16];  // 평문을 읽어오는 변수
    byte ct[16];      // 암호문을 저장할 변수
    //(1) pt xor prev_iv ---> [AES] --> ct, 
    //(2) ct ---> prev_iv
    byte prev_iv[16]; //iv를 저장할 변수
    Copy_state(IV, prev_iv); //시작할때 받은 IV를 prev_iv의 초기값에 사용
    for (int i = 0; i < num_block - 1; i++) { // 마지막 블록 직전까지 
        fin.read((char*)buffer, 16);
        Xor_state(buffer, prev_iv); //pt xor prev_iv
        AES32_Encrypt(buffer, rk, ct); // AES 암호화
        fout.write((char*)ct, 16);
        Copy_state(ct, prev_iv);  //ct ---> prev_iv
    }

    byte last_in[16], last_block[16];
    for (int i = 0; i < remainder; i++) { // 마지막 블록 패딩 & 암호화
        fin.read((char*)&ch, 1);
        last_in[i] = ch;
    }
    padding(last_in, remainder, last_block);
    Xor_state(last_block, prev_iv);
    AES32_Encrypt(last_block, rk, ct); // 마지막 블록의 AES 암호화

    fout.write((char*)ct, 16);

    fin.close();
    fout.close();

}

//=[AES_Enc_CBC() 함수를 수정하여 완성] ===================
void AES_Dec_CBC(const char* fileCT, byte key[16], byte IV[16], const char* filePT) {
    ifstream fin;
    ofstream fout;
    char ch;

    fin.open(fileCT, ios::binary); // 입력 암호문파일을 연다
    if (fin.fail()) {
        cout << "Input file Open Error!" << endl;
        return;
    }

    fout.open(filePT, ios::binary); // 출력 평문파일을 만든다
    if (fout.fail()) {
        cout << "Output file Open Error!" << endl;
        return;
    }

    // 파일 크기를 저장한다
    int file_len;
    fin.seekg(0, fin.end);
    file_len = fin.tellg();
    cout << "file size(ciphertext) = " << file_len << "bytes" << endl;
    fin.seekg(0, fin.beg);

    int num_block, remainder;
    num_block = file_len / 16; //암호문 크기는 16의 배수 바이트
    if ((file_len % 16) != 0) { //파일의 크기가 16의 배수가 아니면 오류
        cout << "File size Error (Not a multiple of 16)" << endl;
        return;
    }

    //AES32 키스케줄
    u32 rk[11][4];
    AES32_Dec_KeySchedule(key, rk);

    //AES32-CBC 복호화

    byte buffer[16];  // 암호문을 읽어오는 변수
    byte pt[16];      // 복호한 평문을 저장할 변수
    byte prev_iv[16];
    //(1) buffer(ct) --> [AES-Decrypt] --> pt --> pt xor prev_iv
    //(2) buffer(ct) --> prev_iv;
    Copy_state(IV, prev_iv);
    for (int i = 0; i < num_block - 1; i++) { // 마지막 블록 직전까지 
        fin.read((char*)buffer, 16);
        AES32_EqDecrypt(buffer, rk, pt); // AES 복호화
        Xor_state(pt, prev_iv); //(1) pt --> pt xor prev_iv
        fout.write((char*)pt, 16);
        Copy_state(buffer, prev_iv);    //(2) buffer(ct) --> prev_iv;
    }

    //마지막 블록 복호화
    fin.read((char*)buffer, 16);
    AES32_EqDecrypt(buffer, rk, pt); // AES 복호화
    Xor_state(pt, prev_iv);

    int last_pt_len;
    last_pt_len = pt_length(pt); //패딩을 제외한 평문의 길이(0~15)를 계산
    if (last_pt_len < 0) {
        cout << "Padding error." << endl;
        return;
    }
    fout.write((char*)pt, last_pt_len); //평문길이만큼만 파일에 저장

    cout << "file size(plaintext) = " << (num_block-1) * 16 + last_pt_len << "bytes" << endl;

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
    AES_Enc_CBC(pPT, key, IV, pCT);

    cout << "AES ECB Decrypt..." << endl;
    AES_Dec_CBC(pCT, key, IV, pDecPT);

}


int main()
{
    File_CBC_test(); //ECB 암호화/복호화 테스트
}
