#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include "Term.h"
#include "Variable.h"

class Polynomial{
private:
    Term* head = nullptr;
    Term* foot = nullptr;
public:
    Polynomial();
    ~Polynomial();
    Polynomial(int coef);
    Polynomial(const Polynomial& poly);
    Polynomial(Term* term);
    Polynomial(Polynomial* poly);
    Polynomial(string info);
    void AnalyzeString(string info); //Parse string for construction
    void Print();
    void Simplify();
    void Sort();

    int CountTerms();
    void PrintTerms();

    Polynomial* Differentiate();


    static Polynomial* Multiply(Polynomial* lhs, Polynomial* rhs);
    static Polynomial* Multiply(Polynomial* lhs, Term* rhs);


    Polynomial* SubPoly(Term* term); //Get polynomial starting from term, removing it

    void Append(Term* term);
    void Append(Polynomial* p);
    void CombineTerms();
    void InsertAfter(Term* newTerm, Term* existingTerm);
    void InsertBefore(Term* newTerm, Term* existingTerm);
    void RemoveTerm(Term* term);
    void Prepend(Term* term);

    Term* GetHead() const {return head;}
    Term* GetFoot() const { return foot; }
};

/*
Polynomial operations:
x + y
x - y
x * y
x / y

(fg)' = fg' + f'g done
(f + g)' = f' + g'


(f/g)' = (f'g-fg')/g2
        = f'g-1 -fg'/g2
(f/g)' = (fg-1)'
        = f'g-1 - fg-2
        = (f'g-1) + (fg-2')

(axn)' = nax(n-1)

*/


#endif