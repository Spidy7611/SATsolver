#include "BruteForceSolver.h"
#include <algorithm>
#include <cmath>

inline bool BFSolver::literalValue(Literal lit, uint64_t mask) const
{
    // literál indexe a maskon
    int var = std::abs(lit) - 1;
    // mask eltolása var bittel és az utolsó bit vizsgálata
    bool value = (mask >> var) & 1;
    // negálás szerint visszatérünk
    return lit < 0 ? !value : value;
}

SolveResult BFSolver::solve(const Formula& form)
{
    //mivel bruteforce túl lassú ezért sose oldunk meg vele 64 literált így nem lép fel undefined behavior
    uint64_t maxMask = 1ULL << form.numVars; // összes lehetséges assignment

    for (uint64_t mask = 0; mask < maxMask; ++mask)
    {
        bool formulaTrue = true;

        // minden clause ellenőrzése
        for (size_t i = 0; i < form.clauses.size(); ++i)
        {
            bool clauseTrue = false;

            for (size_t j = 0; j < form.clauses[i].size(); ++j)
            {
                if (literalValue(form.clauses[i][j], mask))
                {
                    clauseTrue = true;
                    break; // már igaz a clause
                }
            }

            if (!clauseTrue)
            {
                formulaTrue = false;
                break; // már hamis a formula
            }
        }

        if (formulaTrue)
            return SolveResult::SAT; // találtunk kielégítő assignmentet
    }

    return SolveResult::UNSAT; // egyik assignment sem jó
}


