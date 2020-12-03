//
//  main.cpp
//  hw2.3.2
//
//  Created by 王宗祥 on 2020/11/27.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <ctime>
using namespace std;

class Data {
    /*********************************************
        t : click_timestamp;
        sale : Indicates 1 if conversion occurred and 0 if not.
        u : user_id
        p : product_id
        p1,p2 : 某個user_id會出現同時含有兩個不同product_id
        theta : average sales amount per click
        total number of sales : 出現sale=1的總行數
    *********************************************/
public:
    string sale, t, price, age_group, gender, u, p;
    Data() = default;
    Data(string sale, string t, string price, string age_group, string gender, string u, string p) {
        this->sale = sale;
        this->t = t;
        this->price = price;
        this->age_group = age_group;
        this->gender = gender;
        this->u = u;
        this->p = p;
    };
};

map<int, Data> split_big_data(ifstream& in, map<int, Data>& person) {
    string str;
    int num = 0;
    while (getline(in, str, '\n')) {
        vector<string> vec;
        stringstream ss(str);
        while (getline(ss, str, '\t')) {
            vec.push_back(str);
        }
        string sale = vec[0];
        string t = vec[3];
        string price = vec[5];
        string age_group = vec[6];
        string gender = vec[9];
        string p = vec[19];
        string u = vec[22];
        vec.clear();
        person.insert(pair <int, Data >(num, Data(sale, t, price, age_group, gender, u, p)));
        num++;
    }
    return person;
}

void get(const vector<string>& method, int& i, map<int, Data>& person) {
    string u = method[i + 1];
    string p = method[i + 2];
    string t = method[i + 3];
    map<int, Data>::iterator it;
    cout << "********************" << endl;
    for (it = person.begin(); it != person.end(); it++) {
        if (it->second.u == u && it->second.p == p && it->second.t == t) {
            cout << it->second.sale << endl;
            return;
        }
    }
    cout << "Do not Find" << endl;
    cout << "********************" << endl;
    return;
}

void purchased(const string& u, map<int, Data>& person) {
    map<int, Data>::iterator it;
    cout << "********************" << endl;
    for (it = person.begin(); it != person.end(); it++) {
        if (u == it->second.u) {
            cout << it->second.p << " " << it->second.t << " " << it->second.price << " " << it->second.age_group << " " << it->second.gender << endl;
        }
    }
    cout << "********************" << endl;
    return;
}

void clicked(const string& p1, const string& p2, map<int, Data>& person) {
    map<int, Data>::iterator it_1;
    cout << "********************" << endl;
    for (it_1 = person.begin(); it_1 != person.end(); it_1++) {
        if (p1 == it_1->second.p) {
            string u_temp = it_1->second.u;
            map<int, Data>::iterator it_2;
            for (it_2 = it_1; it_2 != person.end(); it_2++) {
                if (p2 == it_2->second.p && u_temp == it_2->second.u) {
                    cout << it_2->second.u << endl;
                    break;
                }
            }
        }
    }
    cout << "********************" << endl;
    return;
}

void profit(const string& t, double& theta, map<int, Data>& person) {
    struct input {
        int num = 0;
        double total_sale = 0.0;
        input() = default;
        input(int num, double total_slae) {
            this->num = num;
            this->total_sale = total_sale;
        };
    };
    map<string, input> ans;

    map<int, Data>::iterator it_1;
    cout << "********************" << endl;
    for (it_1 = person.begin(); it_1 != person.end(); it_1++) {
        if (it_1->second.t >= t) {
            int num = 1;
            double total_sale = it_1->second.sale == "1" ? 1 : 0;
            if (ans.find(it_1->second.u) != ans.end()) {
                ans[it_1->second.u].num ++;
                ans[it_1->second.u].total_sale += total_sale;
            }
            else {
                ans.insert(pair<string, input>(it_1->second.u, input(1, total_sale)));
            }
        }
    }
    map<string, input>::iterator it_2;
    for (it_2 = ans.begin(); it_2 != ans.end(); it_2++) {
        if (it_2->second.total_sale / it_2->second.num >= theta) {
            cout << it_2->first << endl;
        }
    }
    cout << "********************" << endl;
    return;
}

int main() {

    cout << "Wait a moment..." << endl;
    ifstream in("data2.txt");
    map<int, Data> person;
    split_big_data(in, person);
    in.close();
    cout << "Enter the data." << endl;

    map<int, Data>::iterator it;
    vector<string> method;
    string temp;
    while (cin >> temp) {
        if (temp == "quit") {
            break;
        }
        method.push_back(temp);
    }

    double theta;
    for (int i = 0; i < method.size(); i++) {
        if (method[i] == "get") {
            get(method, i, person);
            i += 3;
        }
        else if (method[i] == "purchased") {
            string u = method[i + 1];
            purchased(u, person);
            i += 1;
        }
        else if (method[i] == "clicked") {
            string p1 = method[i + 1];
            string p2 = method[i + 2];
            clicked(p1, p2, person);
            i += 2;
        }
        else if (method[i] == "profit") {
            string t = method[i + 1];
            theta = atof(method[i + 2].c_str());
            profit(t, theta, person);
            i += 2;
        }
    }
    
    return 0;
}
