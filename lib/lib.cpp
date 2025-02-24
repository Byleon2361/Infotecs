#include "lib.hpp"
#include <algorithm>
void sort(std::string& str)
{
    std::sort(str.begin(), str.end(), [](char a, char b) { return a < b; });
    int i = 0;
    while (str[i] != '\0') {
        if (static_cast<int>(str[i]) % 2 == 0) {
            str.erase(i, 1);
            str.insert(i, "KB");
            i++;
        }
        i++;
    }
}
int sum(std::string& str)
{
    int sum = 0;
    for (char i : str) {
        if (i >= '0' && i <= '9') {
            sum += i - '0';
        }
    }
    return sum;
}
int count(std::string& str)
{
    if (str.size() < 2)
        return 1;
    if (str.size() % 32 == 0)
        return 1;
    return 0;
}