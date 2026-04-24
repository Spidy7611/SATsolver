#pragma once

#include "../Solver.h" 
#include "../../formula/DPLLFormula.h"

class stackDPLLSolver : public Solver
{
public:
    //belépési pont 
    SolveResult solve(const Formula& form) override;

private:
    //fő algoritmus
    SolveResult dpllIterative(DPLLFormula& f);

    /**
     * Boolean Constraint Propagation (BCP) a Two Watched Literals segítségével.
     * Végigmegy a trail-en, és beállítja a kényszerített változókat.
     * Igaz, ha nincs konfliktus, Hamis, ha ellentmondást talált.
     */
    bool propagate(DPLLFormula& f);

    /**
     * Segédfüggvény: Beállít egy változót, és ráteszi a trail-re.
     */
    bool assign(DPLLFormula& f, int lit);
};