Randest
=======

Tool for randomness testing of data.

For a more detailed explanation, refer to paper/documentation.pdf. (in Bulgarian)

Compiling
=======

Linux
-----

    > git clone https://github.com/renegat96/randest/
    > cd randest/
    > cd src/
    > mkdir bin
    > g++ -std=c++11 -Wall -O2 main.cpp -o bin/randest

Windows
------

Another way to build Randest is by using [CodeBlocks](http://www.codeblocks.org/). One should add the include/ directory in the include path by the compiler options in the IDE. 

*It is very important to turn C++11 support __on__.* Unless so, there will be compile errors.

A thought worth considering is [installing Cygwin](http://cygwin.com/install.html) and referring to the previous section.

Usage
=====

    usage: randest --int|--double
                   [--stdin|--binary-file path]
                    --kstest |
                    --freqinterval lower upper |
                    --freqpresent |
                    --coupon n t upper |
                    --maxtest groupsize |
                    --monotone t [--increasing|--decreasing] |

The first argument should always be either "--int" or "--double" to specify the type of data input.

What may follow is a specification of what is the source of data. Default value is --stdin. As of now only two modes are supported. _It is possible to adjust the cache size when reading from a file, though it requires modifying the source._

The last option is the test to be run. Specify exactly one test with all its parameters. An exception is the monotone test - whether to examine increasing or decresing subsequences defaults to increasing, if not explicitly stated otherwise.

Examples
======

The following execution of Randest will try to read the given file in binary mode, take as many long long variables and apply a chi-squared test on the resulting sequence:
    
    > randest --int --binary-file file/with/binary/string --freqpresent

Another way to use it is shown here. This would read doubles from standard input as long as there are some and then give the Kolmogorov-Smirnov statistic of the sequence. Piping allows reading from a file.

    > randest --double --stdin --kstest < file/with/a/sequence/of/doubles

And the last example given here is this:

    > randest --int --stdin --coupon 100000 400 100 < file/with/nonnegative/integers/less/than/100

This parametrization of the coupon-collector's test is as follows:

* the collector will take at most 100000 collections;
* all collections larger than 400 items will be considered 400 of length;
* the input to the test is less than 100.
