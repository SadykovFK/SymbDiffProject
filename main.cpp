#include <iostream>
#include <string>
#include <map>
#include "expression.hpp"
#include "parser.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Использование:\n"
                  << "  differentiator --eval \"expr\" x=10 y=12 ...\n"
                  << "  differentiator --diff \"expr\" --by x\n";
        return 0;
    }

    std::string mode = argv[1];
    if (mode == "--eval") {
        if (argc < 3) {
            std::cerr << "Ошибка: не передано выражение для --eval\n";
            return 1;
        }
        std::string expressionStr = argv[2];
        Expression<double> expr = parseExpression(expressionStr);

        std::map<std::string, double> variables;
        for (int i = 3; i < argc; ++i) {
            std::string arg = argv[i];
            auto posEq = arg.find('=');
            if (posEq != std::string::npos) {
                std::string varName = arg.substr(0, posEq);
                double val = std::stod(arg.substr(posEq + 1));
                variables[varName] = val;
            }
        }

        try {
            double result = expr.evaluate(variables);
            std::cout << result << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Ошибка вычисления: " << e.what() << std::endl;
            return 1;
        }

    } else if (mode == "--diff") {
        if (argc < 3) {
            std::cerr << "Ошибка: не передано выражение для --diff\n";
            return 1;
        }
        std::string expressionStr = argv[2];
        std::string diffVar;
        for (int i = 3; i < argc; ++i) {
            if (std::string(argv[i]) == "--by" && i + 1 < argc) {
                diffVar = argv[i + 1];
                break;
            }
        }
        if (diffVar.empty()) {
            std::cerr << "Ошибка: не указана переменная после --by\n";
            return 1;
        }
е
        Expression<double> expr = parseExpression(expressionStr);
        Expression<double> diffExpr = expr.derivative(diffVar);
        std::cout << diffExpr.toString() << std::endl;

    } else {
        std::cerr << "Неизвестный режим: " << mode << std::endl;
        return 1;
    }

    return 0;
}
