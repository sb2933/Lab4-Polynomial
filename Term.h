#ifndef TERM_H
#define TERM_H

#include "Variable.h"

#include <sstream>
#include <vector>


enum Operation {
    ADD,
    MULT,
    DIV,
    DIFF,
    END,
};

class Term {
private:
	int coefficient;
	vector<Variable> variables;
    Term* nextTerm = nullptr;
    Term* priorTerm = nullptr;
public:
    Term(int coefficient);
    Term(Term* t);
    Term(const Term& t) { *this = t; ResetTerms(); }
    Term(int coefficient, vector<Variable>& variables);
    Term(int coefficient, vector<Variable>& variables, Term* priorTerm);
    Term(string nodeInfo);
    int GetCoefficient() const;
    Term* GetNextTerm() const;
    Term* GetPriorTerm() const;
    vector<Variable> GetVariables() const;
    Term* GetAncestor();
    bool GetIsScalar();

    void CombineInto(Term* term); // Adding
    void MultInto(Term* term);      // Single term multiplication

    virtual Term* Differentiate();

    static Term* Multiply(Term* lhs, Term* rhs);
    static Term* Divide(Term* lhs, Term* rhs);
    Term operator*(const int& mult);
    Term operator!();
    
    Term operator*(const Term mult);
    Term operator/(const Term mult);


    void ResetTerms();
    void SetNextTerm(Term* term);
    void SetPriorTerm(Term* term);
    void FullPrint();
    void Print();
    void Simplify();
	void Sort();

    bool IsPolynomial();

    bool operator==(const Term& term) const;
    bool operator!=(const Term& term) const;
    bool operator<(const Term& term) const;
    bool operator>(const Term& term) const;
    bool operator<=(const Term& term) const;
    bool operator>=(const Term& term) const;
};

int GetInt(istringstream& iss);

void FlushWhitespace(istringstream& iss);

template <typename T> int VectorContains(vector<T> vec, T value) {
    for (int i = 0; i < vec.size(); i++) {
        if (vec.at(i) == value)
            return i;
    }
    return -1;
}

#endif