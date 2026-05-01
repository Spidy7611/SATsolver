#pragma once
#include <vector>
#include <algorithm>

struct Watcher {
    int clauseID;
    int blockingLiteral;
};

class CDCLFormula {
public:
    int numVars;
    // Az eredeti és a tanult klózok egy helyen vagy külön is lehetnek. 
    // Itt most egyben kezeljük őket a sebesség miatt.
    std::vector<std::vector<int>> clauses;
    
    std::vector<std::vector<Watcher>> watches;
    std::vector<int> assignments; // 0, 1, -1
    std::vector<int> trail;
    
    // --- ÚJ CDCL SPECIFIKUS MEZŐK ---

    // 1. Döntési szintek: Melyik szinten lett beállítva a változó?
    // (Pl. level[5] = 2 azt jelenti, hogy az x5 változó a 2. döntésnél kapott értéket)
    std::vector<int> decisionLevels;

    // 2. Indoklás (Antecedent): Melyik klóz kényszerítette ezt a változót?
    // Ha döntés volt, akkor -1. Ha unit propagation, akkor a clauseID.
    std::vector<int> reasons;

    // 3. Aktivitás (VSIDS-hez): Melyik változó szerepel sok konfliktusban?
    std::vector<double> activity;

    // 4. Trail limit: Hol kezdődnek az egyes döntési szintek a trail-en?
    // controllstack helyett
    std::vector<int> trailLim;

    int qhead = 0;

    CDCLFormula(int n, const std::vector<std::vector<int>>& c) : numVars(n), clauses(c) {
        assignments.assign(numVars + 1, 0);
        decisionLevels.assign(numVars + 1, -1);
        reasons.assign(numVars + 1, -1);
        activity.assign(numVars + 1, 0.0);
        
        trail.reserve(numVars);
        trailLim.reserve(numVars);
        watches.resize(2 * numVars + 2);
        
        // Kezdeti 2WL (mint a DPLL-nél)
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

    // Új klóz hozzáadása futás közben (tanuláskor)
    void addLearnedClause(const std::vector<int>& newClause) {
        int newID = clauses.size();
        clauses.push_back(newClause);
        // A tanult klózt is azonnal figyelni kell!
        if (newClause.size() >= 2) {
            addWatch(newClause[0], newID, newClause[1]);
            addWatch(newClause[1], newID, newClause[0]);
        }
    }
};