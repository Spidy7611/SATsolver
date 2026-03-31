#pragma once
#include "Solver.h"
#include <map>

class DPLLSolver : public Solver
{
public:
    // A fő belépési pont, amit a main.cpp-ből hívsz
    SolveResult solve(const Formula& form) override;

private:
    /**
     * A rekurzív mag: ez végzi a tényleges keresést.
     * A formulát érték szerint adjuk át, hogy minden szinten saját másolata legyen
     * (egyszerűbb visszalépni/backtrackelni).
     */
    SolveResult dpll(Formula formula);

    /**
     * Egység-propagáció (Unit Propagation): 
     * Megkeresi az 1 hosszú klózokat és kényszeríti az értéküket.
     * Igazzal tér vissza, ha sikeres, és hamissal, ha ellentmondást (üres klózt) talált.
     */
    bool unitPropagate(Formula& formula);

    /**
     * Segédfüggvény a formula redukálásához:
     * Ha egy literált beállítunk, törli a teljesült klózokat 
     * és az ellentétes hamis literálokat a többi klózból.
     */
    void applyAssignment(Formula& formula, Literal lit);

    /**
     * Kiválasztja a következő változót, amire tippelni fogunk (Branching).
     * Egyelőre lehet az első szabad változó a formulából.
     */
    Literal pickVariable(const Formula& formula);
};