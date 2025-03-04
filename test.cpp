#include "SymDiff.hpp"
#include <iostream>

int main() {
    Expression<double> x("x");
    Expression<double> y("y");
    Expression<double> expr = (x + y) * (x - y);

    std::map<std::string, double> variables = {{"x", 3}, {"y", 2}};
    std::cout << "Выражение: " << expr.toString() << std::endl;
    std::cout << "Результат: " << expr.evaluate(variables) << std::endl;

    Expression<double> deriv = expr.derivative("x");
    std::cout << "Производная от x: " << deriv.toString() << std::endl;
    std::cout << "Результат: " << deriv.evaluate(variables) << std::endl;

    return 0;
}