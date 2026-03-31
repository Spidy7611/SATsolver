#pragma once
#include <vector>

using Literal = int;
using Clause = std::vector<Literal>;

struct Formula
{
    int numVars;
    std::vector<Clause> clauses;

    Formula() : numVars(0) {}
};