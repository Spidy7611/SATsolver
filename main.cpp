#include <iostream>
#include "solvers/IncludeSolvers.h"
#include "parser/Parser.cpp"

int main()
{
    // Tesztelendő CNF fájlok
    std::string files[] = {
      // "cnf_examples/aloul-chnl11-13.cnf", 
"cnf_examples/anbul-dated-5-15-u.cnf", 
"cnf_examples/anbul-part-10-13-s.cnf", 
"cnf_examples/anbul-part-10-15-s.cnf", 
"cnf_examples/babic-dspam-vc1080.cnf", 
"cnf_examples/babic-dspam-vc949.cnf", 
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