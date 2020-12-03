//
//  main.cpp
//  hw1.6 Big Int
//
//  Created by 王宗祥 on 2020/11/27.
//

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <ctime>

using namespace std;
#define Maxdig 256

class BigInt {
public:
    string str;
    int sign, len = 1;
    int num[Maxdig] = { 0 };

    BigInt() {
        len = 1;
        sign = 1;
    }
    BigInt(const int& a) {
        if (a == 0) {
            sign = 1;
            len = 1;
            num[0] = 0;
            return;
        }
        len = 0;
        while ((int)(a / pow(10, len)) != 0) {
            num[len] = (int)(a / pow(10, len)) % 10;
            len++;
        }
        sign = 1;
    };
    BigInt(string str) {
        len = str.size();
        sign = str[0] == '-' ? -1 : 1;
        if (sign == -1) {
            str.erase(0, 1);
        }
        for (int i = 0; i < len; i++) {
            num[i] = str[len - i - 1] - '0';
        }
    };

    bool operator>(const BigInt& x) const;
    bool operator<(const BigInt& x) const;
    bool operator!=(const BigInt& x) const;
    bool operator>=(const BigInt& x) const;
    bool operator==(const BigInt& x) const;
    bool operator==(const int& a) const;
    bool operator!=(const int& a) const;

    BigInt operator-() const;

    BigInt operator+(const BigInt& x) const;
    BigInt operator-(const BigInt& x) const;
    BigInt operator*(const BigInt& x) const;
    BigInt operator*(const int& a) const;
    BigInt operator/(const int& a) const;
    BigInt operator%(const int& a) const;

    void print();
};

bool BigInt::operator>(const BigInt& x) const{
    if (sign != x.sign) { return sign > x.sign; }                // •˝ßP¬_•ø≠t∏π
    if (len != x.len) { return len * sign > x.len * x.sign; }    // ¶P∏πÆ…°A¶AßP¬_§j§p
    for (int i = len - 1; i >= 0; i--) {
        if (num[i] != x.num[i]) { return num[i]*sign > x.num[i]*x.sign; }
    }
    return false;
}

bool BigInt::operator<(const BigInt& x) const {
    return x > *this;
}

bool BigInt::operator!=(const BigInt& x) const{
    return *this > x || x > * this;
}

bool BigInt::operator==(const BigInt& x) const {
    return !(*this > x) && !(x > * this);
}

bool BigInt::operator>=(const BigInt& x) const {
    return !(*this < x);
}

bool BigInt::operator==(const int& a) const {
    return num[0] % 2 == a ? true : false;
}

bool BigInt::operator!=(const int& a) const {
    BigInt c(a);
    return *this != c ;
}

BigInt BigInt::operator-() const {
    BigInt temp = *this;
    temp.sign = -sign;
    return temp;
}

BigInt BigInt::operator+(const BigInt& x) const {
    BigInt temp;
    if (sign == x.sign) {
        int shift = 0;
        temp.sign = sign;
        for (int i = 0; i < len; i++) {
            if ((num[i] + shift) + x.num[i] < 10) {
                temp.num[i] = (num[i] + shift) + x.num[i];
                shift = 0;
            }
            else {
                temp.num[i] = (num[i] - shift) - x.num[i] - 10;
                shift = 1;
            }
        }
        return temp;
    }
    return *this - (-x);
}

BigInt BigInt::operator-(const BigInt& x) const {
    BigInt temp;
    if (sign == x.sign) {
        int shift = 0;
        if (*this >= x) {
            temp.sign = 1;
            for (int i = 0; i < len; i++) {
                if ((num[i] - shift) - x.num[i] >= 0) {
                    temp.num[i] = (num[i] - shift) - x.num[i];
                    shift = 0;
                }
                else {
                    temp.num[i] = 10 + (num[i] - shift) - x.num[i];
                    shift = 1;
                }
                if (temp.num[i]!=0) {
                    temp.len = i + 1;
                }
            }
            return temp;
        }
        return -(x - *this);
    }
    return *this + (-x);
}

BigInt BigInt::operator*(const BigInt& x) const {
    BigInt ans;
    ans.sign = sign * x.sign;
    for (int i = 0; i < x.len; i++) {
        int shift = 0;
        int temp;
        for (int j = 0; j < len; j++) {
            temp = num[j] * x.num[i] + shift;
            shift = temp / 10;
            ans.num[i + j] += temp % 10;
            if (ans.num[i + j] >= 10) {
                ans.num[i + j + 1] += ans.num[i + j] / 10;
                ans.num[i + j] = ans.num[i + j] % 10;
            }
        }
        if (shift != 0) {
            ans.num[len + i] += shift;
        }
    }
    int n = 1;
    for (int i = 0; i < Maxdig; i++) {
        if (ans.num[i] != 0) {
            n = i + 1;
        }
    }
    ans.len = n;
    return ans;
}

BigInt BigInt::operator*(const int& a) const{
    BigInt c(a);
    return *this * c;
}

BigInt BigInt::operator/(const int& a) const{
    BigInt final ,temp;
    int shift = 0;
    int j = 0;
    for (int i = len - 1; i >= 0; i--) {
        int tt = num[i] + shift * 10;
        temp.num[j] = tt / a;
        shift = tt % a == 0 ? 0 : tt % a;
        j++;
    }
    if (num[len - 1] < a) {
        final.len = len - 1;
    }
    else {
        final.len = len;
    }
    for (int i = 0; i < final.len; i++) {
        final.num[i] = temp.num[len - 1 - i];
    }
    return final;
}

BigInt BigInt::operator%(const int& a) const{
    BigInt res;
    res.num[0] = num[0] % 2;
    return res;
}

void BigInt::print() {
    if (this->sign == -1) {
        cout << "-";
    }
    for (int i = len - 1; i >= 0; i--) {
        cout << num[i];
    }
    cout << endl;
}

int main() {
    string str1, str2;
    cout << "Enter two BigInts a & b." << endl;
    cin >> str1 >> str2;
    
    BigInt num1(str1);
    BigInt num2(str2);
    
    // GCD_By_Binary
    BigInt max = num1 > num2 ? num1 : num2;
    BigInt min = num1 < num2 ? num1 : num2;
    BigInt ans(1);

    
    while (max != 0 && min != 0) {
        if (max % 2 == 0 && min % 2 == 0) {
            ans = ans * 2;
            max = max / 2;
            min = min / 2;
        }
        else if (max % 2 == 0 && min % 2 == 1) {
            max = max / 2;
        }
        else if (max % 2 == 1 && min % 2 == 0) {
            min = min / 2;
        }

        if (min > max) {
            swap(min, max);
        }
        max = max - min;
    }
    
    ans = ans * min;
    ans.print();
    
    // cout << (double)clock() / CLOCKS_PER_SEC << " S";
    return 0;
}
