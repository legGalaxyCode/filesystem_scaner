//
// Created by !--- legGalaxyCode ---! on 31.12.2020.
//

#include "hash_functions.h"

int hash_func_seed(const std::string& s, int table_size, const int key) {
    int hash_result = 0;
    hash_result = std::hash<std::string>{}(s) % table_size;
//    int seed = 21;
//    for (int i = 0; i < s.size(); ++i)
//        hash_result = ((seed * hash_result) + s[i]) % table_size;
//    hash_result = (hash_result * 2 + 1) % table_size;
    return hash_result;
}