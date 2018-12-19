#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <chrono>
#include <limits.h>
#include "ntial_rand.hpp"

using std::vector;
using std::string;

/****************************
 * Point member definitions *
 ****************************/
/**
 * Prints a point to the output stream
 */
std::ostream& operator<<(std::ostream &output, const NTIAL_RAND::Point &p) {
  output << "( x: " << p.x << ", y: " << p.y << ", a?: " << p.available << ", c?: " << p.chosen << " )";
  return output;
}


/***************************
 * NTIAL_RAND member definitions *
 ****************************/
/**
 * Constructs an instance of the No-Three-In-A-Line problem
 */
NTIAL_RAND::NTIAL_RAND(int N) {
  n = N;

  // Allocate memory
  grid = (Point*)malloc(n*n*sizeof(Point));

  // Initialize data
  init_data();

  // obtain a seed from the system clock
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  generator = std::default_random_engine (seed);
}

/**
 * Destroys an instance of the No-Three-In-A-Line problem
 */
NTIAL_RAND::~NTIAL_RAND() {
  free(grid);
}

/**
 * Initializes data structures. Assumes chosen is empty.
 */
void NTIAL_RAND::init_data() {
  // Fill data structures with points
  size_t offset;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      offset = IDX2(i, j, n);
      grid[offset].x = i;
      grid[offset].y = j;
      grid[offset].available = true;
      grid[offset].chosen = false;
    }
  }
  
  // Fill available array
  for (Point *p = grid; p < grid + n*n; p++) {
    available.push_back(p);
  }

}

/**
 * Resets an instantiated NTIAL_RAND problem.
 */
void NTIAL_RAND::reset() {
  // Clear data structures vector
  chosen.clear();
  available.clear();

  // Reinitialize data
  init_data();
}

/**
 * Prints current state of the grid to stdout. '*' indicates chosen points.
 */
void NTIAL_RAND::print_grid() {
  string border (n+2, '-');

  // Print top border
  std::cout << border << std::endl;

  for (int i = 0; i < n; i++) {
    // Print next row
    std::cout << '|';           
    for (int j = 0; j < n; j++) {
      if (grid[IDX2(i,j,n)].chosen) {
        std::cout << '*';           
      } else {
        std::cout << ' ';
      }
    }
    std::cout << '|' << std::endl;           
  }

  // Print bottom border
  std::cout << border << std::endl;
}

/**
 * Prints current list of available points.
 */
void NTIAL_RAND::print_available() {
  for (auto p : available) {
    std::cout << *p << std::endl;
  }
}

/**
 * Prints current list of chosen points.
 */
void NTIAL_RAND::print_chosen() {
  for (auto p : chosen) {
    std::cout << *p << std::endl;
  }
}

/**
 * FOR TESTING: returns the internal grid
 */
NTIAL_RAND::Point* NTIAL_RAND::get_grid() {
  return grid;
}

/**
 * Checks whether the point (x, y) is in the grid
 */
inline bool NTIAL_RAND::in_grid(int x, int y) {
  return (0 <= x) && (x < n) && (0 <= y) && (y < n);
}

/**
 * Deletes the point p
 */
inline void NTIAL_RAND::delete_point(Point * p) {
  p->available = false;
}

/**
 * Chooses the point p
 */
inline void NTIAL_RAND::choose_point(Point * p) {
  chosen.push_back(p);
  p->chosen = true;
}

/**
 * Chooses a random point from the available set
 */
NTIAL_RAND::Point* NTIAL_RAND::choose_next() {
  std::uniform_int_distribution<int> distribution(0, available.size() - 1);
  int rand_idx = distribution(generator);
  return available[rand_idx];
}

/**
 * Deletes point along line through p with given slope
 */
void NTIAL_RAND::delete_line(Point *p, int rise, int run) {
  int x, y;
  x = p->x + run;
  y = p->y + rise;
  while (in_grid(x, y)) {
    delete_point(grid+IDX2(x,y,n));
    x += run; 
    y += rise;
  }
  x = p->x - run;
  y = p->y - rise;
  while (in_grid(x, y)) {
    delete_point(grid+IDX2(x,y,n));
    x -= run; 
    y -= rise;
  }
}

/**
 * Deletes points that became invalid after adding point p
 */
void NTIAL_RAND::delete_invalid_points(Point * p) {
  int rise, run, gcd;
  for (auto q : chosen) {
    run  = p->x - q->x;
    rise = p->y - q->y;
    gcd = std::__gcd(run, rise);

    run  /= gcd;
    rise /= gcd;
    delete_line(p, rise, run);
  }
}


/**
 * Removes invalid entries from the available list
 */
void NTIAL_RAND::clear_unavailable() {
  int hi = available.size() - 1;
  int cur = 0;

  // Items to the left of cur are available
  while (cur <= hi)
  {
    if (available[cur]->available)
    {
      cur++;
    } else {
      // Move cur to end for deletion
      available[cur] = available[hi];
      hi--;
    }
  }

  // Delete points points from cur onwards
  while (cur < available.size()) {
    available.pop_back();
  }
}

/**
 * Chooses point p and updates state. Does not modify available.
 */
void NTIAL_RAND::process_point(Point* p) {
  delete_invalid_points(p);
  choose_point(p);
  delete_point(p);
}

/**
 * Attempts to find a maximal solution for the n*n No-Three-In-A-Line problem
 *   Returns number of points chosen
 */
int NTIAL_RAND::solve() {
  // Pick a random point q
  std::uniform_int_distribution<int> distribution(0, n-1);
  int rand_i = distribution(generator);
  int rand_j = distribution(generator);
  Point *p = grid+IDX2(rand_i, rand_j, n);

  /* std::cout << "Picked initial point: " << *p << std::endl; */

  // Remove q and update state
  choose_point(p);
  delete_point(p);
  clear_unavailable();

  // Iterate until all points are unavailable
  while (available.size() > 0) {
    p = choose_next();
    /* std::cout << "\nIterating on point: " << *p << std::endl; */
    process_point(p);
    clear_unavailable();
  }

  return chosen.size();
}

/**
 * Returns the point that is diagonally symmetric to p
 */
NTIAL_RAND::Point* NTIAL_RAND::get_diag_symm(Point* p) {
  return grid + IDX2(p->y,p->x,n);
}

/**
 * Attempts to find a maximal diagonally symmetric solution for the n*n No-Three-In-A-Line problem
 *   Returns number of points chosen
 */
int NTIAL_RAND::solve_diag_symm() {
  Point *p1, *p2;

  // Pick a random point
  std::uniform_int_distribution<int> distribution(0, n-1);
  int rand_i = distribution(generator);
  int rand_j = distribution(generator);
  p1 = grid+IDX2(rand_i, rand_j, n);

  /* std::cout << "Picked initial point: " << *p << std::endl; */

  // Remove first point
  choose_point(p1);
  delete_point(p1);

  // Remove mirror image
  p2 = get_diag_symm(p1);
  if (p1 == p2) {
    delete_line(p2, 1, -1);
  } else {
    process_point(p2);
  }

  clear_unavailable();

  // Iterate until all points are unavailable
  while (available.size() > 0) {
    // Choose new point
    p1 = choose_next();
    process_point(p1);

    // Choose mirror image
    p2 = get_diag_symm(p1);
    if (p1 == p2) {
      delete_line(p2, 1, -1);
    } else {
      process_point(p2);
    }

    clear_unavailable();
  }

  return chosen.size();
}
