#include<stdio.h>


//global variable
//int counter;

//복소수 클래스 만들기
class Complex{  //일반적으로 클래스 이름은 대문자로 시작
private:    
    double real;    //실수부
    double imag;    //허수부
    static int counter; //정적변수 (클래스를 통틀어 메모리 하나만 차지. 클래스가 재실행되도 초기화 x)
public: // 외부에서 접근 가능한 영역
    //Complex(){}; //기본 생성자
    //Complex() {real = 1.0; imag = 0.0;}; //생성자
    Complex(double x=0., double y=0.){
        real =x ; imag = y;
        Complex::counter++;     //생성한 변수의 개수 저장 (정적변수)
        printf("[c]counter = %d\n", Complex::counter);
    };
    //Complex(double x, double y){real = x ; imag = y;}; ==> 파라미터에 double x, y를 넣어야만 실행됨
    //동일한 이름의 두가지 함수를 만드는 것 ==> 오버로딩(overloading)

    //복사 생성자 (Copy Constructor): 함수의 변수를 파라미터로 전달할 때 => 용도 중요 알아야 함!!
    Complex(Complex& z) {
        real = z.real; imag = z.imag;
        counter++;
        printf("[&]counter = %d\n", Complex::counter);
    };
    ~Complex() {    //소멸자 (함수 형식은 고정!! 파라미터 X)
        counter--;
        printf("[d]counter = %d\n", Complex::counter);
    }; 

    void Print(){ printf("%f + %f i \n", real, imag);};
    // 복소수 덧셈 - 1단계
    Complex Add(Complex z){     //z3 = z1.Add(z2)
        Complex result;
        result.real = real + z.real;    
        result.imag = imag + z.imag;
        return result;
    }

    // z3 = Add(z1, z2)
    static Complex Add(Complex z1, Complex z2){     //
        Complex z3;

        z3.real = z1.real + z2.real;
        z3.imag = z1.imag + z2.imag;

        return z3;
    }
    inline double Re(){return real;};
    inline double Im(){return imag;};
    inline void SetRe(double x){real = x;}; //외부에서 z1.SetRe(3.0); 으로 설정가능하게 해줌 z1.real=3.0; -> x
    inline void SetIm(double y){imag = y;};
};

void complex_test1(){
    Complex z1(1., 2.);
    Complex z2, z3, z4;
    z1.Print();
    z2.Print();

    //z3.SetRe(z1.Re() + z2.Re());
    //z3.SetIm(z1.Im() + z2.Im());

    z3 = z1.Add(z2);
    z3.Print();

    z4 = Complex::Add(z1, z2);       //Add는 멤버함수

    //z5 = z1 + z2; -> 나중에 구현!

    double abs;
    abs = z2.Re() * z2.Re() + z2.Im() * z2.Im();    //public 영역에 따로 함수를 만든다면 .real .imag 와 같은 기능을 사용가능

}

//클래스에 소속된 정적 변수의 초기화는 함수의 밖에서
//main 함수 실행 전에 실행가능. 프로그램 시작부터 메모리가 생기기 때문.
int Complex::counter = 0;     

int main()
{
    //counter = 0;
    complex_test1(); //실행했을때 스택에 로드 후 끝날때 해제. (최대 3개씩 메모리 차지)

    // (1) 전역변수 없이 카운터 만들기
    // (2) 현재 메모리를 차지하는 클래스의 인스턴스 개수 카운트 하기

}

