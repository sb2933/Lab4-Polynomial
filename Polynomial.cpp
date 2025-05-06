#include "Polynomial.h"

Polynomial::Polynomial() {
    string userIn;
    getline(cin, userIn);
    AnalyzeString(userIn);
    Simplify();
}

Polynomial::Polynomial(int coef) {
    Append(new Term(coef));
}

Polynomial::~Polynomial() {
    Term* t;
    while (head != nullptr) {
        t = head;
        RemoveTerm(t);
        delete t;
    }
}

Polynomial::Polynomial(Term* term) {
    Term* t = term;
    while (t != nullptr) {
        Append(new Term(t));
        t = t->GetNextTerm();
    }
    Sort();
}

Polynomial::Polynomial(Polynomial* poly) {
    Term* t = poly->head;
    while (t != nullptr) {
        Append(new Term(t));
        t = t->GetNextTerm();
    }
    Sort();
}

Polynomial::Polynomial(const Polynomial& poly) {
    Term* t = poly.GetHead();
    while (t != nullptr) {
        Append(new Term(t));
        t = t->GetNextTerm();
    }
    Sort();
}

Polynomial::Polynomial(string info) {   // For parentheses
    istringstream iss(info);
    ostringstream oss;
    string newInfo;
    if (iss.peek() == '(') {
        int innerParenth = 0;
        iss.get();
        while (iss) {
            if (iss.peek() == '(') {
                innerParenth++;
            }
            if (iss.peek() == ')') {
                if (innerParenth == 0) {
                    newInfo = oss.str();
                    break;
                }
                innerParenth--;
            }
            oss << char(iss.get());
        }
        if (innerParenth != 0) {
            throw invalid_argument("Parentheses error");
        }
        AnalyzeString(newInfo);
    }
    else
        AnalyzeString(info);
    Sort();
}

void Polynomial::AnalyzeString(string info) {
    Operation op;
    istringstream iss;
    ostringstream oss;
    Term* lastTerm = nullptr;
    Polynomial* pSub;
    Polynomial* otherPoly;
    string termInfo;
    int parenthCount = 0;
    int diffCount = 0;
    char charInfo;
    iss.str(info);
    if (iss.peek() == '\n')
        iss.get();
    while (iss) {
        if (iss.peek() == ';')
            return;
        oss.str("");
        FlushWhitespace(iss);
        if (!iss)
            break;
        switch (iss.peek()) {
        case '+':
            op = ADD;
            iss.get();
            break;
        case '-':
            op = ADD;
            oss << char(iss.get());
            break;
        case '*':
            op = MULT;
            iss.get();
            pSub = SubPoly(lastTerm);
            break;
        case '\'':
            op = DIFF;
            iss.get();
            pSub = SubPoly(lastTerm);
            break;
        case ';':
            op = END;
            break;
        default:
            op = ADD;
        }
        if (op == END)
            return;
        FlushWhitespace(iss);

        if (op == DIFF) {
            Append(pSub->Differentiate());
            continue;
        }

        FlushWhitespace(iss);

        if (iss.peek() == '(') {
            oss << char(iss.get());
            parenthCount = 1;
            while (parenthCount != 0) {
                if (iss.peek() == '(')
                    parenthCount++;
                if (iss.peek() == ')') {
                    parenthCount--;
                }
                if (iss.peek() == ';')
                    throw invalid_argument("Parentheses error");
                oss << char(iss.get());
            }   //todo calculus
            while (iss.peek() == '\'') {
                diffCount++;
                iss.get();
            }
            otherPoly = new Polynomial(oss.str());
            while (diffCount != 0) {
                otherPoly = otherPoly->Differentiate();
                diffCount--;
            }
            lastTerm = otherPoly->head;
            if (op == ADD)
                Append(otherPoly);
            if (op == MULT) {
                Append(Multiply(pSub, otherPoly));
            }
            FlushWhitespace(iss);
            continue;
        }


        while (iss.peek() != '+' && iss.peek() != '(' && iss.peek() != '\'' && iss.peek() != ';' && iss.peek() != '\n' && iss.peek() != -1 && iss && iss.peek() != ' ') {
            if (iss.peek() == '-' && !isalpha(charInfo)) {
                break;
            }
            charInfo = char(iss.get());
            oss << char(charInfo);

            if (iss.peek() == '\'') {
                oss << char(iss.get());
                break;
            }
            if (iss.peek() == ' ' && charInfo != '-')
                break;
        }
        termInfo = oss.str();
        if (termInfo == " " || termInfo == "")
            continue;
        lastTerm = new Term(termInfo);
        switch (op) {
        case ADD:
            Append(lastTerm);
            break;
        case MULT:
            Append(Multiply(pSub, lastTerm));
            break;
        }
    }
}

Polynomial* Polynomial::SubPoly(Term* term) {
    Term* t = head;
    if (t == term) {
        head = nullptr;
        foot = nullptr;
        return new Polynomial(t);
    }
    while (t != nullptr && t != term) {
        t = t->GetNextTerm();
    }
    if (t == term) {
        foot = t->GetPriorTerm();
        t->GetPriorTerm()->SetNextTerm(nullptr);
        t->SetPriorTerm(nullptr);
        return new Polynomial(t);
    }
}


void Polynomial::Print() {
    Term* t = head;
    while (t != nullptr) {
        if (t == head) {
            t->Print();
            cout << " ";
            t = t->GetNextTerm();
            continue;
        }
        if (t->GetCoefficient() > 0)
            cout << "+";
        t->Print();
        cout << " ";
        t = t->GetNextTerm();
    }
    cout << endl; //temp?
}

void Polynomial::Simplify() {
    if (head == foot)
        return;
    Sort();
    CombineTerms();
}

