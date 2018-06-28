#pragma once
#include <vector>

const int                   DEFAULT_VALUE               = -1;               // Default value for unsuccessful pred/succ (integers)
const std::string           DEFAULT_VALUE_STRING        = "NULL";           // Default value for unsucessful pred/succ (strings)
const int                   BIT_COUNT                   = 30;               // Word size (integers)
const int                   BIT_COUNT_STRING            = 128;              // Word size (strings)
const int                   SIGMA                       = 26;               // Alphabet size
const std::vector<int>      OPS_TO_GENERATE             = {0, 1, 2};        // Set of ops to generate



// Default generic -- do not modify
template<typename T> T Default();
template<> int Default() { return DEFAULT_VALUE; }
template<> std::string Default() { return DEFAULT_VALUE_STRING; }
