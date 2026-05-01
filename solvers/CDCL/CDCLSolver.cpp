#include "CDCLSolver.h"

SolveResult CDCLSolver::solve(const Formula& form) {
    // 1. CDCLFormula példányosítása (2WL inicializálása, adatstruktúrák előkészítése)
    // 2. Kezdeti egységklózok (Unit Clauses) kényszerítése
    
    // 3. Fő CDCL hurok
    while (true) {
        // a. Propagate: Kényszerítések végrehajtása
        // b. Ha konfliktus van:
        //    - Ha a szint 0, return UNSAT
        //    - analyzeConflict -> backjump -> addLearnedClause
        // c. Ha minden változó be van állítva: return SAT
        // d. Ha nincs több kényszerítés: pickVariable -> f.newLevel -> assign
    }

    return SolveResult::UNKNOWN;
}

int CDCLSolver::propagate(CDCLFormula& f) {
    // A trail qhead mutatójától indulva feldolgozzuk az új literálokat
    // A 2WL (watches) listákat használva keressük a kényszerített klózokat
    // Ha egy klóz "konfliktusossá" válik (minden literálja hamis):
    //    return conflictClauseID;
    
    return -1; // Nincs konfliktus
}

void CDCLSolver::analyzeConflict(CDCLFormula& f, int conflictClauseID, std::vector<int>& learnedClause, int& backtrackLevel) {
    // 1. A konfliktusos klóztól indulva (rezolúcióval) visszakövetjük a trail-t
    // 2. Addig végezzük a rezolúciót a 'reason' klózokkal, amíg 1UIP állapotba nem érünk
    // 3. Megállapítjuk a backtrackLevel-t (a második legmagasabb szint a tanult klózban)
    // 4. Összeállítjuk a learnedClause-t
}

int CDCLSolver::pickVariable(CDCLFormula& f) {
    // VSIDS (Variable State Independent Decaying Sum) vagy egyszerű heurisztika
    // Kiválasztja a következő szabad változót, amit beállítunk
    
    return 0; // Visszatér a választott literállal
}

void CDCLSolver::backjump(CDCLFormula& f, int level) {
    // 1. A trail-ről eltávolítjuk a 'level' szintnél magasabb beállításokat
    // 2. Az assignments, level és reason táblákat alaphelyzetbe állítjuk ezekre a változókra
    // 3. A qhead-et visszaállítjuk a megfelelő pozícióba
}

void CDCLSolver::assign(CDCLFormula& f, int lit, int level, int reasonID) {
    // 1. Beállítjuk a literál értékét
    // 2. Eltároljuk a döntési szintet (f.level[var] = level)
    // 3. Eltároljuk az indoklást (f.reason[var] = reasonID)
    // 4. Hozzáadjuk a trail-hez
}