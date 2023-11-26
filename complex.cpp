// Complex1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>

//global variable
//int counter;

//복소수 클래스 만들기
class Complex { // 일반적으로 클래스 이름은 대문자로 시작
public:
    double real;  // 실수부
    double imag;  // 허수부
    static int counter; // 정적변수
public: //외부에서 접근 가능한 영역
    //Complex() {}; // default comstructor 기본 생성자
    //Complex() { real = 1.0; imag = 0.0; }; // 생성자
    Complex(double x = 0., double y = 0.) { 
        real = x; imag = y; 
        Complex::counter++;
        //printf("counter = %d\n", Complex::counter);
    };
    //Complex(double x, double y) { real = x; imag = y; };
    // 동일한 이름의 두 가지 함수를 만드는 것 ==> 오버로딩(overloading)
    void Print() { 
        if(imag >= 0){
            printf("%f + %f i \n", real, imag);
        }
        else{
            printf("%f + (%f) i \n", real, imag);
        }
    };

    inline double Re() { return real; };
    inline double Im() { return imag; };
    inline void SetRe(double x) { real = x; };
    inline void SetIm(double y) { imag = y; };

    Complex conjugate(){
        Complex result;
        result.SetRe(real);
        result.SetIm(-(imag));
        return result;
    }
    static Complex add(Complex z1, Complex z2){
        Complex result;
        result.real = z1.real + z2.real;
        result.imag = z1.imag + z2.imag;
        return result;
    }
    static Complex sub(Complex z1, Complex z2){
        Complex result;
        result.real = z1.real - z2.real;
        result.imag = z1.imag - z2.imag;
        return result;
    }
    static Complex mul(Complex z1, Complex z2){
        Complex result;
        result.real = (z1.real * z2.real) - (z1.imag * z2.imag);
        result.imag = (z1.real * z2.imag) + (z1.imag * z2.real);
        return result;
    }
    static Complex div(Complex z1, Complex z2){
        Complex result;
        Complex z2_conj, denom;
        double d;
        z2_conj = z2.conjugate();
        d = (z2.real * z2.real) + (z2.imag * z2.imag);
        denom = mul(z1, z2_conj);
        result = Complex(denom.real/d, denom.imag/d);
        return result;
    }
    //실수와 복소수간의 덧셈, 곱셈
    static Complex dou_comp_add(double z1, Complex z2){
        Complex tocomp, result;
        tocomp.real = z1;
        tocomp.imag = 0.;
        result = add(tocomp, z2);
        return result;
    }
    static Complex dou_comp_mul(double z1, Complex z2){
        Complex tocomp, result;
        tocomp.real = z1;
        tocomp.imag = 0.;
        result = mul(tocomp, z2);
        return result;
    }    

};

void complex_test1(){
    Complex z1(1.5, 0.);
    Complex z2(4., 3.);
    Complex z3;
    double z4, z6;
    z1.conjugate();
    z3 = Complex::add(z1, z2);
    printf("z1 + z2 = ");
    z3.Print();
    z3 = Complex::sub(z1, z2);
    printf("z1 - z2 = ");
    z3.Print();
    z3 = Complex::mul(z1, z2);
    printf("z1 * z2 = ");
    z3.Print();
    z3 = Complex::div(z1, z2);
    printf("z1 / z2 = ");
    z3.Print();

    z4 = 1.3;
    Complex z5(2.2, 2.5);
    z3 = Complex::dou_comp_add(z4, z5);
    printf("1.3 + (2.2 + 2.5 i) = ");
    z3.Print();
    z6 = 1.5;
    Complex z7(4., 3.);
    z3 = Complex::dou_comp_mul(z6, z7);
    printf("1.5 * (4 + 3 i) = ");
    z3.Print();
}

int Complex::counter = 0;

int main()
{
    //counter = 0;
    complex_test1();

    // (1) 전역변수 없이 카운터 만들기
    // (2) 현재 메모리를 차지하는 클래스의 인스턴스 개수 카운트 하기

}

