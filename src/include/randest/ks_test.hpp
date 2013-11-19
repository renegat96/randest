/*
 * Kolmogorov-Smirnov goodness-of-fit test.
 *   H_0 the data follow a specified distribution
 *   H_a the data do not follow the specified distribution
 *   D = max (F(Y_i) - 
 * */

#pragma once
#include <randest/test.hpp>
#include <randest/data_provider.hpp>
#include <randest/data_sort.hpp>

namespace randest {

    template<typename OutputT = long double>
    struct NormalDistribution {
        long double operator()(const OutputT &x) {
            return x;
        }
    };

    template<typename OutputT = long double,
             typename Compare = std::less<OutputT>,
             typename CDF = NormalDistribution<OutputT>>
    class ks_test : public test {
    private:
        bool ran;
        randest::data_provider<OutputT> *data;
        CDF comp;
        long double ks_statistic;

        /*
        ks_test();
        */

        ks_test();
        data_sort<OutputT> sorted;

    public:

        /*
        ks_test(data_provider<OutputT> *data);
        void run();
        long double getPerformance();
        */

        ks_test(data_provider<OutputT> *data) {
            this->data = data;
            this->comp = comp;
            this->ks_statistic = 0;
        }

        void run() {
            sorted = data_sort<OutputT, Compare>(data);
            ks_statistic = 0;
            for (size_t i = 0; i < sorted; ++i) {
                long double fx = (long double)sorted.count_smaller(sorted[i]) / sorted.size();
                long double candidate = fabsl(CDF(sorted[i]) - fx);
                ks_statistic = std::max(ks_statistic, candidate);
            }
            ran = true;
        }

        long double getPerformance() {
            if (!this->ran) throw "trying to get the result from a test that has not been ran";
            return ks_statistic;
        }
    };
}
