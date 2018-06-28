# PreTree - an easy to implement memory-efficient trie
## Based on the Bachelor thesis for University of Bucharest



## Summary
The PreTree is an easy to implement and (very) memory-efficient data structure created during the work for my undergraduate thesis at University of Bucharest. It can be used for manipulating integer data, as well as strings (of equal length).

All implementations inside this repo are based on the following paper:
*"Memory-efficient and easy data structure for dynamic sets"*

## Contents
The application contains three main components: the implementations of the data structure, a testing tool, and a benchmarking tool.

##### Implementations
The implementations for the *PreTree* and *PreTrie* data structures, in different versions, can be found in the `trees` and `tries` directories, respectively. *Note that the implementations differ slightly from the ones described in the paper. Some differences are due to implementation clarity reasons, others are optimizations.*

##### Testing tool
The fuzz testing tool can be found in the `tests` directory, namely `tests/property_test.cpp`. It can be run via command line, by running ` ./a.out TREE_TYPE TEST_COUNT VALUES_PER_TEST OPERATIONS_PER_TEST`, where `TREE_TYPE` is an enum, and its values should be easily extrapolated by reading the source code. 

##### Benchmarking tool
The benchmarking tool can be found in the `tests` directory, namely `tests/ins_ers_test.cpp`. It can be run via command line, by running ` ./a.out VALUE_COUNT OPERATION_COUNT DATA_TYPE DATASETS`, where `DATA_TYPE` is one of `int`, `string`, and `DATASETS` is the list of datasets, potentially separated by a `|` character. Potential datasets are `RAND`, `INC`, `DEC`, `PERM`. Please refer to the implementation for more details

Example usage: `./a.out 100000 1000000 int INC|DEC` will run a benchmark test of `1,000,000` operations on a set of `100,000` values (initially empty), on the sorted insertions and reverse-sorted insertions datasets.

##### Console
The filem `main.cpp` found inside the root directory of the repo is a console-like application for testing the implementations manually. It can be modified at will to test any of the data structures, as long as it has the proper interface.

##### Configuration
Further configuration can be made by modifying the `constants.hpp` header file. Refer to the implementation for more details.

##### Extras
For an example implementation of the String Insertion Problem, refer to `helper/sip.cpp`. It contains a fast implementation, as well as a naive one, a test generator and a comparer, effectively fuzz testing the correctness of the SIP algorithm described inside the paper.