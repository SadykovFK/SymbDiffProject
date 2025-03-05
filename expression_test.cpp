// run with g++ -std=c++11 -o expression_test expression.cpp expression_test.cpp
// ./expression_test

#include "expression.hpp"
#include <iostream>
#include <cassert>
#include <map>

void testConstant() {
    Expression<double> expr(5.0);
    assert(expr.evaluate({}) == 5.0);
    assert(expr.toString() == "5");
}

void testVariable() {
    Expression<double> expr("x");
    std::map<std::string, double> variables = {{"x", 3.0}};
    assert(expr.evaluate(variables) == 3.0);
    assert(expr.toString() == "x");
}

void testAddition() {
    Expression<double> expr1(2.0);
    Expression<double> expr2(3.0);
    Expression<double> result = expr1 + expr2;
    assert(result.evaluate({}) == 5.0);
    assert(result.toString() == "(2 + 3)");
}

void testSubtraction() {
    Expression<double> expr1(5.0);
    Expression<double> expr2(3.0);
    Expression<double> result = expr1 - expr2;
    assert(result.evaluate({}) == 2.0);
    assert(result.toString() == "(5 - 3)");
}

void testMultiplication() {
    Expression<double> expr1(2.0);
    Expression<double> expr2(3.0);
    Expression<double> result = expr1 * expr2;
    assert(result.evaluate({}) == 6.0);
    assert(result.toString() == "(2 * 3)");
}

void testDivision() {
    Expression<double> expr1(6.0);
    Expression<double> expr2(3.0);
    Expression<double> result = expr1 / expr2;
    assert(result.evaluate({}) == 2.0);
    assert(result.toString() == "(6 / 3)");
}

void testPower() {
    Expression<double> expr1(2.0);
    Expression<double> expr2(3.0);
    Expression<double> result = expr1 ^ expr2;
    assert(result.evaluate({}) == 8.0);
    assert(result.toString() == "(2 ^ 3)");
}

void testSin() {
    Expression<double> expr(0.0);
    Expression<double> result = Expression<double>::sin(expr);
    assert(result.evaluate({}) == 0.0);
    assert(result.toString() == "sin(0)");
}

void testCos() {
    Expression<double> expr(0.0);
    Expression<double> result = Expression<double>::cos(expr);
    assert(result.evaluate({}) == 1.0);
    assert(result.toString() == "cos(0)");
}

void testLn() {
    Expression<double> expr(1.0);
    Expression<double> result = Expression<double>::ln(expr);
    assert(result.evaluate({}) == 0.0);
    assert(result.toString() == "ln(1)");
}

void testExp() {
    Expression<double> expr(0.0);
    Expression<double> result = Expression<double>::exp(expr);
    assert(result.evaluate({}) == 1.0);
    assert(result.toString() == "exp(0)");
}

void testSubstitute() {
    Expression<double> expr("x");
    Expression<double> substituted = expr.substitute("x", 5.0);
    assert(substituted.evaluate({}) == 5.0);
    assert(substituted.toString() == "5");
}

int main() {
    testConstant();
    testVariable();
    testAddition();
    testSubtraction();
    testMultiplication();
    testDivision();
    testPower();
    testSin();
    testCos();
    testLn();
    testExp();
    testSubstitute();

    std::cout << "Все тесты пройдены!" << std::endl;
    return 0;
}