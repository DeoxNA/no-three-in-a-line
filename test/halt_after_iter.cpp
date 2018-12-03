#include <iostream>
#include "../src/ntial.hpp"

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
      NTIAL prob (n);
      solved = prob.solve();
      if (solved) {
        prob.print_grid();
      }
    }
}
