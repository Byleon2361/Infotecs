#include "../lib/lib.hpp"
#include <iostream>
int main()
{
    std::string str("87902452389452389511111");
    sort(str);
    std::cout << str << "\n";
    std::cout << sum(str) << "\n";
    std::cout << count(str) << "\n";
    return 0;
}