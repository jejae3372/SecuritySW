// 파일 암호화: AES-CBC-CTS 모드


#include <iostream>
#include <fstream>
using namespace std;

//AES32 구현을 포함시킨다 (mac vscode로 실행하면서 헤더파일의 인식 오류가 있어서 cpp파일을 include했습니다.)
#include "AES32.cpp"

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

//CBC-CTS Encryption
void AES_Enc_CTS(const char* filePT, byte key[16], byte IV[16], const char* fileCT) {
    ifstream fin;
    ofstream fout;

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
    num_block = file_len / 16; //블록이 다 차있는 n-1까지의 블록수
    remainder = file_len - 16 * (num_block); // 마지막 블록에 포함될 평문 바이트(0~15)

    cout << "file size(ciphertext) = " << (num_block) * 16 + remainder << "bytes" << endl;

    //AES32 키스케줄
    u32 rk[11][4];
    AES32_Enc_KeySchedule(key, rk);

    //AES32-CBC-CTS 암호화
    
    byte buffer[16];  // 평문을 읽어오는 변수
    byte ct[16];      // 암호문을 저장할 변수
    //n-1단계의 암호문에서 마지막 크기만큼 떼어 낸 마지막 암호문. tail : 떼어내고 남은 부분
    byte head[remainder]; 

    byte prev_iv[16]; //iv를 저장할 변수
    Copy_state(IV, prev_iv); //시작할때 받은 IV를 prev_iv의 초기값에 사용
    for (int i = 0; i < num_block - 1; i++) { // 1 ~ n-2 까지 실행
        fin.read((char*)buffer, 16);
        Xor_state(buffer, prev_iv); //pt xor prev_iv
        AES32_Encrypt(buffer, rk, ct); // AES 암호화
        fout.write((char*)ct, 16);
        Copy_state(ct, prev_iv);  //ct ---> prev_iv
    }
    //n-1 단계 암호화 -> 마지막 암호문 head 저장
    fin.read((char*)buffer, 16);
    Xor_state(buffer, prev_iv); //pt xor prev_iv
    AES32_Encrypt(buffer, rk, ct); // AES 암호화
    for(int i = 0; i < 16; i++){
        head[i] = ct[i];
    }
//마지막 단계 (pt 가 16의 배수가 아니라면 -> 0으로 채워줌) -> 0으로 채우면 xor했을 때, 끝에 tail 이 남는 것과 같다. 
    byte last_in[16];
    fin.read((char*)&(last_in), 16);
    for (int k = remainder; k < 16; k++){
        last_in[k] = 0x00;
    }
    Xor_state(last_in, ct);

    AES32_Encrypt(last_in, rk, ct); // 마지막 블록의 AES 암호화

    fout.write((char*)ct, 16); // n-1 번째 출력
    fout.write((char*)head, remainder); // n 번째 출력

    fin.close();
    fout.close();

}

//CBC-CTS Decryption================================
void AES_Dec_CTS(const char* fileCT, byte key[16], byte IV[16], const char* filePT) {
    ifstream fin;
    ofstream fout;

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
    remainder = file_len - 16 * (num_block);
    //AES32 키스케줄
    u32 rk[11][4];
    AES32_Dec_KeySchedule(key, rk);


    //AES32-CBC-CTS 복호화
    byte buffer[16], last_in[remainder];  //last_in = 마지막 단계에 들어오는 remainder 크기의 블록
    byte pt[16], last_pt[remainder];      // 복호한 평문을 저장할 변수, last_pt = 마지막 평문
    byte prev_iv[16], tail[16 - remainder], prev_in[16];

    Copy_state(IV, prev_iv);
    for (int i = 0; i < num_block - 1; i++) { // 1 ~ n-2 단계 실행
        fin.read((char*)buffer, 16);
        AES32_EqDecrypt(buffer, rk, pt); // AES 복호화
        Xor_state(pt, prev_iv); //(1) pt --> pt xor prev_iv
        fout.write((char*)pt, 16);
        Copy_state(buffer, prev_iv);   //(2) buffer(ct) --> prev_iv;
    }
//n - 1 번째 입력 블록, n번째 입력 블록을 별도 저장 후 복호화한다.
    fin.read((char*)buffer, 16); // n - 1번째 입력
    fin.read((char*)last_in, remainder); // n 번째 입력
//n - 1번째 블록을 이용해 마지막 복호화 부터 진행한다.
    AES32_EqDecrypt(buffer, rk, pt); // AES 복호화
    for(int i = remainder; i < 16; i++){
        tail[i - remainder] = pt[i]; //n-1 번째에 넘져줄 tail
    }
    // n -  1 번째 입력할 prev_in (n번째 입력 + tail) 완성
    for(int p = 0; p < remainder; p++){
        prev_in[p] = last_in[p];
    }
    for(int q = remainder; q < 16; q++){
        prev_in[q] = tail[q - remainder];
    }       
    
    Xor_state(pt, prev_in);
    Copy_state(pt, last_pt);
    AES32_EqDecrypt(prev_in, rk, pt);
    Xor_state(pt, prev_iv);
    fout.write((char*)pt, 16);
    fout.write((char*)last_pt, remainder); //평문길이만큼만 파일에 저장

    fin.close();
    fout.close();

    cout << "file size(plaintext) = " << (num_block) * 16 + remainder << "bytes" << endl;
}

void File_CTS_test() {
    const char* pPT = "PT.bin";
    const char* pCT = "CT.bin";
    const char* pDecPT = "DecPT.bin";

    byte key[16], IV[16];
    for (int i = 0; i < 16; i++) {
        key[i] = i;
        IV[i] = i;
    }

    
    cout << "AES CTS Encrypt..." << endl;
    AES_Enc_CTS(pPT, key, IV, pCT);

    cout << "AES CTS Decrypt..." << endl;
    AES_Dec_CTS(pCT, key, IV, pDecPT);

}


int main()
{
    File_CTS_test();
}
