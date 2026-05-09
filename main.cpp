#include <iostream>
#include "solvers/IncludeSolvers.h"
#include "parser/Parser.cpp"

int main()
{
    // Tesztelendő CNF fájlok
    std::string files[] = {
        "cnf_examples/test/sat/bmc-1.cnf",
        "cnf_examples/test/sat/bmc-2.cnf",
        "cnf_examples/test/sat/bmc-3.cnf",
        "cnf_examples/test/sat/bmc-4.cnf",
        "cnf_examples/test/sat/bmc-5.cnf",
        "cnf_examples/test/sat/bmc-6.cnf",
        "cnf_examples/test/sat/bmc-7.cnf",
        "cnf_examples/test/sat/bmc-8.cnf",
        "cnf_examples/test/sat/bmc-9.cnf",
        "cnf_examples/test/sat/bmc-10.cnf",
        "cnf_examples/test/unsat/unsat1.cnf",
        "cnf_examples/test/unsat/unsat2.cnf",
        "cnf_examples/test/unsat/unsat3.cnf",
        "cnf_examples/test/unsat/unsat.cnf",
    };

    //BF
    /*for (const auto& filename : files) {
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
//stackDPLL
     for (const auto& filename : files) {
        std::cout << "Testing file with stackDPLL: " << filename << " ... ";

        Formula f = parseCNF(filename); // CNF fájl beolvasása
        stackDPLLSolver solver;                // Solver objektum
        SolveResult result = solver.solve(f);

        if (result == SolveResult::SAT)
            std::cout << "SAT\n";
        else
            std::cout << "UNSAT\n";
    }*/
    

//CDCL
     for (const auto& filename : files) {
        std::cout << "Testing file with CDCL: " << filename << " ... ";

        Formula f = parseCNF(filename); // CNF fájl beolvasása
        CDCLSolver solver;                // Solver objektum
        SolveResult result = solver.solve(f);

        if (result == SolveResult::SAT)
            std::cout << "SAT\n";
        else
            std::cout << "UNSAT\n";
    }
    return 0;
}