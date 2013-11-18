#include <randest/data_provider.hpp>
#include <randest/freq_int_present.hpp>
namespace randest {
    class freq_int_interval : public freq_int_present {
    private:
        size_t *count;
        long long lower_bound, upper_bound;
    public:
        freq_int_interval(data_provider<long long> *data, long long lower_bound, long long upper_bound) : freq_int_present(data) {
            this->lower_bound = lower_bound;
            this->upper_bound = upper_bound;
            this->count = new size_t[upper_bound - lower_bound]();
            this->chi_squared = 0;
        }
        ~freq_int_interval() {
            delete [] count;
        }
        void run() {
            for (size_t i = 0; i < data->size(); ++i) {
                count[data->at(i) - lower_bound]++;
            }
            long double expected = ((long double)data->size()) / (long double)(upper_bound - lower_bound);
            for (size_t i = 0; i < upper_bound - lower_bound; ++i) {
                long double deviation = expected - count[i];
                this->chi_squared += deviation * deviation / expected;
            }
            ran = true;
        }
    };
}
