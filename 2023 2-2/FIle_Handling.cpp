#include <iostream>
#include <fstream>
using namespace std;

void cout_test(){
    int n;
    double x = 10.0;
    n = 10;

    cout << "n = " << n << endl;
    cout << "x = " << x << endl;

    cout.unsetf(ios::dec);
    cout.setf(ios::hex | ios::showbase | ios::uppercase);
    
    cout << "n = "; 
    cout.width(15);
    cout.fill('*');       //남은 자리에 채움
    cout << n << endl;    //정수형 에만 flag가 적용됨
    cout << "x = " << x << endl;
    
}

void textfile_write(){
    ofstream fout;
    
    fout.open("file_out1.txt");
    if (fout.fail()){
        cout << "File open error!" << endl;
        return;
    }
    char buf[255];
    cout << "Input text: ";
    cin >> buf;
    fout << buf << endl;
    fout << "my first C++ file." ; 
    
    fout.close();

}
void textfile_copy(){
    ifstream fin;
    ofstream fout;

    fin.open("file_in.txt");
    if (fin.fail()) {
        cout << "Input file error!" << endl;
        return;
    }
    fout.open("file_out.txt");
    if(fout.fail()){
        cout << "Output file error." << endl;
        return;
    }
    char ch;
    while(fin.get(ch)){     //call by reference ... 함수정의: get(char  &ch)...
        fout.put(ch);

    }
    fin.close();
    fout.close();
}

void textfile_buffer(){
    ifstream fin;
    char ch;
    char buffer[80];
    fin.open("file_in.txt");
    if(fin.fail()){
        cout << "File open error !" << endl;
        return;
    }
    int counter;
    counter = 0;
    while (fin.get(ch)){    //get(char &ch) 선언 --> call by reference
        buffer[counter] = ch;
        counter++; 
    }
    fin.close();
    cout << "counter = " << counter << endl;
    buffer[counter] = 0x00; // null-character
    //c언어 문자열(string) null - terminating string
    //문자열 = [문자1][문자2]...[문자n] [0x00] <- 0x00이 나오면 문자열의 끝을 알림.
    cout << buffer;
}

void binfine_make(){
    ofstream fout;
    char ch;
    int x;
    
    //binary file: 바이트 배열로 파일을 만들기 (0x00 ~ 0xff)
    fout.open("binary.bin", ios::binary);
    if(fout.fail()){
        cout<<"file open error !" << endl;
        return;
    }
    ch = 'A';
    x = 0x01020304; //16진수로 표현된 정수
    fout.write(&ch , 1);    //(데어터가 있는 주소, 데이터 크기_byte 단위)
    //x = [01020304] -> [04, 03, 02, 01] (little endian)
    fout.write((char*)&x, sizeof(x));   //x가 들어았는 주소. -> 정수말고 문자형으로 강제 통일 시켜 읽힘

    fout.close();

}

void binfile_copy(){
    ifstream fin;
    ofstream fout;
    char ch;

    fin.open("binary.bin", ios::binary);
    if(fin.fail()){
        cout << "Input file open error" << endl;
        return;
    }
    fout.open("binary_copy.bin", ios::binary);
    if(fout.fail()){
        cout << "Output file open error" << endl;
        return;
    }    
    
    //한글자씩 읽어서 복사하기
    while(fin.read(&ch, 1)){        // read(저장할 주소, 데이터 길이)
        fout.write(&ch, 1);         // 주소에 저장된 데이터를 한글짜 가져와 쓰세요
        
    }       
    fin.close();
    fout.close();
}

void file_size_ver1(){
    ifstream fin;
    
    fin.open("binary.bin", ios::binary);
    if(fin.fail()){
        cout << "Input file open error" << endl;
        return;
    }    
    char ch;
    int counter = 0;
    while(fin.read(&ch, 1)){
        counter++;
    }

    cout << "File size : " << counter << endl;

    fin.close();
}

void file_size_ver2(){
    ifstream fin;
    
    fin.open("binary.bin", ios::binary);
    if(fin.fail()){
        cout << "Input file open error" << endl;
        return;
    }    
    //파일 크기 알아보기
    int file_size;
    fin.seekg(0, fin.end);  //파일 포인터를 맨 뒤로
    file_size = fin.tellg();    //현재 포인터 위치정보

    cout << "File size : " << file_size << endl;

    fin.close();
}

int main(){
    //std::cout << "Hello World!\n";
    //cout_test();
    //textfile_write();
    //textfile_copy();
    //textfile_buffer();
    //binfine_make();
    //binfile_copy();
    //file_size_ver1();
}