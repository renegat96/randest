#include <iostream>
#include <string>
#include <vector>
#include <gmpxx.h>

int main(int argc, char* argv[]) {
    std::vector<std::string> arguments;
    for (int i = 1; i < argc; i++) {
        arguments.emplace_back(argv[i]);
    }

    
    mpz_class x0(arguments[0]);
    mpz_class a(arguments[1]);
    mpz_class c(arguments[2]);
    mpz_class m(arguments[3]);

    int count = std::stoi(arguments[4]);

    for (int i = 0; i < count; ++i) {
        x0 = (a * x0) + c;
        x0 = x0 % m;
        mpf_class helper = x0;
        std::cout << helper / m << '\n';
    }

    return 0;
}
