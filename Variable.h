#ifndef VARIABLE_H
#define VARIABLE_H

#include <iostream>
#include <sstream>

using namespace std;

class Variable {
    private:
        int exponent = 0;
        char variable = '/';
    public:
        Variable() {}
        Variable(const Variable& var) { *this = var; }
        Variable(int exponent, char variable) : exponent(exponent), variable(variable) {}    

        void Invert();

        int Differentiate(); //power rule, return coefficient

        int GetExponent() const {return exponent;}
        char GetVariable() const {return variable;}

        void SetExponent(int exp) {exponent = exp;}

        void Print() const;

        bool operator==(const Variable& var) const;
        bool operator!=(const Variable& var) const;
        bool operator<(const Variable& var) const;
        bool operator>(const Variable& var) const;
        bool operator<=(const Variable& var) const;
        bool operator>=(const Variable& var) const;
};





#endif