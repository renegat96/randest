#pragma once

#include <vector>

#include <randest/freq_int_present.hpp>

namespace randest {
    template<typename OutputT = long double,
             typename Compare = std::less<OutputT>>
    class monotone_test : test {
        bool ran;
        bool runUps;
        size_t t;
        std::vector<long long> lengths;
        randest::data_provider<OutputT> *data;
        randest::freq_int_present *freq;
    public:
        monotone_test(randest::data_provider<OutputT> *data, size_t t, bool runUps = true) {
            this->runUps = runUps;
            this->ran = false;
            this->data = data;
            this->t = t;
        }
        ~monotone_test() {
            if (ran) delete this->freq;
        }
        void run() {
            lengths.clear();
            lengths.push_back(1);

            for (size_t i = 1; i < data->size(); ++i) {
                if ((Compare()(data->at(i - 1), data->at(i)) && runUps) ||
                    (Compare()(data->at(i), data->at(i - 1)) && !runUps)) {
                    lengths.back()++;
                } else {
                    i++;
                    if (i < data->size()) lengths.push_back(1);
                }
            }

            ::randest::mem_data<long long> runs(lengths);
            this->freq = new randest::freq_int_present(&runs);
            this->freq->run();
            this->ran = true;
        }
        long double getPerformance() {
            if (!this->ran) throw "trying to get the result from a test that has not been ran";
            return this->freq->getPerformance();
        }
    };
}
