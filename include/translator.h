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
    int is_operator(const string& ch) {
        return ch == "+" || ch == "-" || ch == "*" || ch == "/";
    }

    int is_number(const string& token) {
        return !token.empty() && (isdigit(token[0])
            || (token.size() > 1 && token[0] == '-' && isdigit(token[1])));
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

    void input(const string& numb, const size_t index) {
        string result;
        do {
            cout << "Input " + numb + ": ";
            cin >> result;
        } while (!is_number(result));
        postfix[index] = result;
    }

    void error_checker() {
        int state = 0;
        int bracketCount = 0;
        int operatorCount = 0;
        for (int i = 0; i < infix.length(); i++) {
            if (infix[i] == ' ') continue;
            if (state == 0) {
                if (isdigit(infix[i]) || isalpha(infix[i]))  state = 1;
                else if (infix[i] == '(') { bracketCount++; state = 2; }
                else throw string("Incorrect expression");
            }
            else if (state == 1) {                
                if (isdigit(infix[i]) || isalpha(infix[i]))  state = 1;
                else if (infix[i] == '.') state = 3;
                else if (infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/')
                { operatorCount++; state = 2; }
                else if (infix[i] == ')') { bracketCount--; state = 1; }
                else string("Incorrect expression");
            }
            else if (state == 2) {
                if (isdigit(infix[i]) || isalpha(infix[i])) state = 1;
                else if (infix[i] == '.') state = 3;
                else if (infix[i] == '(') { bracketCount++; state = 2; }
                else throw string("Incorrect expression");
            }
            else if (state == 3) {
                if (isdigit(infix[i]) || isalpha(infix[i]))  state = 1;
                else throw string("Incorrect expression"); 
            }
        }
        if (!(state == 1 && bracketCount == 0 && operatorCount > 0)) throw string("Incorrect expression");
    }

     void parse() {
        string token;
        for (char curr : infix) {
            if (curr == ' ') continue;
            else if (isalnum(curr) || isalpha(curr) || curr == '.') token += curr;
            else if (is_operator(string(1,curr)) || curr == '(' || curr == ')') {
                if (!token.empty()) {
                    lexem.push_back(token);
                    token.clear();
                }
                lexem.push_back(string(1, curr));
            }        
        }
        if (!token.empty()) lexem.push_back(token);       
    }

     void infix_to_postfix() {
         error_checker();
         parse();
         stack<string> stack;         
         for (const auto& token : lexem) {
             if (isdigit(token[0]) || isalpha(token[0])) postfix.push_back(token);
             else if (is_operator(token)) {
                 while (!stack.empty() && priority[stack.top()[0]] >= priority[token[0]]) {
                     postfix.push_back(stack.top());
                     stack.pop();
                 }
                 stack.push(token);
             }
             else if (token == "(") stack.push("(");
             else if (token == ")") {
                 while (!stack.empty() && stack.top() != "(") {
                     postfix.push_back(stack.top());
                     stack.pop();
                 }
                 stack.pop();
             }
         }
         while (!stack.empty()) {
             postfix.push_back(stack.top());
             stack.pop();
         }
     }

public:
    Translator(string inf) : infix(inf) {
        priority = { {'+', 1}, {'-', 1}, {'*', 2}, {'/', 2} };
    }    

    double calculate() {
        infix_to_postfix();
        for (int i = 0; i < postfix.size(); i++)
            if (!is_number(postfix[i]) && !is_operator(postfix[i]) && postfix[i] != "(" && postfix[i] != ")") input(postfix[i], i);        
        stack<double> stack;
        for (const auto& token : postfix) {
            if (is_number(token)) stack.push(stod(token));          
            else if (is_operator(token)) {
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

    string get_infix() const { return infix; }

    string get_postfix() {
        infix_to_postfix();
        string postf;
        for (auto c : postfix) postf += c;
        return postf;
    }
};
template<typename = void>
void Begin() {
    while (1) {        
        cout << "Input arithmetic expression: ";
        string s;
        getline(cin, s);
        Translator trans(s);
        double result;
        try
        {
            result = trans.calculate();
            cout << "Result: " << result << endl;
        }
        catch (string error)
        {
            cout << error << endl;
        }
    }
}
