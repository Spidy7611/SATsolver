#include "stackDPLLSolver.h"
#include <cmath>

//belépési pont
SolveResult DPLLSolver::solve(const Formula& form) {
    // Itt inicializáljuk a DPLLFormula objektumot az eredeti formulából
    DPLLFormula f(form.numVars, form.clauses);
    // Meghívjuk az iteratív keresőt
    return dpllIterative(f);
}

// 2. Az iteratív DPLL 
SolveResult DPLLSolver::dpllIterative(DPLLFormula& f) {
    while (true) {
        // Először futtatjuk a kényszerpályákat (BCP)
        if (!propagate(f)) {
            // HA KONFLIKTUS VAN:
            // Itt kell a controlStack alapján visszalépni (Backtrack)
            // Ha már nincs hova visszalépni, akkor UNSAT
        } else {
            // HA NINCS KONFLIKTUS:
            // Ellenőrizzük, kész vagyunk-e (minden változó be van-e állítva)
            // Ha nem, akkor itt kell döntést hozni (pickVariable)
            // A döntés indexét elmentjük a controlStack-be
        }
    }
}

// 3. A 2WL motor (Boolean Constraint Propagation)
bool DPLLSolver::propagate(DPLLFormula& f) {
    // Ez a függvény végigmegy a trail azon elemein, amiket még nem vizsgáltunk
    // Használja a f.watches listát a módosult literálokhoz
    
    // Itt történik a " blockingLiteral" ellenőrzése
    // Itt történik az új figyelő keresése a klózokban
    
    // Ha egy klózban mindenki hamis -> return false (Konfliktus)
    // Ha egy klóz egységklózzá válik -> f.assign(kényszerített_lit)
    
    return true; 
}

// 4. Értékadás és naplózás
bool DPLLSolver::assign(DPLLFormula& f, int lit) {
    // Beállítjuk az assignments[var] értékét
    // Rátesszük a literált a trail-re
    // (Fontos: ha ellentmondásos az értékadás, jelezzük)
    return true;
}