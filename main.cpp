#include <iostream>
#include <complex>
#include "expression.hpp"

int main() {
    using namespace std;

    Expression<double> expr1(5.8);
    Expression<double> expr2("x");
    Expression<double> expr3 = expr1 + expr2;
    Expression<double> expr4 = expr3 * Expression<double>::sin(expr2);

    cout << "Expression: " << expr4.toString() << endl;

    map<string, double> variables = {{"x", 3.14}};
    cout << "Evaluation: " << expr4.evaluate(variables) << endl;

    Expression<complex<double>> cexpr1(complex<double>(2.0, 3.0));
    Expression<complex<double>> cexpr2("z");
    Expression<complex<double>> cexpr3 = cexpr1 + cexpr2;
    Expression<complex<double>> cexpr4 = cexpr3 * Expression<complex<double>>::exp(cexpr2);

    cout << "Complex Expression: " << cexpr4.toString() << endl;

    map<string, complex<double>> cvariables = {{"z", complex<double>(1.0, -1.0)}};
    cout << "Complex Evaluation: " << cexpr4.evaluate(cvariables) << endl;

    return 0;
}