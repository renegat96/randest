#pragma once
#include <fstream>
#include <algorithm>
#include <vector>
namespace randest {
    template<typename OutputT>
    class data_provider {
    public:
        virtual size_t size() = 0;
        virtual OutputT const& operator[](size_t pos) = 0;
        OutputT at(size_t pos) {
            return (*this)[pos];
        }
    };
}
