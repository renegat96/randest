#include <iostream>
#include <string>
#include <vector>
#include <randest/data_provider.hpp>
#include <randest/tests.hpp>

enum data_type {
    integer,
    longdouble
} type;

enum source {
    input,
    binaryfile,
    textfile
} s;

void printUsage() {
    std::cout << "usage: randest --int|--double\n" <<
                 "               [--stdin|--binary-file path]\n"
                 "                --kstest |\n" <<
                 "                --freqinterval lower upper |\n" <<
                 "                --freqpresent |\n" <<
                 "                --coupon n t upper |\n" <<
                 "                --maxtest groupsize |\n" <<
                 "                --monotone t [--increasing|--decreasing] |\n";
}

randest::data_provider<long long>* readIntegers(const source &s, const std::string &filepath) {
    randest::data_provider<long long> *data = nullptr;
    if (s == input) {
        std::vector<long long> raw;
        long long current;
        while (std::cin >> current) raw.push_back(current);
        data = new randest::mem_data<long long>(raw);
    } else if (s == binaryfile) {
        data = new randest::binary_file_data<long long>(filepath);
    }
    return data;
}

randest::data_provider<long double>* readDoubles(const source &s, const std::string &filepath) {
    randest::data_provider<long double> *data = nullptr;
    if (s == input) {
        std::vector<long double> raw;
        long double current;
        while (std::cin >> current) raw.push_back(current);
        data = new randest::mem_data<long double>(raw);
    } else if (s == binaryfile) {
        data = new randest::binary_file_data<long double>(filepath);
    }
    return data;
}

void kstest(randest::data_provider<long double> *data) {
    std::cout << "Running K-S test...\n";
    randest::ks_test<long double> test(data);
    test.run();
    std::cout << "Result: " << test.getPerformance() << '\n';
}

void freqinterval(randest::data_provider<long long> *data, const long long &upper, const long long &lower) {
    std::cout << "Running a chi^2 test on numbers in interval...\n";
    randest::freq_int_interval<long long> test(data, upper, lower);
    test.run();
    std::cout << "Result: " << test.getPerformance() << '\n';
}

void freqpresent(randest::data_provider<long long> *data) {
    std::cout << "Running a chi^2 test present numbers in data...\n";
    randest::freq_int_present test(data);
    test.run();
    std::cout << "Result: " << test.getPerformance() << '\n';
}

void coupon_test(randest::data_provider<long long> *data, const size_t &n, const size_t &t, const size_t &upper) {
    std::cout << "Running a coupon-collector's test...\n";
    randest::coupon_test<long long> test(data, 0, upper, t, n);
    test.run();
    std::cout << "Result: " << test.getPerformance() << '\n';
}

template<typename T>
void maxtest(randest::data_provider<T> *data, const size_t &group_size) {
    std::cout << "Running a max-of-t test...\n";
    randest::max_test<T> test(data, group_size);
    test.run();
    std::cout << "Result: " << test.getPerformance() << '\n';
}

template<typename T>
void monotone(randest::data_provider<T> *data, const size_t t, const bool &ups) {
    std::cout << "Running a monotone (run) test...\n";
    randest::monotone_test<T> test(data, t, ups);
    test.run();
    std::cout << "Result: " << test.getPerformance() << '\n';
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        printUsage();
        return 0;
    }
    std::vector<std::string> arguments;
    for (int i = 1; i < argc; i++) {
        arguments.emplace_back(argv[i]);
    }
    size_t consumed = 0;
    auto check = [&consumed, &arguments]() -> void {
            if (consumed == arguments.size()) {
                std::cout << "insufficient number of arguments\n";
                printUsage();
                exit(-1);
            }
        };
    if (arguments[consumed].compare("--int") == 0) { 
        type = integer;
    } else if (arguments[consumed].compare("--double") == 0) {
        type = longdouble;
    } else {
        printUsage();
        return -1;
    }
    consumed++; check();
    s = input;
    std::string filepath;
    if (0 == arguments[consumed].compare("--stdin")) {
        s = input;
        consumed++;
    } else if (0 == arguments[consumed].compare("--binary-file")) {
        s = binaryfile;
        consumed++; check();
        filepath = arguments[consumed];
        consumed++;
    } else if (0 == arguments[consumed].compare("--file")) {
        s = textfile;
        consumed++; check();
        filepath = arguments[consumed];
        consumed++;
    }
    check();
    if (0 == arguments[consumed].compare("--kstest")) {
        if (type == integer) throw "data type and test incompatible";
        randest::data_provider<long double> *data = readDoubles(s, filepath);
        kstest(data);
    } else
    if (0 == arguments[consumed].compare("--freqinterval")) {
        if (type == longdouble) throw "data type and test incompatible";
        auto data = readIntegers(s, filepath);
        consumed++; check();
        long long l = std::stoll(arguments[consumed]);
        consumed++; check();
        long long u = std::stoll(arguments[consumed]);
        freqinterval(data, l, u);
    } else
    if (0 == arguments[consumed].compare("--freqpresent")) {
        if (type == longdouble) throw "data type and test incompatible";
        auto data = readIntegers(s, filepath);
        freqpresent(data);
    } else
    if (0 == arguments[consumed].compare("--coupon")) {
        if (type == longdouble) throw "data type and test incompatible";
        auto data = readIntegers(s, filepath);
        consumed++; check();
        long long n = std::stoll(arguments[consumed]);
        consumed++; check();
        long long t = std::stoll(arguments[consumed]);
        consumed++; check();
        long long upper = std::stoll(arguments[consumed]);
        coupon_test(data, n, t, upper);
    } else
    if (0 == arguments[consumed].compare("--maxtest")) {
        consumed++; check();
        long long group_size = std::stoll(arguments[consumed]);
        if (type == integer) {
            auto data = readIntegers(s, filepath);
            maxtest<long long>(data, group_size);
        } else if (type == longdouble) {
            auto data = readDoubles(s, filepath);
            maxtest<long double>(data, group_size);
        }
    } else
    if (0 == arguments[consumed].compare("--monotone")) {
        consumed++; check();
        long long t = std::stoll(arguments[consumed]);
        bool ups = true;
        if (consumed < arguments.size()) {
            if (0 == arguments[consumed].compare("--increasing"))
                ups = true;
            else
            if (0 == arguments[consumed].compare("--decreasing"))
                ups = false;
        }
        if (type == integer) {
            auto data = readIntegers(s, filepath);
            monotone<long long>(data, t, ups);
        } else if (type == longdouble) {
            auto data = readDoubles(s, filepath);
            monotone<long double>(data, t, ups);
        }
    } else {
        std::cout << "invalid or wrong number of arguments\n";
        printUsage();
        return -1;
    }
    return 0;
}

/*
 * usage: randest --int|--double
 *                [--stdin|--binary-file path]
 *                 --kstest |
 *                 --freqinterval lower upper |
 *                 --freqpresent |
 *                 --coupon n t upper |
 *                 --maxtest groupsize |
 *                 --monotone t [--increasing|--decreasing] |
*/
