#pragma once
#include <fstream>
#include <algorithm>
namespace randest {
    template<typename OutputT>
    class data_provider {
    public:
        virtual size_t size() = 0;
        virtual OutputT const& operator[](size_t pos) = 0;
    };
    
    template<typename OutputT>
    class file_data : public data_provider<OutputT> {
    private:
        size_t sizeT;
        ::std::string filename;
        size_t memPosition;
        char* cachedMem;
        size_t bytes;
    public:
        file_data(::std::string filename, size_t bytes = 128 << 20) {
            this->filename = filename;
            this->cachedMem = new char[bytes];
            this->memPosition = 0;
            this->bytes = bytes;
            ::std::ifstream file(this->filename, ::std::ios::in | ::std::ios::binary);
            if (file.is_open()) {
                this->sizeT = file.tellg();
                file.seekg(0, ::std::ios::end);
                this->sizeT = (size_t)file.tellg() - this->sizeT;
                this->sizeT /= sizeof(OutputT);
                file.seekg(0, ::std::ios::beg);
                file.read(cachedMem, bytes);
                file.close();
            } else throw "something went wrong while opening the file";
        }
        const ::std::string getFilename() {
            return ::std::string(this.filename);
        }
        size_t size() {
            return this->sizeT;
        }
        OutputT const& operator[](size_t pos) {
            pos *= sizeof(OutputT);
            if (pos < memPosition || (memPosition + bytes * sizeof(OutputT) - 1) < pos) {
                ::std::ifstream file(this->filename, ::std::ios::in | ::std::ios::binary);
                if (file.is_open()) {
                    file.seekg(pos, ::std::ios::beg);
                    file.read(cachedMem, bytes);
                    file.close();
                    memPosition = pos;
                } else throw "something went wrong while opening the file";
            }
            return *(reinterpret_cast<OutputT*>(cachedMem + memPosition - pos));
        }
    };

    template<typename OutputT>
    class mem_data : public data_provider<OutputT> {
    private:
        ::std::vector<OutputT> data;
    public:
        mem_data(::std::vector<OutputT> input) {
            data = ::std::vector<OutputT>(input);
        }
        mem_data(OutputT *data, size_t len) {
            data = ::std::vector<OutputT>(data, data + len);
        }
        OutputT const& operator[](size_t pos) {
            return data[pos];
        }
        size_t size() {
            return data.size();
        }
    };
}