void Polynomial::Sort()  {
    if (head == nullptr) {
        return;
    }
    Term* t = head->GetNextTerm();
    Term* nextTerm;
    Term* searchTerm;
    while (t != nullptr) {
        nextTerm = t->GetNextTerm();
        searchTerm = t->GetPriorTerm();
        while (searchTerm != nullptr && *t < *searchTerm) {
            searchTerm = searchTerm->GetPriorTerm();
        }
        if (searchTerm == nullptr) {
            RemoveTerm(t);
            Prepend(t);
        }
        else if (searchTerm == t->GetPriorTerm()) {
            // skip insertion if not necessary
        }
        else {
            RemoveTerm(t);
            InsertAfter(t, searchTerm);
        }
        t = nextTerm;
        if (t == nullptr)
            break;
    }
}

int Polynomial::CountTerms() {
    Term* t = head;
    int i = 0;
    while (t != nullptr) {
        i++;
        t = t->GetNextTerm();
    }
    return i;
}

void Polynomial::PrintTerms() {
    Term* t = head;
    while (t != nullptr) {
        t->Print();
        cout << endl;
        t = t->GetNextTerm();
    }
}


void Polynomial::CombineTerms() { // must be done after sort
    Term* t = head;
    while (t != foot) {
        if (t->GetVariables() == t->GetNextTerm()->GetVariables()) {
            t->CombineInto(t->GetNextTerm());
            RemoveTerm(t->GetNextTerm());
        }
        t = t->GetNextTerm();
    }
}
void Polynomial::Append(Term* term) {
    Term* t;
    if (head == nullptr) {
        head = term;
        foot = term->GetAncestor();
        return;
    }
    t = foot;
    t->SetNextTerm(term);
    term->SetPriorTerm(t);
    foot = term->GetAncestor();
}

void Polynomial::Append(Polynomial* p) {
    Append(p->GetHead());
}

void Polynomial::InsertAfter(Term* newTerm, Term* existingTerm) {
    Term* t = head;
    if (t == foot) {
        Append(newTerm);
        return;
    }
    while (t != nullptr) {
        if (t == existingTerm) {
            t->GetNextTerm()->SetPriorTerm(newTerm);
            newTerm->SetNextTerm(t->GetNextTerm());
            t->SetNextTerm(newTerm);
            newTerm->SetPriorTerm(t);
        }
        t = t->GetNextTerm();
    }
}

void Polynomial::InsertBefore(Term* newTerm, Term* existingTerm) {
    Term* t = head;
    if (existingTerm == head) {
        Prepend(newTerm);
        return;
    }
    while (t != nullptr) {
        if (t == existingTerm) {
            t->GetPriorTerm()->SetNextTerm(newTerm);
            newTerm->SetPriorTerm(t->GetPriorTerm());
            newTerm->SetNextTerm(t);
            t->SetPriorTerm(newTerm);
        }
        t = t->GetNextTerm();
    }
}

void Polynomial::RemoveTerm(Term* term) {
    Term* t = head;
    if (term == head) {
        t = term->GetNextTerm();
        head = t;
        t->SetPriorTerm(nullptr);
        term->ResetTerms();
    }
    if (term == foot) {
        foot = term->GetPriorTerm();
        foot->SetNextTerm(nullptr);
        term->ResetTerms();
        return;
    }
    while (t != nullptr) {
        if (term == t) {
            t->GetNextTerm()->SetPriorTerm(t->GetPriorTerm());
            t->GetPriorTerm()->SetNextTerm(t->GetNextTerm());
            term->ResetTerms();
            return;
        }
        t = t->GetNextTerm();
    }
}

void Polynomial::Prepend(Term* term) {
    term->SetPriorTerm(nullptr);
    if (head == nullptr) {
        head = term;
        return;
    }
    Term* t = head;
    t->SetPriorTerm(term);
    term->SetNextTerm(t);
    head = term;
}



Polynomial* Polynomial::Multiply(Polynomial* lhs, Polynomial* rhs) {
    Term* lhsTerm = lhs->head;
    Term* rhsTerm = rhs->head;
    Term* t;
    Polynomial* p = nullptr;
    while (lhsTerm != nullptr) {
        while (rhsTerm != nullptr) {
            t = new Term(Term::Multiply(lhsTerm, rhsTerm));
            if (p == nullptr)
                p = new Polynomial(t);
            else
                p->Append(t);
            rhsTerm = rhsTerm->GetNextTerm();
        }
        lhsTerm = lhsTerm->GetNextTerm();
        rhsTerm = rhs->head;
    }
    return p;
}

Polynomial* Polynomial::Multiply(Polynomial* lhs, Term* rhs) {
    Term* lhsTerm = lhs->head;
    Term* t;
    if (lhsTerm == nullptr || rhs == nullptr)
        throw invalid_argument("Null pointer(s) supplied to Multiply function");
    t = new Term(Term::Multiply(lhsTerm, rhs));
    Polynomial* p = new Polynomial(t);
    lhsTerm = lhsTerm->GetNextTerm();
    while (lhsTerm != nullptr) {
        t = new Term(Term::Multiply(lhsTerm, rhs));
        p->Append(t);
        lhsTerm = lhsTerm->GetNextTerm();
    }
    p->Print();
    return p;
}




Polynomial* Polynomial::Differentiate() {
    Term* t = head;
    Term* tDiff = t->Differentiate();
    Polynomial* p = new Polynomial(tDiff);
    t = t->GetNextTerm();
    while (t != nullptr) {
        tDiff = t->Differentiate();
        p->Append(tDiff);
        t = t->GetNextTerm();
    }
    return p;
}