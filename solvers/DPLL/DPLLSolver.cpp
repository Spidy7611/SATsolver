#include "DPLLSolver.h"
#include <algorithm>

// A fő publikus interfész
SolveResult DPLLSolver::solve(const Formula& form)
{
    // Itt készíted elő az első hívást (pl. a kezdeti formula átadása)
    // return dpll(form);
    return SolveResult::UNKNOWN; 
}

// A rekurzív mag
SolveResult DPLLSolver::dpll(Formula formula)
{
    // 1. Egység-propagáció futtatása
    
    // 2. Alapesetek ellenőrzése (SAT / UNSAT)
    
    // 3. Változó választás (Branching)
    
    // 4. Rekurzív hívás az egyik irányba (pl. IGAZ)
    
    // 5. Ha az sikertelen, visszalépés és rekurzív hívás a másik irányba (HAMIS)

    return SolveResult::UNSAT;
}

// Egység-propagáció implementációja
bool DPLLSolver::unitPropagate(Formula& formula)
{
    // Ciklus, amíg találunk egységklózt:
    //   - Megkeressük az 1 elemű klózokat
    //   - Alkalmazzuk az értékadást (applyAssignment)
    //   - Ellenőrizzük, lett-e üres klóz (konfliktus)
    
    return true; 
}

// A formula módosítása egy konkrét literál alapján
void DPLLSolver::applyAssignment(Formula& formula, Literal lit)
{
    // Végigmegyünk a klózokon:
    //   - Ha a klóz tartalmazza 'lit'-et -> a klóz teljesült, törölhető
    //   - Ha a klóz tartalmazza '-lit'-et -> a literál hamis, törölhető a klózból
}

// Következő változó kiválasztása
Literal DPLLSolver::pickVariable(const Formula& formula)
{
    // Visszaad egy literált a még meglévő klózok közül
    return 0; 
}