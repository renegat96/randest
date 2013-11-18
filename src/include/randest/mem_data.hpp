#pragma once
#include <vector>
#include <randest/data_provider.hpp>
namespace randest {
    template<typename OutputT>
    class mem_data : public data_provider<OutputT> {
    private:
        ::std::vector<OutputT> data;
    public:
        /*
        mem_data(const ::std::vector<OutputT> &input);
        mem_data(OutputT *data, size_t len);
        OutputT const& operator[](size_t pos);
        size_t size();
        */

        mem_data(const ::std::vector<OutputT> &input) {
            std::copy(input.begin(), input.end(), std::back_inserter(data));
        }

        mem_data(OutputT *data, size_t len) {
            data = ::std::vector<OutputT>(data, data + len);
        }

        OutputT const& operator[](size_t pos) {
            return this->data[pos];
        }

        size_t size() {
            return data.size();
        }
    };
}
