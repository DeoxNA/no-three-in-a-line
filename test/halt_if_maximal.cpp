#include <iostream>
/* #include "../src/ntial_dr.hpp" */
#include "../src/ntial_rand.hpp"

int main () {
    int n;
    std::cout << "n: ";
    std::cin >> n;
    
    #pragma omp parallel shared(n)
    {
        int chosen;
        // NTIAL_DR prob (n);
        NTIAL_RAND prob (n);
    
        bool solved = false;
    

        while (!solved) {
        prob.reset();
        chosen = prob.solve_diag_symm();
        // chosen = prob.solve();
        solved = (chosen == 2*n);
        }
    
        #pragma omp critical 
        {
        prob.print_grid();
        std::cout << "Chosen: " << chosen << std::endl;
        }
    }
}
