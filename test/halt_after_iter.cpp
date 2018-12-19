#include <iostream>
/* #include "../src/ntial_dr.hpp" */
#include "../src/ntial_rand.hpp"

int main () {
    int n;
    std::cout << "n: ";
    std::cin >> n;
    
    int runs;
    std::cout << "runs: ";
    std::cin >> runs;
    
    bool solved = false;
    
    #pragma omp parallel for private(solved)
    for (int i = 0; i < runs; i++) {
      // NTIAL_DR prob (n);
      NTIAL_RAND prob (n);
      solved = prob.solve();
      if (solved) {
        prob.print_grid();
      }
    }
}
