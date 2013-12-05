#pragma once

#include <vector>

#include <randest/data_provider.hpp>
#include <randest/ks_test.hpp>

namespace randest {
    template<typename T>
    struct XPowerT {
        XPowerT(size_t t) : t(t) {}
        long double operator()(const T &x) {
            T result = x;
            for (int i = 1; i < t; ++i) {
                result = result * x;
            }
            return static_cast<long double>(result);
        }
    private:
        size_t t;
    };
    template<typename OutputT = long double,
             typename Compare = std::less<OutputT>>
    class max_test : public test {
    private:

        bool ran;
        size_t group_size;
        ::std::vector<OutputT> max_elements;
        ::randest::data_provider<OutputT> *data;
        Compare compare;

    public:

        /*
        max_test(::randest::data_provider<OutputT> *data, size_t group_size);
        max_test(::randest::data_provider<OutputT> *data, size_t group_size, Compare compare);
        ~max_test();
        void run();
        long double getPerformance();

        struct XPowerT {
            long double operator()(const OutputT &x);
        };
         */


        max_test(::randest::data_provider<OutputT> *data, size_t group_size) {
            this->ran = false;
            this->data = data;
            this->group_size = group_size;
        }
        max_test(::randest::data_provider<OutputT> *data, size_t group_size, Compare compare)
                : max_test(data, group_size) {
            this->compare = compare;
        }
        ~max_test() {
            delete kstest;
        }
        void run() {
            if (kstest != nullptr) {
                delete kstest;
                kstest = nullptr;
            }

            this->max_elements.clear();
            for (size_t i = 0; i < this->data->size(); i += group_size) {
                this->max_elements.push_back(this->data->at(i));
                for (size_t j = i; j < i + group_size && i + group_size <= this->data->size(); ++j) {
                    if (compare(this->max_elements.back(), this->data->at(j))) {
                        this->max_elements.back() = this->data->at(j);
                    }
                }
            }

            ::randest::mem_data<OutputT> max_elements_data(max_elements);
            this->kstest = new ks_test<OutputT> (&max_elements_data, XPowerT<OutputT>(group_size));
            this->kstest->run();
            this->ran = true;
        }
        long double getPerformance() {
            if (!this->ran) throw "trying to get the result from a test that has not been ran";
            return this->kstest->getPerformance();
        }
    private:
        ::randest::ks_test<OutputT> *kstest = nullptr;
    };
}
