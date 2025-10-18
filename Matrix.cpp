#include <iostream>
#include <vector>

class Matrix
{
protected:                    
    int row;
    int col;
    std::vector<std::vector<float>> values;

public:
//конструкторы
    Matrix(int r, int c, float number = 0.);
    Matrix(int r, int c, std::vector<float> data);
    Matrix(int r, int c, std::vector<std::vector<float>> data);
//методы и операторы
    Matrix operator + (const Matrix& M);
    Matrix operator + (const float& num);
    Matrix operator - (const Matrix& M);
    Matrix operator - (const float& num);
    Matrix operator * (const Matrix& M);
    Matrix operator * (const float& num);
    Matrix operator - ();
    Matrix T();
    void print();
    int getRow() const { return row; }
    int getCol() const { return col; }
//вспомогательные методы для Sq_Matrix
protected:
    float det();          
    Matrix inverse();      
};

//квадратные матрицы
class Sq_Matrix: public Matrix
{
public:
//конструкторы
    Sq_Matrix(int size, float number = 0)
        : Matrix(size, size, number) {}

    Sq_Matrix(int size, std::vector<float> data)
        : Matrix(size, size, data) {}

    Sq_Matrix(int size, std::vector<std::vector<float>> data)
        : Matrix(size, size, data) {}
//методы
    Sq_Matrix(const Matrix& m);
    Sq_Matrix operator !();
    Sq_Matrix inverse();
    float det();
};



// Векторные матрицы
class Row_Vector: public Matrix
{
public:
    Row_Vector(int size, std::vector<float> data)
        : Matrix(1, size, data) {}

    Row_Vector(int size, float number = 0)
        : Matrix(1, size, number) {}

    float abs();
};

// Векторные матрицы
class Col_Vector: public Matrix
{
public:
    Col_Vector(int size, std::vector<float> data)
        : Matrix(size, 1, data) {}

    Col_Vector(int size, float number = 0)
        : Matrix(size, 1, number) {}

    float abs();
};


float Matrix::det()
{
    if(row != col){
        throw "Matrix must be square to calculate determinant";
    }

    if(row == 1){
        return values[0][0];
    }

    if(row == 2){
        return values[0][0] * values[1][1] - values[0][1] * values[1][0];
    }

    float determinant = 0;
    
    for(int c = 0; c < col; ++c){
        std::vector<std::vector<float>> sub_data;
        for(int i = 1; i < row; ++i){
            std::vector<float> sub_row;
            for(int j = 0; j < col; ++j){
                if(j == c) continue;
                sub_row.push_back(values[i][j]);
            }
            sub_data.push_back(sub_row);
        }
        Matrix subMatrix(row - 1, col - 1, sub_data);
        determinant += (c % 2 == 0 ? 1 : -1) * values[0][c] * subMatrix.det();
    }
    return determinant;
}

Matrix::Matrix(int r, int c, float number)
{
    values = std::vector<std::vector<float>>(r, std::vector<float>(c, number));
    row = r;
    col = c;
}

Matrix::Matrix(int r, int c, std::vector<float> data)
{
    values = std::vector<std::vector<float>>(r, std::vector<float>(c, 0));
    row = r;
    col = c;
    if(data.size() != (size_t)r*c){
        throw "Data size does not match matrix size";
    }
    int i = 0;
    for(auto& str: values)
        for(float& n: str)
            n = data[i++];
}

Matrix::Matrix(int r, int c, std::vector<std::vector<float>> data)
{
    if(data.size() != (size_t)r || data.empty() || data[0].size() != (size_t)c){
        throw "Data size do not match matrix size";
    }
    values = data;
    row = r;
    col = c;
    int i = 0;
    for(auto& elem: values)
        elem = data[i++];
}

Matrix Matrix::operator + (const Matrix& M)
{
    if(row != M.row || col != M.col){
        throw "Matrices dimensions don't match for addition";
    }
    Matrix result(row, col);
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < col; ++j){
            result.values[i][j] = values[i][j] + M.values[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator + (const float& num)
{
    Matrix result(row, col);
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < col; ++j){
            result.values[i][j] = values[i][j] + num;
        }
    }
    return result;
}

