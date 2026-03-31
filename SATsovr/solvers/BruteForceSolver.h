#pragma once
#include "Solver.h"
#include <cstdint>
class BFSolver : public Solver
{


    inline bool literalValue(Literal lit, uint64_t mask) const;

public:
    SolveResult solve(const Formula& form);

    
};
