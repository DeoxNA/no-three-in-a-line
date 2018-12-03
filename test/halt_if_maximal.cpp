#include <iostream>
#include "../src/ntial.hpp"

int main () {
    int n;
    std::cout << "n: ";
    std::cin >> n;
    
    NTIAL prob (n);
    
    bool solved = false;
    
    while (!solved) {
      prob.reset();
      solved = prob.solve();
    }

    prob.print_grid();
}
