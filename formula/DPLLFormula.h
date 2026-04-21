#pragma once
#include <vector>

//cache miatt intek és nem pointerek 
struct Watcher {
    int clauseID;
    int blockingLiteral; // egy másik literál a klózból, ami valószínűleg igaz
};

class DPLLFormula {
public:
    int numVars;
    std::vector<std::vector<int>> clauses;
    
    // watches[litIdx] -> a literált figyelő klózok listája
    // Indexelés: var 1 -> idx 2, var -1 -> idx 3 (litToIdx függvény)
    std::vector<std::vector<Watcher>> watches;
    
    std::vector<int> assignments; // 0: nincs, 1: True, -1: False
    std::vector<int> trail;       // Értékadások sorrendje (stack)
    std::vector<int> controlStack; // Hol kezdődtek a döntések (backtrack-hez)

    DPLLFormula(int n, const std::vector<std::vector<int>>& c) : numVars(n), clauses(c) {
        assignments.assign(numVars + 1, 0);
        trail.reserve(numVars);
        controlStack.reserve(numVars);
        watches.resize(2 * numVars + 2);
        
        // Kezdeti 2WL beállítás: minden legalább 2 hosszú klóz első két elemét figyeljük
        for (int i = 0; i < (int)clauses.size(); ++i) {
            if (clauses[i].size() >= 2) {
                addWatch(clauses[i][0], i, clauses[i][1]);
                addWatch(clauses[i][1], i, clauses[i][0]);
            }
        }
    }

    inline int litToIdx(int lit) const {
        return (lit > 0) ? (lit << 1) : ((-lit) << 1 | 1);
    }

    void addWatch(int lit, int clauseID, int blockLit) {
        watches[litToIdx(lit)].push_back({clauseID, blockLit});
    }
};