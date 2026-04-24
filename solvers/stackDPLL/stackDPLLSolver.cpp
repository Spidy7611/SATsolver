#include "stackDPLLSolver.h"
#include <cmath>
#include <algorithm>

// 1. Belépési pont
SolveResult stackDPLLSolver::solve(const Formula& form) {
    DPLLFormula f(form.numVars, form.clauses);
    return dpllIterative(f);
}

// 2. Az iteratív DPLL
SolveResult stackDPLLSolver::dpllIterative(DPLLFormula& f) {
    // 0. LÉPÉS: Kezdeti Unit klózok beállítása
    for (const auto& clause : f.clauses) {
        if (clause.size() == 1) {
            if (!assign(f, clause[0])) {
                return SolveResult::UNSAT; // Már az alapformula is ellentmondásos
            }
        }
    }

    while (true) {
        // 1. LÉPÉS: BCP (Propagáció)
        if (!propagate(f)) {
            // HA KONFLIKTUS VAN (Backtrack fázis)
            if (f.controlStack.empty()) {
                return SolveResult::UNSAT; // Nincs hova visszalépni
            }

            // Megnézzük, hol volt az utolsó döntésünk
            int targetSize = f.controlStack.back();
            f.controlStack.pop_back();

            // Kimentjük az utolsó döntést (hogy megpróbálhassuk az ellenkezőjét)
            int failedDecision = f.trail[targetSize];

            // Visszatekerjük az időt (Törlés a trail-ről és assignments-ből)
            while ((int)f.trail.size() > targetSize) {
                int lit = f.trail.back();
                f.assignments[std::abs(lit)] = 0; // Nullázás
                f.trail.pop_back();
            }
            
            // A propagációs mutatót is vissza kell húzni!
            f.qhead = targetSize;

            // Kényszerítjük a bukott döntés ELLENKEZŐJÉT
            assign(f, -failedDecision);

        } else {
            // HA NINCS KONFLIKTUS (Döntési fázis)
            
            // Keresünk egy még beállítatlan változót (1-től indulunk, mert a 0. index semmi)
            auto it = std::find(f.assignments.begin() + 1, f.assignments.end(), 0);
            int nextVar = (it == f.assignments.end()) ? 0 : std::distance(f.assignments.begin(), it);

            if (nextVar == 0) {
                return SolveResult::SAT; // Nincs több szabad változó, KÉSZ VAGYUNK!
            }
            
            // Lementjük a jelenlegi szintet a verembe
            f.controlStack.push_back(f.trail.size());
            
            // Meghozzuk a döntést (Tippelünk egy IGAZ értéket)
            assign(f, nextVar); 
        }
    }
}

// 3. A 2WL motor (Boolean Constraint Propagation)
bool stackDPLLSolver::propagate(DPLLFormula& f) {
    while (f.qhead < (int)f.trail.size()) {
        int p = f.trail[f.qhead++];
        int falseLit = -p; // A literál, ami most HAMIS lett
        
        // Kérjük a hamis literálhoz tartozó postaládát
        auto& watchList = f.watches[f.litToIdx(falseLit)];
        
        int i = 0;
        while (i < (int)watchList.size()) {
            Watcher& w = watchList[i];
            int clauseID = w.clauseID;
            int blockingLit = w.blockingLiteral;
            
            // 1. Ellenőrzés: A klóz már igaz? (A blockingLiteral igaz?)
            int bVar = std::abs(blockingLit);
            int bVal = (blockingLit > 0) ? 1 : -1;
            if (f.assignments[bVar] == bVal) {
                i++; // Mázlink van, ezzel a klózzal most nincs dolgunk
                continue;
            }
            
            // 2. Keresünk egy új, nem-hamis literált a klózban
            const auto& clause = f.clauses[clauseID];
            bool foundNewWatch = false;
            
            for (int lit : clause) {
                if (lit == falseLit) continue; // Önmagunkat nem figyelhetjük
                
                int lVar = std::abs(lit);
                int lVal = (lit > 0) ? 1 : -1;
                
                // Ha a literál IGAZ vagy ISMERETLEN (!= hamis)
                if (f.assignments[lVar] != -lVal) {
                    // Megtaláltuk az új figyelőt! Átregisztráljuk.
                    f.addWatch(lit, clauseID, blockingLit);
                    
                    // Töröljük a jelenlegi figyelőt ebből a listából (Gyors törlés vektor közepéből)
                    watchList[i] = watchList.back();
                    watchList.pop_back();
                    foundNewWatch = true;
                    break; // Kilépünk a klóz vizsgálatából
                }
            }
            
            // 3. Ha nem találtunk új figyelőt, a klóz egységklózzá (vagy üressé) vált!
            if (!foundNewWatch) {
                i++; // A figyelőt itt kell hagynunk, mert nincs hova átrakni
                
                // A blockingLiteral az utolsó reményünk, be kell állítanunk!
                if (!assign(f, blockingLit)) {
                    return false; // Ellentmondás! A literál már a másik irányba van beállítva.
                }
            }
        }
    }
    return true; // Minden kényszerítés sikeresen lefutott
}

// 4. Értékadás és naplózás
bool stackDPLLSolver::assign(DPLLFormula& f, int lit) {
    int var = std::abs(lit);
    int val = (lit > 0) ? 1 : -1;

    // Ha a változó már megkapta PONTOSAN ezt az értéket, nincs baj
    if (f.assignments[var] == val) return true;
    
    // Ha a változó a fordítottjára van állítva, az KONFLIKTUS!
    if (f.assignments[var] == -val) return false;

    // Ha idáig eljut, akkor a változó eddig 0 (ismeretlen) volt.
    f.assignments[var] = val;
    f.trail.push_back(lit);
    return true;
}