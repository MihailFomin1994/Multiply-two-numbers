/* Multiply two numbers!Simple!

Rules
1. The arguments are passed as strings.
2. The numbers will be very large
3. The arguments can be negative, in decimals, and might have leadingand trailing zeros.e.g. "-01.300"
4. Answer should be returned as string
5. The returned answer should not have leading or trailing zeroes(when applicable) e.g. "0123" and "1.100" are wrong, they should be "123" and "1.1"
6. Zero should not be signedand "-0.0" should be simply returned as "0".
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <iterator>

/*      BASE VARIANLES      */
int BASE = 10;
size_t LEN_F_NAIVE = 4;
const char RAZDELITEL = '.';

/*      SUPPORT FUNCTIONS      */
void print_v(const std::vector<int>& v)
{
    for (auto& s : v)
    {
        std::cout << s << " ";
    }
    std::cout << std::endl;
}

unsigned int pow2(unsigned int exp) {
    if (exp == 0) return 1;
    unsigned int result = 2;
    for (unsigned int i = 1; i < exp; i++) {
        result *= 2;
    }
    return result;
}

/*   KARATSUBA MULTIPLY   */
void remove_insignificant_zeroes(std::string& number) {
    auto it_point = std::find(number.begin(), number.end(), RAZDELITEL);
    std::string number_before_point{ number.begin(), it_point };
    std::string number_after_point;

    auto it_before = number_before_point.begin();
    for (; it_before != number_before_point.end(); it_before++) {
        if (*it_before != '0') break;
    }
    number_before_point.erase(number_before_point.begin(), it_before);
    if (number_before_point.size() == 0) number_before_point = "0";

    if (it_point != number.end()) {
        number_after_point.insert(number_after_point.end(), it_point + 1, number.end());
        auto it_after = std::prev(number_after_point.end());
        for (; it_after != number_after_point.begin(); it_after--) {
            if (*it_after != '0') break;
        }
        number_after_point.erase(it_after + 1, number_after_point.end());
        if (number_after_point.size() == 1 && number_after_point.at(0) == '0') number_after_point.clear();
    }
    number = number_before_point;
    if (number_after_point.size() != 0) {
        number += RAZDELITEL + number_after_point;
    }
}

unsigned int get_numberes(std::string& X, std::string& Y,
    std::vector<int>& vX, std::vector<int>& vY) {
    unsigned int point_dist = 0;

    auto it_point_X = std::find(X.begin(), X.end(), RAZDELITEL);
    auto it_point_Y = std::find(Y.begin(), Y.end(), RAZDELITEL);

    if (!(it_point_X == X.end() && it_point_Y == Y.end())) {

        if (it_point_X != X.end() && it_point_Y == Y.end())
        {
            point_dist = 2 * std::distance(it_point_X, X.end()) - 2;
        }
        if (it_point_X == X.end() && it_point_Y != Y.end())
        {
            point_dist = 2 * std::distance(it_point_Y, Y.end()) - 2;
        }
        if (it_point_X != X.end() && it_point_Y != Y.end()) {
            point_dist = 2 * std::max(std::distance(it_point_X, X.end()), std::distance(it_point_Y, Y.end())) - 2;
        }

        if (it_point_X == X.end()) X.push_back('.0');
        if (it_point_Y == Y.end()) Y.push_back('.0');

        int X_point_dist = std::distance(it_point_X, X.end());
        int Y_point_dist = std::distance(it_point_Y, Y.end());
        int dif = X_point_dist - Y_point_dist;
        if (dif > 0)
        {
            for (int i = 0; i < dif; i++) {
                Y.push_back('0');
            }
        }
        if (dif < 0) {
            dif *= -1;
            for (int i = 0; i < dif; i++) {
                X.push_back('0');
            }
        }
        X.erase(it_point_X);
        Y.erase(it_point_Y);
    }
    else {
        point_dist = 0;
    }
    for (auto it = X.crbegin(); it != X.crend(); it++) {
        vX.push_back(*it - '0');
    }

    for (auto it = Y.crbegin(); it != Y.crend(); it++) {
        vY.push_back(*it - '0');
    }

    unsigned int len = std::max(X.length(), Y.length());
    unsigned int tmp = len, k = 0;
    while (tmp > 1)
    {
        tmp = tmp / 2;
        ++k;
    }
    len = pow2(k + 1);

    vX.resize(len);
    vY.resize(len);

    return point_dist;
}

