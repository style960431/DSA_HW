//
//  main.cpp
//  hw3.2
//
//  Created by 王宗祥 on 2020/11/27.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <sstream>
#include <cmath>
using namespace std;
#define str_max 1000000

class oper_t {
public:
    string sym;
    int pr;
    oper_t(const string& str) : sym(str), pr(0) {};
    oper_t(const string& str, const int& v) : sym(str), pr(v) {};
    void operator=(const oper_t& n) {
        sym = n.sym;
        pr = n.pr;
        return;
    }
    bool operator>(const oper_t& n) const { return pr > n.pr; }
    bool operator<(const oper_t& n) const { return pr < n.pr; }
    bool operator>=(const oper_t& n) const { return !(*this < n); }
    bool operator<=(const oper_t& n) const { return !(*this > n); }
    bool operator==(const oper_t& n) const { return pr == n.pr; }
};

bool isoperator(string& a) {
    if (a == "(" || a == ")" || a == "+" || a == "-" || a == "*" ||
        a == "/" || a == "s" || a == "c" || a == "e" || a == "l" ||
        a == "p" || a == "f" ||    a == "," || a == "sin" || a == "cos" ||
        a == "exp" || a == "log" || a == "pow" || a == "sqrt" || a == "fabs") {
        return true;
    }
    return false;
}

bool compare_operator(const string& a, const string& b) {
    if ((a == "&" && b == "&") || (a == "|" && b == "|") ||
        (a == "<" && b == "<") || (a == ">" && b == ">") ||
        (a == "=" && b == "=") || (a == "!" && b == "=")) {
        return true;
    }
    return false;
}

void convert(vector<string>& vec, string str) {
    for (int i = 0; i < str.size(); i++) {
        string temp_1(1, str[i]);
        if (isoperator(temp_1)) {
            if (str.substr(i, 3) == "sin") { i += 2; vec.push_back("sin"); }
            else if (str.substr(i, 3) == "cos") { i += 2; vec.push_back("cos"); }
            else if (str.substr(i, 3) == "exp") { i += 2; vec.push_back("exp"); }
            else if (str.substr(i, 3) == "log") { i += 2; vec.push_back("log"); }
            else if (str.substr(i, 3) == "pow") { i += 2; vec.push_back("pow"); }
            else if (str.substr(i, 4) == "sqrt") { i += 3; vec.push_back("sqrt"); }
            else if (str.substr(i, 4) == "fabs") { i += 3; vec.push_back("fabs"); }
            else if (str.substr(i, 1) == ",") { vec.push_back(","); }
            else { vec.push_back(temp_1); }
        }
        else {
            vec.push_back(temp_1);
            if (i > 0) {
                string temp_2(1, str[i - 1]);
                if (!isoperator(temp_2)) {
                    string op1 = vec.back();
                    vec.pop_back();
                    string op2 = vec.back();
                    vec.pop_back();
                    vec.push_back(op2 + op1);
                }
            }
        }
    }
    return;
}

oper_t preced(vector<string>& temp, int& i) {
    if (temp[i] == "sin" || temp[i] == "cos" || temp[i] == "log" ||
        temp[i] == "exp" || temp[i] == "pow" || temp[i] == "sqrt" ||
        temp[i] == "fabs") {
        oper_t op(temp[i], 1);
        return op;
    }
    else if (temp[i] == "+" && (i == 0 || temp[i - 1] == "(")) { oper_t op(temp[i], 2); return op; }
    else if (temp[i] == "-" && (i == 0 || temp[i - 1] == "(")) { oper_t op(temp[i], 2); return op; }
    else if (temp[i] == "+" && isoperator(temp[i - 1]) && temp[i - 1] != ")") {
        oper_t op(temp[i], 2);
        return op;
    }
    else if (temp[i] == "-" && isoperator(temp[i - 1]) && temp[i - 1] != ")") {
        oper_t op(temp[i], 2);
        return op;
    }
    else if (temp[i] == "*") { oper_t op(temp[i], 3); return op; }
    else if (temp[i] == "+" || temp[i] == "-") { oper_t op(temp[i], 4); return op; }
}

double eval(const double& a, const double& b, const string& oper_t) {
    if (oper_t == "+") { return a + b; }
    else if (oper_t == "-") { return a - b; }
    else if (oper_t == "*") { return a * b; }
    else if (oper_t == "pow") { return pow(a, b); }
}

