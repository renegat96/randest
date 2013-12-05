#pragma once
#include <set>
#include <algorithm>
#include <randest/data_provider.hpp>
#include <randest/freq_int_interval.hpp>
#include <randest/test.hpp>

namespace randest {
    template<class OutputT>
    class coupon_test : public test {
        bool ran;
        size_t t, n;
        randest::data_provider<OutputT> *data;
        randest::freq_int_present *test;
        OutputT lower_bound;
        OutputT upper_bound;
        std::set<OutputT> found;
        std::vector<long long> lengths;
    public:
        coupon_test(randest::data_provider<OutputT> *data, OutputT lower_bound, OutputT upper_bound, size_t t, size_t n) {
            this->t = t;
            this->n = n;
            this->ran = false;
            this->data = data;
            this->lower_bound = lower_bound;
            this->upper_bound = upper_bound;
        }
        ~coupon_test() {
            delete test;
        }
        void run() {
            found.clear();
            lengths.clear();
            size_t last = 0;
            for (size_t i = 0; i < data->size() && lengths.size() < n; ++i) {
                found.insert(data->at(i));
                if (found.size() == (this->upper_bound - this->lower_bound)) {
                    lengths.push_back(std::min(t, i - last));
                    last = i;
                    found.clear();
                }
            }
            randest::mem_data<long long> lengths_data(lengths);
            test = new randest::freq_int_interval<long long>(&lengths_data, this->upper_bound - this->lower_bound, t + 1);
            test->run();
            ran = true;
        }
        long double getPerformance() {
            return this->test->getPerformance();
        }
    };
}
