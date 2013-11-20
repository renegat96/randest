/*
 * Kolmogorov-Smirnov goodness-of-fit test.
 *   H_0 the data follow a specified distribution
 *   H_a the data do not follow the specified distribution
 *   D = max (F(Y_i) - 
 * */

#pragma once

#include <functional>

#include <randest/test.hpp>
#include <randest/data_provider.hpp>
#include <randest/data_sort.hpp>

namespace randest {

    template<typename OutputT = long double>
    struct NormalDistribution {
        long double operator()(const OutputT &x) {
            return static_cast<long double>(x);
        }
    };

    template<typename OutputT = long double,
             typename Compare = std::less<OutputT>,
             typename CDF = std::function<long double(OutputT)>>
    class ks_test : public test {
    private:
        bool ran;
        randest::data_provider<OutputT> *data;
        long double ks_statistic;

        /*
        ks_test();
        */

        ks_test();

        CDF cdf;

    public:

        /*
        ks_test(data_provider<OutputT> *data, CDF cdf = NormalDistribution<OutputT>());
        void run();
        long double getPerformance();
        */

        ks_test(data_provider<OutputT> *data, CDF cdf = NormalDistribution<OutputT>()) {
            this->data = data;
            this->ks_statistic = 0;
            this->cdf = cdf;
        }

        void run() {
            ::randest::data_sort<OutputT> sorted = data_sort<OutputT, Compare>(data);
            ks_statistic = 0;
            for (size_t i = 0; i < sorted.size(); ++i) {
                long double fx = static_cast<long double>(sorted.count_smaller(sorted[i])) / sorted.size();
                long double observation = cdf(sorted[i]);
                long double candidate = fabsl(observation - fx);
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