double eval(const double& a, const string& oper_t) {
    if (oper_t == "-") { return (-a); }
    else if (oper_t == "+") { return (+a); }
    else if (oper_t == "sin") { return sin(a); }
    else if (oper_t == "cos") { return cos(a); }
    else if (oper_t == "exp") { return exp(a); }
    else if (oper_t == "log") { return log(a); }
    else if (oper_t == "sqrt") { return sqrt(a); }
    else if (oper_t == "fabs") { return fabs(a); }
}

vector<oper_t> InfixToPostfix(string& str) {
    vector<string> temp;
    stack<oper_t> op_temp;
    vector<oper_t> v_temp;
    vector<oper_t>::iterator it;

    convert(temp, str);
    for (int i = 0; i < temp.size(); i++) {
        cout << "Read°G\t" << temp[i] << endl;
        if (isoperator(temp[i])) {
            if (temp[i] == "(") {
                oper_t o(temp[i], 100);
                op_temp.push(o);
            }
            else if (temp[i] == ")") {
                cout << "Pop°G\t";
                while (!op_temp.empty() && op_temp.top().sym != "(") {
                    v_temp.push_back(op_temp.top());
                    cout << op_temp.top().sym << ", ";
                    op_temp.pop();
                }
                cout << endl << endl;
                op_temp.pop();
            }
            else if (temp[i] == ",") {
                cout << "POP:\t";
                while (!op_temp.empty() && op_temp.top().sym != "(") {
                    v_temp.push_back(op_temp.top());
                    cout << op_temp.top().sym << ", ";
                    op_temp.pop();
                }
                cout << endl;
            }
            else {
                oper_t opr = preced(temp, i);
                if (!op_temp.empty() && opr.pr == 2 && op_temp.top().pr == 2) {
                    op_temp.push(opr);
                    cout << "Push°G\t" << opr.sym << endl;
                    cout << endl;
                }
                else if (!op_temp.empty() && opr.pr < op_temp.top().pr) {
                    op_temp.push(opr);
                    cout << "Push°G\t" << opr.sym << endl;
                    cout << endl;
                }
                else {
                    if (!op_temp.empty()) {
                        cout << "Pop°G\t";
                        while (!op_temp.empty() && opr.pr >= op_temp.top().pr) {
                            v_temp.push_back(op_temp.top());
                            cout << op_temp.top().sym << ", ";
                            op_temp.pop();
                        }
                        cout << endl;
                    }
                    op_temp.push(opr);
                    cout << "Push°G\t" << opr.sym << endl;
                    cout << endl;
                }
            }
        }
        else {
            oper_t o(temp[i]);
            v_temp.push_back(o);
            cout << endl;
        }
    }
    cout << "Pop°G\t";
    while (!op_temp.empty()) {
        v_temp.push_back(op_temp.top());
        cout << op_temp.top().sym << ", ";
        op_temp.pop();
    }
    cout << endl << endl;
    cout << "The postfix form =\t";

    for (int i = 0; i < v_temp.size(); i++) {
        cout << v_temp[i].sym << " ";
    }
    cout << endl;
    return v_temp;
}

double Postfix_value(vector<oper_t>& v) {
    stack<double> oper_a;
    vector<oper_t>::iterator it;
    for (int i = 0; i < v.size(); i++) {
        if (!isoperator(v[i].sym)) {
            double num = stod(v[i].sym);
            oper_a.push(num);
        }
        else {
            double a = oper_a.top();
            oper_a.pop();
            if (v[i].sym == "pow") {
                double b = oper_a.top();
                oper_a.pop();
                oper_a.push(eval(b, a, v[i].sym));
            }
            else if (v[i].pr == 1 || v[i].pr == 2) {
                oper_a.push(eval(a, v[i].sym));
            }
            else {
                double b = oper_a.top();
                oper_a.pop();
                oper_a.push(eval(b, a, v[i].sym));
            }
        }
    }
    double ans = round(oper_a.top() * pow(10, 6)) / pow(10, 6);
    if (fabs(ans) < 1E-7) ans = 0;
    return oper_a.top();
}

int main() {
    vector<oper_t> v;
    vector<string> v1;

    vector<string>::iterator it;
    string str;
    double value;
    ifstream in("..//hw3_2_testcases//2_1.in");
    getline(in, str);
    in.close();
    v = InfixToPostfix(str);
    value = Postfix_value(v);
    printf("The value =\t%f\n", value);
    
    return 0;
}

