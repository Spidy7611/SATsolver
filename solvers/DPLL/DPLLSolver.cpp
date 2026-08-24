#include "DPLLSolver.h"
#include <algorithm>

// A fő publikus interfész, amit a main.cpp hív meg
SolveResult DPLLSolver::solve(const Formula& form)
{
    // Meghívjuk a rekurzív magot. 
    
    // itt automatikusan készül egy első másolat az eredeti adatról.
    return dpll(form);
}

// A rekurzív mag (DPLL algoritmus)
SolveResult DPLLSolver::dpll(Formula formula)
{
    // 1. Egység-propagáció: Kényszerített értékadások elvégzése
    // Ha false-t ad vissza, az azt jelenti, hogy üres klóz (konfliktus) keletkezett.
    if (!unitPropagate(formula)) {
        return SolveResult::UNSAT;
    }

    // 2. Alapeset: Ha nincs több klóz, a formula kielégítve (minden klóz True lett)
    if (formula.clauses.empty()) {
        return SolveResult::SAT;
    }

    // 3. Változó választás (Branching / Döntés)
    // Ebben a "buta" verzióban egyszerűen vesszük az első maradék klóz első literálját.
    Literal var = pickVariable(formula);

    // 4. Rekurzív hívás - 1. ág: Próbáljuk meg IGAZ-ra állítani a választott változót
    // Készítünk egy másolatot a jelenlegi állapotról
    Formula trueBranch = formula;
    applyAssignment(trueBranch, var);
    if (dpll(trueBranch) == SolveResult::SAT) {
        return SolveResult::SAT;
    }

    // 5. Backtrack - 2. ág: Ha az IGAZ ág nem vezetett sikerre, próbáljuk meg a HAMIS-at
    // Itt az eredeti 'formula' objektumot használjuk, amit ez a rekurzív szint kapott.
    Formula falseBranch = formula;
    applyAssignment(falseBranch, -var);
    return dpll(falseBranch);
}

// Egység-propagáció implementációja
bool DPLLSolver::unitPropagate(Formula& formula)
{
    bool foundAny = true;

    while (foundAny) {
        foundAny = false;
        Literal unitLiteral = 0;

        // 1. Keressünk egy egységklózt (mérete pontosan 1)
        for (const auto& clause : formula.clauses) {
            // Ha közben bárhol üres klózt találunk, az azonnali UNSAT ezen az ágon
            if (clause.empty()) return false;

            if (clause.size() == 1) {
                unitLiteral = clause[0];
                foundAny = true;
                break; 
            }
        }

        // 2. Ha találtunk egységklózt, alkalmazzuk a benne lévő literált
        if (foundAny) {
            applyAssignment(formula, unitLiteral);
            
            // 3. Egy gyors ellenőrzés a törlés után: keletkezett-e üres klóz?
            for (const auto& clause : formula.clauses) {
                if (clause.empty()) return false; 
            }
        }
    }

    return true; 
}

// A formula fizikai módosítása (Literál és Klóz törlések)
void DPLLSolver::applyAssignment(Formula& formula, Literal lit)
{
    Literal negatedLit = -lit;

    // Hátulról előre haladunk a vektorban, hogy a törlés (erase) 
    // ne rontsa el a még sorra nem került elemek indexelését.
    for (int i = formula.clauses.size() - 1; i >= 0; --i) {
        bool clauseSatisfied = false;
        
        // Megnézzük, a klóz IGAZ lesz-e ettől a literáltól
        for (Literal l : formula.clauses[i]) {
            if (l == lit) {
                clauseSatisfied = true;
                break;
            }
        }

        if (clauseSatisfied) {
            // Ha a klóz tartalmazza a literált, az egész klóz teljesült -> Töröljük
            formula.clauses.erase(formula.clauses.begin() + i);
        } else {
            // Ha a klóz nem teljesült, nézzük meg, van-e benne HAMIS literál (-lit)
            auto& currentClause = formula.clauses[i];
            for (int j = (int)currentClause.size() - 1; j >= 0; --j) {
                if (currentClause[j] == negatedLit) {
                    // Ez a literál kiesik a klózból, mert hamis
                    currentClause.erase(currentClause.begin() + j);
                }
            }
        }
    }
}


Literal DPLLSolver::pickVariable(const Formula& formula)
{
    // Ha van maradék klóz, vegyük az elsőnek az első elemét
    if (!formula.clauses.empty() && !formula.clauses[0].empty()) {
        return formula.clauses[0][0];
    }
    return 0; 
}