Matrix Matrix::operator - (const Matrix& M)
{
    if(row != M.row || col != M.col){
        throw "Matrices dimensions don't match for subtraction";
    }
    Matrix result(row, col);
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < col; ++j){
            result.values[i][j] = values[i][j] - M.values[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator - (const float& num)
{
    Matrix result(row, col);
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < col; ++j){
            result.values[i][j] = values[i][j] - num;
        }
    }
    return result;
}

Matrix Matrix::operator * (const Matrix& M)
{
    if(col != M.row){
        throw "Matrix's size doesn't match for multiplication";
    }
    Matrix result(row, M.col);
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < M.col; ++j){
            result.values[i][j] = 0;
            for(int k = 0; k < col; ++k){
                result.values[i][j] += values[i][k] * M.values[k][j];
            }
        }
    }
    return result;
}

Matrix Matrix::operator * (const float& num)
{
    Matrix result(row, col);
    for(int i = 0; i < row; ++i){
        for( int j = 0; j < col; ++j){
            result.values[i][j] = values[i][j] * num;
        }
    }
    return result;
}

Matrix Matrix::operator - ()
{
    Matrix result(row, col);
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < col; ++j){
            result.values[i][j] = -values[i][j];
        }
    }
    return result;
}

Matrix Matrix::T()
{
    Matrix result(col, row);
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < col; ++j){
            result.values[j][i] = values[i][j];
        }
    }
    return result;
}

void Matrix::print()
{
    for(const auto& elem : values)
    {
        for(const auto& n : elem)
        {
            std::cout << n << ' ';
        }
        std::cout << '\n';
    }
}

Matrix Matrix::inverse()
{
    if(row != col){
        throw "Matrix must be square for inverse";
    }
    float d = det();
    if(d == 0){
        throw "Error! det = 0";
    }
    Matrix cof(row, col);
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < col; ++j){
            std::vector<std::vector<float>> minor;
            for(int mi = 0; mi < row; ++mi){
                if(mi == i) continue;
                std::vector<float> minor_row;
                for(int mj = 0; mj < col; ++mj){
                    if(mj == j) continue;
                    minor_row.push_back(values[mi][mj]);
                }
                minor.push_back(minor_row);
            }
            Matrix m(row-1, col-1, minor);
            cof.values[i][j] = ((i+j)%2==0 ? 1 : -1) * m.det();
        }
    }
    Matrix adj = cof.T();
    return adj * (1.0f/d);
}



// Реализация методов Sq_Matrix
Sq_Matrix::Sq_Matrix(const Matrix& m) 
    : Matrix(m)
{
    if(m.getRow() != m.getCol()) {
        throw "Matrix must be square!";
    }
}

// публичная обёртка det()
float Sq_Matrix::det()
{
    return Matrix::det();
}

Sq_Matrix Sq_Matrix::operator!()
{
    Matrix inv = Matrix::inverse();  
    return Sq_Matrix(inv);            
}

Sq_Matrix Sq_Matrix::inverse()
{
    Matrix inv = Matrix::inverse();
    return Sq_Matrix(inv);
}

// Примеры использования
int main() 
{
    try {
        std::vector<std::vector<float>> a = {
            {1, 2},
            {3, 4}
        };
        std::vector<std::vector<float>> b = {
            {1, 2, 3},
            {4, 5, 6}
        };

        Matrix A(2, 2, a);
        Matrix B(2, 3, b);
        Matrix C = A * B;  
        
        Matrix D(3, 2, b); 
        Matrix E = A * D;  
        
        C.print();
    }
    catch (const char* error_message) {
        std::cout << "Error: " << error_message << std::endl;
    }
    catch (...) {
        std::cout << "Unknown error" << std::endl;
    }
    
    try {
        std::vector<std::vector<float>> a = {
            {5, 6},
            {7, 8}
        };
        Matrix A(2, 2, a);
        Matrix B(2, 2, a);


        Matrix X = A - B;
        X.print();
    }
    catch (const char* msg) {
        std::cout << "Error: " << msg << std::endl;
    }
    catch (...) {
        std::cout << "Unknown error" << std::endl;
    }
    
    return 0;
}