std::vector<int> naive_mul(const std::vector<int>& vX, const std::vector<int>& vY)
{
    auto len = vX.size();
    std::vector<int> result(2 * len);

    for (unsigned int i = 0; i < len; i++)
    {
        for (unsigned int j = 0; j < len; j++)
        {
            result[i + j] += vX[i] * vY[j];
        }
    }
    return result;
}

std::vector<int> karatsuba_mul(const std::vector<int>& vX, const std::vector<int>& vY)
{
    auto len = vX.size();
    std::vector<int> result(2 * len);

    if (len <= LEN_F_NAIVE) {
        return naive_mul(vX, vY);
    }

    auto k = len / 2;

    std::vector<int> Xr{ vX.begin(), vX.begin() + k };
    std::vector<int> Xl{ vX.begin() + k, vX.end() };
    std::vector<int> Yr{ vY.begin(), vY.begin() + k };
    std::vector<int> Yl{ vY.begin() + k, vY.end() };

    std::vector<int> P1 = karatsuba_mul(Xl, Yl);
    std::vector<int> P2 = karatsuba_mul(Xr, Yr);

    std::vector<int> Xlr(k);
    std::vector<int> Ylr(k);

    for (unsigned int i = 0; i < k; ++i) {
        Xlr[i] = Xl[i] + Xr[i];
        Ylr[i] = Yl[i] + Yr[i];
    }

    std::vector<int> P3 = karatsuba_mul(Xlr, Ylr);

    for (unsigned int i = 0; i < len; ++i) {
        P3[i] -= P2[i] + P1[i];
    }

    for (unsigned int i = 0; i < len; i++) {
        result[i] = P2[i];
    }

    for (unsigned int i = len; i < 2 * len; i++) {
        result[i] = P1[i - len];
    }

    for (unsigned int i = k; i < len + k; i++) {
        result[i] += P3[i - k];
    }

    return result;
}

void finalize(std::vector<int>& result) {
    for (unsigned int i = 0; i < result.size() - 1; i++) {
        result[i + 1] += result[i] / BASE;
        result[i] %= BASE;
    }
}

std::string get_result(std::vector<int> result) {
    std::string sresult = "";
    for (auto it = result.crbegin(); it != result.crend(); it++)
    {
        sresult.push_back(*it + '0');
    }
    return sresult;
}

std::string multiply(std::string X, std::string Y) {
    bool check_minus_X = false, check_minus_Y = false;
    if (X.at(0) == '-') {
        X.at(0) = '0';
        check_minus_X = true;
    }
    if (Y.at(0) == '-') {
        Y.at(0) = '0';
        check_minus_Y = true;
    }

    remove_insignificant_zeroes(X);
    remove_insignificant_zeroes(Y);

    if (X == "0" || Y == "0") return "0";

    std::vector<int> vX, vY, resultV;
    unsigned int point_dist = get_numberes(X, Y, vX, vY);
    
    resultV = karatsuba_mul(vX, vY);
    finalize(resultV);

    std::string result = get_result(resultV);
    if (point_dist != 0) result.insert(result.end() - point_dist, RAZDELITEL);
    
    remove_insignificant_zeroes(result);
    
    if ((check_minus_X == true && check_minus_Y == false) || (check_minus_X == false && check_minus_Y == true)) {
        result.insert(result.begin(), '-');
    }
    return result;
}

void testing_mul(const std::string X, const std::string Y, const std::string res) {
    std::string test_res = multiply(X, Y);
    if (test_res == res) {
        std::cout << "OK" << std::endl;
        std::cout << X << " * " << Y << " = " << test_res << std::endl;
    }
    else
    {
        std::cout << "ERROR" << std::endl;
        std::cout << X << " * " << Y << " != " << test_res << std::endl;
    }
    std::cout << std::endl;
}

void Test()
{
    testing_mul("2", "3", "6");
    testing_mul("30", "69", "2070");
    testing_mul("11", "85", "935");

    testing_mul("-0.00", "0.0000", "0");
    testing_mul("-0.01", "0.0000", "0");
    testing_mul("2.01", "3.0000", "6.03");
    testing_mul("2", "-3.000001", "-6.000002");
    testing_mul("-5.0908", "-123.1", "626.67748");
}

int main()
{
    Test();
}
