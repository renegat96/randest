#pragma once
namespace randest {
    class test {
    public:
        virtual void run() = 0;
        virtual long double getPerformance() = 0;
        virtual ~test() {}
    };
}
