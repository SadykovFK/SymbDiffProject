#include "parser.hpp"
#include <cctype>
#include <stdexcept>
#include <vector>

enum class TokenType {
    Number, Variable, Plus, Minus, Mul, Div, Pow,
    LParen, RParen, FuncSin, FuncCos, FuncExp, FuncLn, End
};

struct Token {
    TokenType type;
    std::string text;
};

static std::vector<Token> tokenize(const std::string& str) {
    std::vector<Token> tokens;
    size_t i = 0;
    while (i < str.size()) {
        if (std::isspace(static_cast<unsigned char>(str[i]))) {
            i++;
            continue;
        } else if (std::isdigit(static_cast<unsigned char>(str[i])) || str[i] == '.') {
            size_t start = i;
            while (i < str.size() && (std::isdigit(static_cast<unsigned char>(str[i])) || str[i] == '.')) {
                i++;
            }
            tokens.push_back({TokenType::Number, str.substr(start, i - start)});
        } else if (std::isalpha(static_cast<unsigned char>(str[i]))) {
            size_t start = i;
            while (i < str.size() && std::isalpha(static_cast<unsigned char>(str[i]))) {
                i++;
            }
            std::string word = str.substr(start, i - start);
            if (word == "sin") {
                tokens.push_back({TokenType::FuncSin, word});
            } else if (word == "cos") {
                tokens.push_back({TokenType::FuncCos, word});
            } else if (word == "exp") {
                tokens.push_back({TokenType::FuncExp, word});
            } else if (word == "ln") {
                tokens.push_back({TokenType::FuncLn, word});
            } else {
                tokens.push_back({TokenType::Variable, word});
            }
        } else {
            switch (str[i]) {
                case '+': tokens.push_back({TokenType::Plus, "+"}); break;
                case '-': tokens.push_back({TokenType::Minus, "-"}); break;
                case '*': tokens.push_back({TokenType::Mul, "*"}); break;
                case '/': tokens.push_back({TokenType::Div, "/"}); break;
                case '^': tokens.push_back({TokenType::Pow, "^"}); break;
                case '(': tokens.push_back({TokenType::LParen, "("}); break;
                case ')': tokens.push_back({TokenType::RParen, ")"}); break;
                default:
                    throw std::runtime_error("Неизвестный символ в выражении: " + std::string(1, str[i]));
            }
            i++;
        }
    }
    tokens.push_back({TokenType::End, ""});
    return tokens;
}

static Expression<double> parseExpr(std::vector<Token>& tokens, size_t& pos);
static Expression<double> parseTerm(std::vector<Token>& tokens, size_t& pos);
static Expression<double> parseFactor(std::vector<Token>& tokens, size_t& pos);
static Expression<double> parsePrimary(std::vector<Token>& tokens, size_t& pos);

Expression<double> parseExpression(const std::string& str) {
    auto tokens = tokenize(str);
    size_t pos = 0;
    Expression<double> result = parseExpr(tokens, pos);
    if (tokens[pos].type != TokenType::End) {
        throw std::runtime_error("Лишние токены после парсинга выражения");
    }
    return result;
}

static Expression<double> parseExpr(std::vector<Token>& tokens, size_t& pos) {
    Expression<double> left = parseTerm(tokens, pos);
    while (true) {
        TokenType t = tokens[pos].type;
        if (t == TokenType::Plus) {
            pos++;
            Expression<double> right = parseTerm(tokens, pos);
            left = left + right;
        } else if (t == TokenType::Minus) {
            pos++;
            Expression<double> right = parseTerm(tokens, pos);
            left = left - right;
        } else {
            break;
        }
    }
    return left;
}

static Expression<double> parseTerm(std::vector<Token>& tokens, size_t& pos) {
    Expression<double> left = parseFactor(tokens, pos);
    while (true) {
        TokenType t = tokens[pos].type;
        if (t == TokenType::Mul) {
            pos++;
            Expression<double> right = parseFactor(tokens, pos);
            left = left * right;
        } else if (t == TokenType::Div) {
            pos++;
            Expression<double> right = parseFactor(tokens, pos);
            left = left / right;
        } else {
            break;
        }
    }
    return left;
}

static Expression<double> parseFactor(std::vector<Token>& tokens, size_t& pos) {
    Expression<double> left = parsePrimary(tokens, pos);
    while (tokens[pos].type == TokenType::Pow) {
        pos++;
        Expression<double> right = parsePrimary(tokens, pos);
        left = left ^ right;
    }
    return left;
}

static Expression<double> parsePrimary(std::vector<Token>& tokens, size_t& pos) {
    Token token = tokens[pos];
    if (token.type == TokenType::Number) {
        pos++;
        return Expression<double>(std::stod(token.text));
    } else if (token.type == TokenType::Variable) {
        pos++;
        return Expression<double>(token.text);
    } else if (token.type == TokenType::FuncSin ||
               token.type == TokenType::FuncCos ||
               token.type == TokenType::FuncExp ||
               token.type == TokenType::FuncLn) {
        TokenType funcType = token.type;
        pos++;
        // ожидаем скобку (
        if (tokens[pos].type != TokenType::LParen) {
            throw std::runtime_error("Ожидается '(' после имени функции");
        }
        pos++;
        Expression<double> arg = parseExpr(tokens, pos);
        if (tokens[pos].type != TokenType::RParen) {
            throw std::runtime_error("Ожидается ')' после аргумента функции");
        }
        pos++;

        switch (funcType) {
            case TokenType::FuncSin: return Expression<double>::sin(arg);
            case TokenType::FuncCos: return Expression<double>::cos(arg);
            case TokenType::FuncExp: return Expression<double>::exp(arg);
            case TokenType::FuncLn:  return Expression<double>::ln(arg);
            default: break;
        }
    } else if (token.type == TokenType::LParen) {
        pos++;
        Expression<double> expr = parseExpr(tokens, pos);
        if (tokens[pos].type != TokenType::RParen) {
            throw std::runtime_error("Ожидается ')' в выражении");
        }
        pos++;
        return expr;
    }
    throw std::runtime_error("Некорректный токен при парсинге");
}
