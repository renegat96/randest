#pragma once
#include <map>
#include <randest/test.hpp>
#include <randest/data_provider.hpp>
namespace randest {
    class freq_int_present : public test {
    protected:
        bool ran;
        ::std::map<long long, size_t> present;
        data_provider<long long> *data;
        long double chi_squared;
    public:
        /*
        freq_int_present(data_provider<long long> *data);
        void run();
        long double getPerformance();
        */

        freq_int_present(data_provider<long long> *data) {
            this->ran = false;
            this->chi_squared = 0;
            this->data = data;
        }
        void run() {
            for (size_t i = 0; i < data->size(); ++i) {
                present[data->at(i)]++;
            }
            long double expected = ((long double)data->size()) / (long double)present.size();
            for (auto record : present) {
                long double deviation = expected - record.second;
                this->chi_squared += deviation * deviation / expected;
            }
            ran = true;
        }
        long double getPerformance() {
            if (!this->ran) throw "trying to get the result from a test that has not been ran";
            return this->chi_squared;
        }
    };
}
