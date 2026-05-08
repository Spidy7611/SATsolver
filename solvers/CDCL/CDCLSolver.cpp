#include "CDCLSolver.h"
#include <cmath>
#include <algorithm>

SolveResult CDCLSolver::solve(const Formula& form) {
    // 1. Konverzió: Az alap formulából CDCL-re váltunk
    // Itt feltételezzük, hogy a CDCLFormula konstruktora elvégzi a 2WL inicializálást
    CDCLFormula f(form.numVars, form.clauses); 

    // 2. Kezdeti kényszerítések (0. szintű unit propagation)
    if (propagate(f) != -1) return SolveResult::UNSAT;

    while (!f.allAssigned()) {
        int conflictID = propagate(f);

        if (conflictID != -1) {
            // --- KONFLIKTUS KEZELÉS ---
            if (f.currentLevel() == 0) return SolveResult::UNSAT;

            std::vector<int> learnedClause;
            int backtrackLevel = 0;

            // 1UIP alapú tanulás
            analyzeConflict(f, conflictID, learnedClause, backtrackLevel);

            // Visszaugrás a kiszámolt szintre
            backjump(f, backtrackLevel);

            // Új tudás rögzítése
            f.addLearnedClause(learnedClause);

            // A tanult klóz első eleme (1UIP) kényszerített lesz a visszaugrás után
            if (!assign(f, learnedClause[0], f.clauses.size() - 1)) {
                return SolveResult::UNSAT;
            }
            
            continue; 
        }

        // --- DÖNTÉS ---
        if (f.allAssigned()) break;

        int nextLit = pickVariable(f);
        f.newDecisionLevel(); // Új szintet nyitunk a trailLim-ben
        if (!assign(f, nextLit, -1)) {
            return SolveResult::UNSAT;
        }
    }

    return SolveResult::SAT;
}

int CDCLSolver::propagate(CDCLFormula& f) {
    while (f.qhead < (int)f.trail.size()) {
        int p = f.trail[f.qhead++];
        int falseLit = -p; // Aki igaz lett, annak az ellentettje vált hamissá
        
        auto& ws = f.watches[f.litToIdx(falseLit)];
        for (size_t i = 0; i < ws.size(); ) {
            // Blocking literal trükk: ha a mentett lit igaz, a klóz biztosan igaz
            if (f.value(ws[i].blockingLiteral) == 1) {
                i++;
                continue;
            }

            int currClauseID = ws[i].clauseID;
            auto& c = f.clauses[currClauseID];

            // Garantáljuk, hogy a hamis figyelő a c[1]-ben legyen (c[0] a másik figyelő)
            if (c[0] == falseLit) std::swap(c[0], c[1]);

            // Ha a másik figyelő (c[0]) igaz, a klóz rendben van
            if (f.value(c[0]) == 1) {
                ws[i].blockingLiteral = c[0];
                i++;
                continue;
            }

            // Új figyelőt keresünk (c[2]-től kezdve)
            bool found = false;
            for (size_t k = 2; k < c.size(); ++k) {
                if (f.value(c[k]) != -1) { // Nem hamis, tehát jó lesz őrszemnek
                    std::swap(c[1], c[k]);
                    f.addWatch(c[1], currClauseID, c[0]);
                    // Törlés a jelenlegi listából: az utolsó elemet idehozzuk
                    ws[i] = ws.back();
                    ws.pop_back();
                    found = true;
                    break;
                }
            }

            if (!found) {
                // Nem találtunk új figyelőt -> c[0]-t kell vizsgálni
                if (f.value(c[0]) == -1) {
                    // KONFLIKTUS: Mindkét figyelő hamis
                    return currClauseID;
                } else {
                    // UNIT PROPAGATION: c[0] kényszerítetté vált
                    if (!assign(f, c[0], currClauseID)) {
                        return currClauseID;
                    }
                    i++;
                }
            }
        }
    }
    return -1;
}

void CDCLSolver::analyzeConflict(CDCLFormula& f, int conflictID, std::vector<int>& learned, int& btLevel) {
    std::vector<bool> seen(f.numVars + 1, false);
    int counter = 0;
    int p = 0;
    int index = f.trail.size() - 1;
    
    learned.push_back(0); // 1UIP helye
    btLevel = 0;

    int currID = conflictID;
    do {
        auto& c = f.clauses[currID];
        for (int lit : c) {
            int var = std::abs(lit);
            if (!seen[var] && f.decisionLevels[var] > 0) {
                seen[var] = true;
                if (f.decisionLevels[var] >= f.currentLevel()) {
                    counter++;
                } else {
                    learned.push_back(lit);
                    btLevel = std::max(btLevel, f.decisionLevels[var]);
                }
            }
        }

        // Következő literál a trailről, ami érintett a konfliktusban
        while (index >= 0 && !seen[std::abs(f.trail[index])]) {
            index--;
        }

        if (index < 0) {
            break;
        }

        p = f.trail[index];
        currID = f.reasons[std::abs(p)];
        seen[std::abs(p)] = false;
        counter--;
        index--;

    } while (counter > 0 && index >= 0);

    learned[0] = -p; // Az 1UIP literál tagadva kerül be
}

void CDCLSolver::backjump(CDCLFormula& f, int level) {
    // Visszaugrás a trailLim segítségével
    int targetSize = (level <= 0) ? 0 : f.trailLim[level - 1];
    
    while ((int)f.trail.size() > targetSize) {
        int lit = f.trail.back();
        int var = std::abs(lit);
        
        f.assignments[var] = 0;
        f.decisionLevels[var] = -1;
        f.reasons[var] = -1;
        
        f.trail.pop_back();
    }
    
    f.qhead = targetSize;
    f.trailLim.resize(level);
}

bool CDCLSolver::assign(CDCLFormula& f, int lit, int reasonID) {
    int var = std::abs(lit);
    int val = (lit > 0) ? 1 : -1;

    if (f.assignments[var] == val) return true;
    if (f.assignments[var] == -val) return false;

    f.assignments[var] = val;
    f.decisionLevels[var] = f.currentLevel();
    f.reasons[var] = reasonID;
    f.trail.push_back(lit);
    return true;
}

int CDCLSolver::pickVariable(CDCLFormula& f) {
    // Egyszerű statikus heurisztika: az első szabad változó
    for (int i = 1; i <= f.numVars; ++i) {
        if (f.assignments[i] == 0) return i;
    }
    return 0;
}