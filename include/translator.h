#include <iostream>
#include "string"
#include <vector>
#include <map>
#include <stack>

using namespace std;

class Translator {
private:
    string infix;
    vector<string> lexem;
    vector<string> postfix;
    map<char, int> priority;

    int is_operator(const char& ch) {
        return ch == '+' || ch == '-' || ch == '*' || ch == '/';
    }

    int is_number(const string& token) {
        return !token.empty() && (isdigit(token[0])
            || (token.size() > 1 && token[0] == '-' && isdigit(token[1])));
    }

    void parse() {
        string token;
        for (char curr : infix) {
            if (curr == ' ') continue;
            else if (isalnum(curr) || isalpha(curr)) token += curr;
            else if (is_operator(curr) || curr == '(' || curr == ')') {
                if (!token.empty()) {
                    lexem.push_back(token);
                    token.clear();
                }
                lexem.push_back(string(1, curr));
            }
            else throw "Unknown symbol";
        }
        if (!token.empty()) lexem.push_back(token);
    }

    double apply_operator(double left, double right, const string& op) {
        if (op == "+") return left + right;
        if (op == "-") return left - right;
        if (op == "*") return left * right;
        if (op == "/") {
            if (right == 0) throw "Division by zero";
            return left / right;
        }
        throw "Unknown operator";
    }

public:
    Translator(string inf) : infix(inf) {
        priority = { {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2} };
    }

    void infix_to_postfix() {
        stack<string> stack;
        parse();
        for (const auto& token : lexem) {
            if (isdigit(token[0]) || isalpha(token[0])) postfix.push_back(token);
            else if (is_operator(token[0])) {
                while (!stack.empty() && priority[stack.top()[0]] >= priority[token[0]]) {
                    postfix.push_back(stack.top());
                    stack.pop();
                }
                stack.push(token);
            }
            else if (token[0] == '(') stack.push("(");
            else if (token[0] == ')') {
                while (!stack.empty() && stack.top() != "(") {
                    postfix.push_back(stack.top());
                    stack.pop();
                }
                stack.pop();
            }
            else if (is_number(stack.top()) && isdigit(token[0])) {
                string curr = stack.top() + token[0];
                stack.pop();
                stack.push(curr);
            }
        }
        while (!stack.empty()) {
            postfix.push_back(stack.top());
            stack.pop();
        }
    }

    double calculate() {
        stack<double> stack;
        for (const auto& token : postfix) {
            if (is_number(token)) stack.push(stod(token));
            else if (is_operator(token[0])) {
                if (stack.size() < 2) throw "Not enough operands";
                double right = stack.top(); stack.pop();
                double left = stack.top(); stack.pop();
                double result = apply_operator(left, right, token);
                stack.push(result);
            }
            else  throw "Invalid token";
        }
        if (stack.size() != 1) throw "Too many operands";
        return stack.top();
    }

    string get_infox() const { return infix; }

    string get_postfix() const {
        string postf;
        for (auto c : postfix) postf += c;
        return postf;
    }
};