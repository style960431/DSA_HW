//
//  main.cpp
//  hw3.1
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
        a == "/" || a == "<" || a == ">" || a == "^" || a == "!" ||
        a == "=" || a == "&" || a == "|" || a == "~" || a == "%" ||
        a == "<<" || a == ">>" || a == "&&" || a == "||" ||
        a == "!=" || a == "==") {
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
            vec.push_back(temp_1);
            if (i > 0) {
                string temp_2(1, str[i - 1]);
                if (isoperator(temp_2) && compare_operator(temp_2, temp_1)) {
                    vec.pop_back();
                    vec.pop_back();
                    vec.push_back(temp_2 + temp_1);
                }
            }
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
    if (temp[i] == "!" || temp[i] == "~") { oper_t op(temp[i], 2); return op; }
    else if (temp[i] == "+" && i == 0) { oper_t op(temp[i], 2); return op; }
    else if (temp[i] == "-" && i == 0) { oper_t op(temp[i], 2); return op; }
    else if (temp[i] == "+" && isoperator(temp[i - 1]) && temp[i - 1] != ")") {
        oper_t op(temp[i], 2);
        return op;
    }
    else if (temp[i] == "-" && isoperator(temp[i - 1]) && temp[i - 1] != ")") {
        oper_t op(temp[i], 2);
        return op;
    }
    else if (temp[i] == "*" || temp[i] == "/" || temp[i] == "%") { oper_t op(temp[i], 3); return op; }
    else if (temp[i] == "+" || temp[i] == "-") { oper_t op(temp[i], 4); return op; }
    else if (temp[i] == ">>") { oper_t op(temp[i], 5); return op; }
    else if (temp[i] == "<<") { oper_t op(temp[i], 5); return op; }
    else if (temp[i] == "==") { oper_t op(temp[i], 6); return op; }
    else if (temp[i] == "!=") { oper_t op(temp[i], 6); return op; }
    else if (temp[i] == "&") { oper_t op(temp[i], 7); return op; }
    else if (temp[i] == "^") { oper_t op(temp[i], 8); return op; }
    else if (temp[i] == "|") { oper_t op(temp[i], 9); return op; }
    else if (temp[i] == "&&") { oper_t op(temp[i], 10); return op; }
    else if (temp[i] == "||") { oper_t op(temp[i], 11); return op; }
}

int eval(const int& a, const int& b, const string& oper_t) {
    if (oper_t == "+") { return a + b; }
    else if (oper_t == "-") { return a - b; }
    else if (oper_t == "*") { return a * b; }
    else if (oper_t == "/") { return a / b; }
    else if (oper_t == "%") { return a % b; }
    else if (oper_t == "+") { return a + b; }
    else if (oper_t == "&") { return (a & b); }
    else if (oper_t == "^") { return (a ^ b); }
    else if (oper_t == "|") { return (a | b); }
    else if (oper_t == "<<") { return (a << b); }
    else if (oper_t == ">>") { return (a >> b); }
    else if (oper_t == "&&") { return (a && b); }
    else if (oper_t == "||") { return (a || b); }
    else if (oper_t == "==") { return (a == b); }
    else if (oper_t == "!=") { return (a != b); }
}

int eval(const int& a, const string& oper_t) {
    if (oper_t == "~") { return (~a); }
    else if (oper_t == "-") { return (-a); }
    else if (oper_t == "+") { return (+a); }
    else if (oper_t == "!") { return (!a); }
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
    cout << "The postfix form is :\t";

    for (int i = 0; i < v_temp.size(); i++) {
        cout << v_temp[i].sym << " ";
    }
    cout << endl;

    return v_temp;
}

int Postfix_value(vector<oper_t>& v) {
    stack<int> oper_a;
    vector<oper_t>::iterator it;
    for (int i = 0; i < v.size(); i++) {
        if (!isoperator(v[i].sym)) {
            stringstream val(v[i].sym);
            int num = 0;
            val >> num;
            oper_a.push(num);
        }
        else {
            int a = oper_a.top();
            oper_a.pop();
            if (v[i].pr == 2) {
                int c = eval(a, v[i].sym);
                oper_a.push(c);
            }
            else {
                int b = oper_a.top();
                oper_a.pop();
                int c = eval(b, a, v[i].sym);
                oper_a.push(c);

            }
        }
    }
    return oper_a.top();
}

int main() {
    vector<oper_t> v;
    vector<string>::iterator it;
    string str;
    int value;
    ifstream in("..//hw3_1_testcases//3_2.in");
    getline(in, str);
    in.close();
    v = InfixToPostfix(str);
    value = Postfix_value(v);
    cout << value << endl;
    return 0;
}

