#include <iostream>
#include "ntial.hpp"

int main () {
    int n = 4;
    NTIAL prob (n);
    
    Point* grid = prob.get_grid();

    prob.delete_point(grid+IDX2(2,2,n));

    prob.print_grid();
    prob.print_available();
    
    std::cout << "\nNEW DELETE RANK\n" << std::endl;

    grid[IDX2(1,2,n)].delete_rank = 1;
    grid[IDX2(2,1,n)].delete_rank = 2;
    grid[IDX2(3,3,n)].delete_rank = 4;
    grid[IDX2(2,1,n)].delete_rank = -1;

    prob.sort_available();
    prob.print_available();
    
    std::cout << "\nSIZE OF Point: " << sizeof(Point) << std::endl;

    prob.solve();
    
    std::cout << "\nSOLUTION\n" << std::endl;
    prob.print_grid();
    std::cout << "\nAVAILABLE:" << std::endl;
    prob.print_available();
    std::cout << "\nCHOSEN:" << std::endl;
    prob.print_chosen();
}
