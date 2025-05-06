#include "Variable.h"

int Variable::Differentiate() {
    exponent--;
    return exponent + 1;
}

void Variable::Invert() {
    exponent *= -1;
}

void Variable::Print() const {
    cout << variable;
    if (exponent != 1)
        cout << exponent;
}

// Comparison operators to sort by variable chars
bool Variable::operator==(const Variable& var) const {
    if (variable == var.variable && exponent == var.exponent)
        return true;
    return false;
}
bool Variable::operator!=(const Variable& var) const {
    return !(*this == var);
}
bool Variable::operator<(const Variable& var) const {
    if (*this == var) {
        return false;
    }
    if (variable < var.variable) {
        return true;
    }
    if (variable == var.variable && exponent > var.exponent)
        return true;
    return false;
}
bool Variable::operator>(const Variable& var) const {
    if (*this == var)
        return false;
    if (variable > var.variable)
        return true;
    if (variable == var.variable && exponent < var.exponent)
        return true;
    return false;
}
bool Variable::operator<=(const Variable& var) const {
    if (*this == var)
        return true;
    if (*this < var)
        return true;
    return false;
}
bool Variable::operator>=(const Variable& var) const {
    if (*this == var)
        return true;
    if (*this > var)
        return true;
    return false;
}

void FlushWhitespace(istringstream& iss) {
    while (iss.peek() == ' ') {
        iss.get();
    }
}
int GetInt(istringstream& iss) {
    ostringstream oss;
    string info;
    FlushWhitespace(iss);
    if (iss.peek() == '+')
        iss.get();
    if (!isdigit(iss.peek()) && iss.peek() != '-')
        return 1;
    if (iss.peek() == '-') {
        oss << char(iss.get());
    }
    FlushWhitespace(iss);
    while (isdigit(char(iss.peek()))) {
        oss << char(iss.get());
    }
    FlushWhitespace(iss);
    info = oss.str();
    if (info == "" || info == "+")
        return 1;
    if (info == "-")
        return -1;
    return stoi(info);
}