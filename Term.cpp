#include "Term.h"

Term::Term(int coefficient) {
    this->coefficient = coefficient;
}

Term::Term(Term* t) {
    coefficient = t->coefficient;
    variables = t->GetVariables();
    Simplify();
}

Term::Term(int coefficient, vector<Variable>& variables) : coefficient(coefficient), variables(variables) {
    Simplify(); 
}

Term::Term(int coefficient, vector<Variable>& variables, Term* priorTerm) : coefficient(coefficient), variables(variables), priorTerm(priorTerm) { 
    Simplify();
    priorTerm->nextTerm = this;
}

Term::Term(string nodeInfo) {
    bool differentiate = false;
    istringstream iss;
    ostringstream oss("");
    string valString;
    char varChar;
    int varNum;
    iss.str(nodeInfo);
    char c;
    coefficient = GetInt(iss);
    while (iss) {
        oss.str("");
        varChar = char(tolower(iss.get()));
        if (!isalpha(varChar)) {
            cout << char(varChar) << " ? ? ?" << endl;
            throw invalid_argument("Letter not detected for variable.");
        }
        if (iss.peek() == ' ')
            varNum = 1;
        else
            varNum = GetInt(iss);
        variables.push_back(Variable(varNum, varChar));
        if (iss.peek() == '\'') {
            differentiate = true;
            break;
        }
    }
    Simplify();
    if (differentiate) {
        Term* t = Differentiate();
        t->nextTerm->SetPriorTerm(this);
        nextTerm = t->nextTerm;
        coefficient = t->coefficient;
        variables = t->variables;
    }
}

void Term::FullPrint() {
    Term* t = this;
    while (t != nullptr) {
        t->Print();
        t = t->GetNextTerm();
        cout << " ";
    }
    cout << endl;
}

void Term::Print() {
    if (coefficient == 0) {
        return;
    }
    cout << coefficient;
    for (int i = 0; i < variables.size(); i++) {
        variables.at(i).Print();
    }
}

void Term::Simplify() {
    Sort();
    vector<Variable> newVars;
    for (int i = 0; i < variables.size(); i++) {
        if (i == variables.size() - 1) {
            break;
        }
        if (variables.at(i).GetVariable() == variables.at(i + 1).GetVariable()) {
            variables.at(i).SetExponent(variables.at(i).GetExponent() + variables.at(i + 1).GetExponent());
            variables.erase(variables.begin() + i + 1);
        }
    }
}

void Term::Sort() {
	Variable lowestVar;
	int lowestCharIndex;
	Variable temp;
	for (int i = 0; i < variables.size(); i++) {
		lowestVar = variables.at(i);
		lowestCharIndex = i;
		for (int j = i; j < variables.size(); j++) {
			if (variables.at(j) < lowestVar) {
				lowestVar = variables.at(j);
				lowestCharIndex = j;
			}
		}
		if (lowestCharIndex == i)
			continue;	// No need for a swap
		temp = variables.at(lowestCharIndex);
		variables.at(lowestCharIndex) = variables.at(i);
		variables.at(i) = temp;
	}
}


Term* Term::Multiply(Term* lhs, Term* rhs) {
    vector<Variable> vars;
    int newCoef = lhs->coefficient * rhs->coefficient;
    for (int i = 0; i < lhs->variables.size(); i++) {
        vars.push_back(lhs->variables.at(i));
    }
    for (int i = 0; i < rhs->variables.size(); i++) {
        vars.push_back(rhs->variables.at(i));
    }
    return new Term(newCoef, vars);
}

Term* Term::Divide(Term* lhs, Term* rhs) {
    vector<Variable> vars;
    Variable var;
    int newCoef = lhs->coefficient / rhs->coefficient;
    for (int i = 0; i < lhs->variables.size(); i++) {
        vars.push_back(lhs->variables.at(i));
    }
    for (int i = 0; i < rhs->variables.size(); i++) {
        var = rhs->variables.at(i);
        var.Invert();
        vars.push_back(var);
    }
    return new Term(newCoef, vars);
}

