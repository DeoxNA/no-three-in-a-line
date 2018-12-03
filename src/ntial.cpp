#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
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
  output << "( x: " << p.x << ", y: " << p.y << ", dr: " << p.delete_rank << ", c?: " << p.chosen << " )";
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
NTIAL::NTIAL(unsigned int N) {
  n = N;

  // Allocate memory
  grid = (Point*)malloc(n*n*sizeof(Point));

  // Initialize data
  init_data();

  // Fill available array
  for (Point *p = grid; p < grid + n*n; p++) {
    available.push_back(p);
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
 * Initializes data structures. Assumes chosen is empty.
 */
void NTIAL::init_data() {
  // Fill data structures with points
  size_t offset;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      offset = IDX2(i, j, n);
      grid[offset].x = i;
      grid[offset].y = j;
      grid[offset].delete_rank = 0;
      grid[offset].chosen = false;
    }
  }
}

/**
 * Resets an instantiated NTIAL problem.
 */
void NTIAL::reset() {
  // Empty chosen vector
  chosen.clear();

  // Reinitialize data
  init_data();
}

/**
 * Prints current state of the grid to stdout. 'x' indicates chosen points.
 */
void NTIAL::print_grid() {
  string border (n+2, '-');

  // Print top border
  std::cout << border << std::endl;

  for (int i = 0; i < n; i++) {
    // Print next row
    std::cout << '|';           
    for (int j = 0; j < n; j++) {
      if (grid[IDX2(i,j,n)].chosen) {
        std::cout << 'x';           
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
void NTIAL::print_available() {
  for (auto p : available) {
    if (p->delete_rank < INT_MAX) {
      std::cout << *p << std::endl;
    }
  }
}

/**
 * Prints current list of chosen points.
 */
void NTIAL::print_chosen() {
  for (auto p : chosen) {
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
 * Sorts the list of available points.
 */
void NTIAL::sort_available() {
  std::sort(available.begin(), available.end(), Point::PointCompare);
}


/**
 * Checks whether the point (x, y) is in the grid
 */
inline bool NTIAL::in_grid(int x, int y) {
  return (0 <= x) && (x < n) && (0 <= y) && (y < n);
}

/**
 * Deletes the point p
 */
inline void NTIAL::delete_point(Point * p) {
  p->delete_rank = INT_MAX;
}

/**
 * Chooses the point p
 */
inline void NTIAL::choose_point(Point * p) {
  chosen.push_back(p);
  p->chosen = true;
}

/**
 * Chooses a random point from those with the least delete rank.
 */
Point* NTIAL::choose_next() {
  int min_dr = available[0]->delete_rank;
  int i = 1;
  while (min_dr == available[i]->delete_rank) {
    i++;
  } 
  std::uniform_int_distribution<int> distribution(0, i-1);
  int rand_idx = distribution(generator);
  return available[rand_idx];
}

/**
 * Deletes points that became invalid after adding point p
 */
void NTIAL::delete_invalid_points(Point * p) {
  // TODO Shida do your thing
  int rise, run, x, y, gcd;
  for (auto q : chosen) {
    run  = p->x - q->x;
    rise = p->y - q->y;
    gcd = std::__gcd(run, rise);
    run  /= gcd;
    rise /= gcd;

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
}

/**
 * Updates the delete rank after choosing point p
 */
void NTIAL::update_delete_rank(Point * p) {
  // TODO USE GCD
  // The number of lattice points on a line with end points (a,b) and (b,c)
  //   is gcd(c-a, d-b) + 1
  // Source: https://math.stackexchange.com/questions/628117/how-to-count-lattice-points-on-a-line
  int rise, run, x, y, gcd, num_points;
  Point *q;
  /* #pragma omp parallel for collapse(2) private(q, rise, run, x, y, gcd, num_points) */
  for (int j = 0; j < n; j++) {
    for (int i = 0; i < n; i++) {
      q = grid + IDX2(i,j,n);
      // Skip unavailable points
      if (q->delete_rank < INT_MAX) { 
        num_points = 0;
        run   = p->x - i;
        rise  = p->y - j;
        gcd = std::__gcd(run, rise);
        run  /= gcd;
        rise /= gcd;

        x = p->x + run;
        y = p->y + rise;
        while (in_grid(x, y)) {
          if (grid[IDX2(x,y,n)].delete_rank < INT_MAX) {
            num_points++;
          }
          x += run; 
          y += rise;
        }
        x = p->x - run;
        y = p->y - rise;
        while (in_grid(x, y)) {
          if (grid[IDX2(x,y,n)].delete_rank < INT_MAX) {
            num_points++;
          }
          x -= run; 
          y -= rise; 
        }
        // Update delete rank (excludes the point being considered)
        /* #pragma omp atomic */
        q->delete_rank += num_points - 1;
      }
    }
  }
}

/**
 * Attempts to find a maximal solution for the n*n No-Three-In-A-Line problem
 */
bool NTIAL::solve() {
  // Pick a random point q
  std::uniform_int_distribution<int> distribution(0, n-1);
  int rand_i = distribution(generator);
  int rand_j = distribution(generator);
  Point *q = grid+IDX2(rand_i, rand_j, n);

  /* std::cout << "Picked initial point\n" << *q << std::endl; */

  // Remove q and update state
  choose_point(q);
  delete_point(q);
  update_delete_rank(q);
  sort_available();

  // Iterate until all points are unavailable
  Point *p;
  while (available[0]->delete_rank < INT_MAX) {
    p = choose_next();
    /* std::cout << "Iterating on point: " << *p << std::endl; */
    delete_invalid_points(p);
    choose_point(p);
    delete_point(p);
    update_delete_rank(p);
    sort_available();
  }

  return (chosen.size() >= 2*n);
}
