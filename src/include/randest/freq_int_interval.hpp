#pragma once
#include <randest/data_provider.hpp>
#include <randest/freq_int_present.hpp>
namespace randest {
    struct Equidistributed {
        long double operator()(const size_t &sampleSize, const size_t &groupCount, const size_t &groupIndex) {
            return static_cast<long double>(sampleSize) / groupCount;
        }
    };

    template<typename T = long long,
             typename Expectation  = Equidistributed>
    class freq_int_interval : public test {
    private:
        ::std::map<long long, size_t> present;
        T lower_bound, upper_bound;
        data_provider<long long> *data;
        bool ran;
        long double chi_squared;
        size_t *count;
    public:
        freq_int_interval(data_provider<T> *data, T lower_bound, T upper_bound) {
            this->lower_bound = lower_bound;
            this->upper_bound = upper_bound;
            this->count = new size_t[upper_bound - lower_bound]();
            this->chi_squared = 0;
            this->data = data;
        }
        //~freq_int_interval() {
        //    delete [] count;
        //}
        void run() {
            for (size_t i = 0; i < data->size(); ++i) {
                count[data->at(i) - lower_bound]++;
            }
            for (size_t i = 0; i < upper_bound - lower_bound; ++i) {
                long double expected = Expectation()(data->size(), upper_bound - lower_bound, i);
                long double deviation = expected - count[i];
                this->chi_squared += deviation * deviation / expected;
            }
            ran = true;
        }
        long double getPerformance() {
            if (!this->ran) throw "trying to get the result from a test that has not been ran";
            return chi_squared;
        }
    };
}
