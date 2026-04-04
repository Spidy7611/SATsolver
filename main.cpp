#include <iostream>
#include "solvers/IncludeSolvers.h"
#include "parser/Parser.cpp"

int main()
{
    // Tesztelendő CNF fájlok
    std::string files[] = {
        "cnf_examples/example.cnf",
        "cnf_examples/sat_example.cnf",
        "cnf_examples/unsat_example.cnf"
    };
//BF
    for (const auto& filename : files) {
        std::cout << "Testing file with BF: " << filename << " ... ";

        Formula f = parseCNF(filename); // CNF fájl beolvasása
        BFSolver solver;                // Solver objektum
        SolveResult result = solver.solve(f);

        if (result == SolveResult::SAT)
            std::cout << "SAT\n";
        else
            std::cout << "UNSAT\n";
    }

//DPLL
     for (const auto& filename : files) {
        std::cout << "Testing file with DPLL: " << filename << " ... ";

        Formula f = parseCNF(filename); // CNF fájl beolvasása
        DPLLSolver solver;                // Solver objektum
        SolveResult result = solver.solve(f);

        if (result == SolveResult::SAT)
            std::cout << "SAT\n";
        else
            std::cout << "UNSAT\n";
    }
    return 0;
}