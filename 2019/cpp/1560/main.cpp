#include <iostream>
#include <iomanip>
#include <cstdlib>
using namespace std;

class Matrix{
public:
    Matrix(int _r, int _c);//MARK
    Matrix(const Matrix &a);
    ~Matrix();
    Matrix operator +(const Matrix &mat);
    Matrix operator -(const Matrix &mat);
    Matrix operator *(const Matrix &mat);
    friend ostream & operator << (ostream &out, const Matrix &mat);
    friend istream & operator >> (istream &in, Matrix &mat);
private:
    int r,c;
    double **a;
};

Matrix::Matrix(int _r, int _c){//MARK
    r=_r;//MARK
    c=_c;//MARK
    a = new double*[r];
    for(int i=0; i<r; i++){
        a[i] = new double[c];
    }
}

Matrix::Matrix(const Matrix &mat){
    this->c = mat.c;
    this->r = mat.r;
    this->a = new double*[r];
    for(int i=0; i<r; i++){
        this->a[i] = new double[c];
        for(int j=0; j<c; j++){
            this->a[i][j] = mat.a[i][j];
        }
    }
}

Matrix::~Matrix(){
    for(int i=0; i<r; i++){
        delete[]a[i];
    }
    delete[]a;
}

Matrix Matrix::operator +(const Matrix &mat){
    if(this->r!=mat.r||this->c!=mat.c){
        cout<<"invalid operation!"<<endl;
        exit(1);
    }
    else{
        Matrix sum(r,c);
        for(int i=0; i<r; i++){
            for(int j=0; j<c; j++){
                sum.a[i][j] = this->a[i][j]+mat.a[i][j];
            }
        }
        return sum;
    }

}

Matrix Matrix::operator -(const Matrix &mat){
   if(this->r!=mat.r||this->c!=mat.c){
        cout<<"invalid operation!"<<endl;
        exit(1);
    }
    else{
        Matrix sub(r,c);
        for(int i=0; i<r; i++){
            for(int j=0; j<c; j++){
                sub.a[i][j] = this->a[i][j]-mat.a[i][j];
            }
        }
        return sub;
    }
}

Matrix Matrix::operator *(const Matrix &mat){
   if(this->c!=mat.r){
        cout<<"invalid operation!"<<endl;
        exit(1);
    }
    else{
        Matrix mul(this->r,mat.c);
        for(int i=0; i<r; i++){
            for(int j=0; j<c; j++){
                mul.a[i][j] = 0;
            }
        }
        for(int i=0; i<r; i++){
            for(int j=0; j<mat.c; j++){
                for(int k=0; k<this->c; k++){
                    mul.a[i][j] += this->a[i][k]*mat.a[k][j];
                }
            }
        }
        return mul;
    }

}
ostream & operator << (ostream &out, const Matrix &mat){
    for(int i=0; i<mat.r; i++){
        for(int j=0; j<mat.c; j++){
            out<<setw(6)<<mat.a[i][j];
        }
        out<<endl;
    }
    return out;
}
istream & operator >> (istream &in, Matrix &mat){
    for(int i=0; i<mat.r; i++){
        for(int j=0; j<mat.c; j++){
            in>>mat.a[i][j];
        }
    }
    return in;
}
int main() {
    int r,c;

    cin>>r>>c;
    if(r<1 || c<1){
    cout<<"Input error."<<endl;
    exit(1);
    }
    Matrix A(r,c);
    cin >> A;

    cin>>r>>c;
    if(r<1 || c<1){
    cout<<"Input error."<<endl;
    exit(1);
    }
    Matrix B(r,c);
    cin >> B;


    cout<<"A + B:"<<endl;
    cout << A + B <<endl;

    cout<<"A - B:"<<endl;
    cout << A - B <<endl;

    cout<<"A * B:"<<endl;
    cout << A * B <<endl;

    return 0;
}
