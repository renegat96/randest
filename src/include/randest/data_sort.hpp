#pragma once
#include <randest/data_provider.hpp>
#include <algorithm>

namespace randest {
    template<typename OutputT,
             typename Compare = std::less<OutputT>>
    class data_sort {
    private:
        struct comparison {
            bool operator()(const unsigned int &a, const unsigned int &b) {
                return Compare(this->data->at(a), this->data->at(b));
            }
        };
        unsigned int *indexes;
        unsigned int *reverse_indexes;
        data_provider<OutputT> *data;
    public:

        /*

        data_sort(data_provider<OutputT> *data);
        ~data_sort();
        void sort();
        size_t size();
        OutputT at(const size_t &index);
        OutputT reverse_at(const size_t &index);

        */

        data_sort(data_provider<OutputT> *data) {
            this->data = data;
            this->indexes = new unsigned int[data->size()];
            this->reverse_indexes = new unsigned int[data->size()];
            this->sort();
        }
        ~data_sort() {
            delete [] indexes;
            delete [] reverse_indexes;
        }
        void sort() {
            for (size_t i = 0; i < data->size(); ++i) {
                indexes[i] = i;
            }
            std::sort(indexes, indexes + this->data->size(), comparison());
            for (size_t i = 0; i < data->size(); ++i) {
                reverse_indexes[indexes[i]] = i;
            }
        }

        size_t size() {
            return data->size();
        }

        OutputT operator[](const size_t &index) {
            return this->data->at(indexes[index]);
        }
        OutputT at(const size_t &index) {
            return this->data->at(indexes[index]);
        }
        OutputT reverse_at(const size_t &index) {
            return this->data->at(reverse_indexes[index]);
        }
        size_t count_smaller(const OutputT &value) {
            return std::upper_bound(indexes, indexes + this->data->size(), value, comparison()) - indexes;
        }
    };
}
