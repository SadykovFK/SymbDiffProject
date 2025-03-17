#include <iostream>
#include <map>
#include "expression.hpp"
#include "parser.hpp"

static void check(bool condition, const std::string& testName) {
    if (condition) {
        std::cout << "[OK]   " << testName << std::endl;
    } else {
        std::cout << "[FAIL] " << testName << std::endl;
    }
}

int main() {
    // Тест 1: парсинг и вычисление "2+2"
    {
        Expression<double> expr = parseExpression("2+2");
        double res = expr.evaluate({});
        check(res == 4.0, "2+2 == 4");
    }

    // Тест 2: x^2 при x=3 => 9
    {
        Expression<double> expr = parseExpression("x^2");
        double res = expr.evaluate({{"x", 3.0}});
        check(res == 9.0, "x^2 при x=3 => 9");
    }

    // Тест 3: производная x^2 по x => 2*x
    {
        Expression<double> expr = parseExpression("x^2");
        Expression<double> d = expr.derivative("x");
        std::string result = d.toString(); 
        check(result.find("2") != std::string::npos && result.find("x") != std::string::npos,
              "d/dx(x^2) содержит '2' и 'x'");
    }

    // Тест 4: sin(x) при x=0 => 0
    {
        Expression<double> expr = parseExpression("sin(x)");
        double res = expr.evaluate({{"x", 0.0}});
        check(std::abs(res) < 1e-12, "sin(0) = 0");
    }

    // Тест 5: diff sin(x) => cos(x)
    {
        Expression<double> expr = parseExpression("sin(x)");
        Expression<double> d = expr.derivative("x");
        std::string result = d.toString(); 
        check(result.find("cos") != std::string::npos,
              "d/dx(sin(x)) = cos(x)");
    }

    return 0;
}