void Term::CombineInto(Term* term) {
    if (*this != *term)
        return;
    coefficient += nextTerm->GetCoefficient();
}

void Term::MultInto(Term* term) {
    coefficient = coefficient * term->GetCoefficient();
    for (int i = 0; i < term->variables.size(); i++) {
        variables.push_back(term->variables.at(i));
    }
    Simplify();
}

void Term::ResetTerms() {
    priorTerm = nullptr;
    nextTerm = nullptr;
}

Term* Term::Differentiate() { // CLUSTERFUCK FIX
    if (variables.empty()) {
        return new Term(0);
    }
    Term* workTerm = nullptr;
    Term* newTerm = nullptr;
    Term* headTerm;
    int newCoef;
    vector<Variable> workVector = variables;
    newCoef = coefficient * workVector.at(0).GetExponent();
    workVector.at(0).Differentiate();
    workTerm = new Term(newCoef, workVector);
    headTerm = workTerm;
    for (int i = 1; i < variables.size(); i++) {
        workVector = variables;
        newCoef = coefficient * workVector.at(i).Differentiate();
        newTerm = new Term(newCoef, workVector, workTerm);
        workTerm = newTerm;
    }
    return headTerm;
}


Term* Term::GetAncestor() {
    Term* t = this;
    while (t->nextTerm != nullptr) {
        t = t->GetNextTerm();
    }
    return t;
}

bool Term::GetIsScalar() {
    if(variables.empty())
        return true;
    else return false;
}


Term Term::operator*(const int& mult) {
    coefficient *= mult;
}

Term Term::operator!() {

}


Term Term::operator*(const Term mult) {
    int newCoef = coefficient * mult.coefficient;
    vector<Variable> vars;
    for (int i = 0; i < variables.size(); i++) {
        vars.push_back(Variable(variables.at(i).GetExponent(), variables.at(i).GetVariable()));
    }
    for (int i = 0; i < mult.GetVariables().size(); i++) {
        vars.push_back(Variable(mult.GetVariables().at(i).GetExponent(), mult.GetVariables().at(i).GetVariable()));
    }
    return Term(newCoef, vars);
}

Term Term::operator/(const Term mult) {
}

bool Term::IsPolynomial() {
    if (nextTerm == nullptr)
        return false;
    return true;
}

bool Term::operator==(const Term& term) const {
    if (variables.size() != term.GetVariables().size())
        return false;
    for (int i = 0; i < variables.size(); i++) {
        if (variables.at(i) != term.GetVariables().at(i) || variables.at(i).GetExponent() != term.GetVariables().at(i).GetExponent())
            return false;
    }
    return true;
}

bool Term::operator!=(const Term& term) const {
    return !(*this == term);
}

bool Term::operator<(const Term& term) const {
    for (int i = 0; i < variables.size(); i++) {
        if (i == term.GetVariables().size()) {
            return true;
        }
        if (variables.at(i) < term.GetVariables().at(i)) {
            return true;
        }
        if (variables.at(i) > term.GetVariables().at(i)) {
            return false;
        }
    }
    return false;
}


bool Term::operator>(const Term& term) const {
    for (int i = 0; i < variables.size(); i++) {
        if (i == term.variables.size())
            return true;
        if (variables.at(i) > term.variables.at(i))
            return true;
        if (variables.at(i) < term.variables.at(i))
            return false;
    }
    return false;
}

bool Term::operator<=(const Term& term) const {
    if (*this == term)
        return true;
    return (*this < term);
}

bool Term::operator>=(const Term& term) const {
    if (*this == term)
        return true;
    return (*this < term);
}




int Term::GetCoefficient() const { return coefficient; }
Term* Term::GetNextTerm() const { return nextTerm; }
Term* Term::GetPriorTerm() const { return priorTerm; }
vector<Variable> Term::GetVariables() const { return variables; }


void Term::SetNextTerm(Term* term) { nextTerm = term; }
void Term::SetPriorTerm(Term * term) { priorTerm = term; }