#pragma once
#include <vector>
#include "WatchedFormula.h"

class DPLLFormula : public WatchedFormula {
public:
    int numVars;
    std::vector<std::vector<int>> clauses;
    std::vector<int> assignments; // 0: nincs, 1: True, -1: False
    std::vector<int> trail;       // Értékadások sorrendje (stack)
    std::vector<int> controlStack; // Hol kezdõdtek a döntések (backtrack-hez)

    int qhead = 0; // Propagációs pointer a trail-ben

    DPLLFormula(int n, const std::vector<std::vector<int>>& c)
        : WatchedFormula(n), numVars(n), clauses(c) {
        assignments.assign(numVars + 1, 0);
        trail.reserve(numVars);
        controlStack.reserve(numVars);
        initTwoWatchedLiterals(clauses);
    }
};
