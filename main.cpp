#include "Polynomial.h"
#include "Term.h"
#include "Variable.h"

int main() {
	
	string userIn;
	cout << "Enter two polynomials ended with a semicolon:" << endl;
	Polynomial* p1 = new Polynomial;
	Polynomial* p2 = new Polynomial;
	p1->Append(p2);
	p1->Simplify();
	cout << "Result: ";
	p1->Print();
	
}