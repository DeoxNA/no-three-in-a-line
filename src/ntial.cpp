#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits.h>
#include "ntial.hpp"

using std::vector;
using std::string;

/****************************
 * Point member definitions *
 ****************************/
/**
 * Prints a point to the output stream
 */
std::ostream& operator<<(std::ostream &output, const Point &p) {
  output << "( x: " << p.x << ", y: " << p.y << ", dr: " << p.delete_rank << ", d?: " << p.deleted << " )";
  return output;
}

/**
 * Compares references to Points by comparing the delete_rank of the objects they point to.
 */
bool Point::PointCompare(Point* a, Point* b) {
  return a->delete_rank < b->delete_rank;
}


/***************************
 * NTIAL member definitions *
 ****************************/
/**
 * Constructs an instance of the No-Three-In-A-Line problem
 */
NTIAL::NTIAL(int N) {
  n = N;

  // Allocate memory
  grid = (Point*)malloc(n*n*sizeof(Point));

  // Fill data structures with points
  size_t offset;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      offset = IDX2(i, j, n);
      grid[offset].x = i;
      grid[offset].y = j;
      grid[offset].delete_rank = 0;
      grid[offset].deleted = false;

      available.push_back(grid+offset);
    }
  }

  // obtain a seed from the system clock
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  generator = std::default_random_engine (seed);
}

/**
 * Destroys an instance of the No-Three-In-A-Line problem
 */
NTIAL::~NTIAL() {
  free(grid);
}

/**
 * Prints current state of the grid to the terminal. 'x' indicates non-delted points.
 */
void NTIAL::print_grid() {
  string border (n+2, '-');

  // Print top border
  std::cout << border << std::endl;

  for (int i = 0; i < n; i++) {
    // Print next row
    std::cout << '|';           
    for (int j = 0; j < n; j++) {
      if (grid[IDX2(i,j,n)].deleted) {
        std::cout << ' ';           
      } else {
        std::cout << 'x';
      }
    }
    std::cout << '|' << std::endl;           
  }

  // Print bottom border
  std::cout << border << std::endl;
}

/**
 * Sorts the list of available points.
 */
void NTIAL::sort_available() {
  std::sort(available.begin(), available.end(), Point::PointCompare);
}

/**
 * Prints current list of available points.
 */
void NTIAL::print_available() {
  for (auto p : available) {
    std::cout << *p << std::endl;
  }
}

/**
 * FOR TESTING: returns the internal grid
 */
Point* NTIAL::get_grid() {
  return grid;
}

/**
 * Deletes the point at (i,j)
 */
void NTIAL::delete_point(const int i, const int j) {
  grid[IDX2(i,j,n)].delete_rank = INT_MAX;
  grid[IDX2(i,j,n)].deleted     = true;
}

/**
 * Attempts to find a maximal solution for the n*n No-Three-In-A-Line problem
 */
void NTIAL::solve() {
  // Pick a random point
  std::uniform_int_distribution<int> distribution(0,n-1);
  int rand_i = distribution(generator);
  int rand_j = distribution(generator);
  Point p = grid[IDX2(rand_i,rand_j,n)];

  std::cout << "Picked initial point\n" << p << std::endl;

  // TODO updating delete rank must account for INT_MAX overflow
}
