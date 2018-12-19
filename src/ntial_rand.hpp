#ifndef NTIAL_RAND_H
#define NTIAL_RAND_H

#include <vector>
#include <list>
#include <random>

#define IDX2(i,j,i_stride) ((i)+(j*i_stride))

/**
 * An instance of the No-Three-In-A-Line problem
 */
class NTIAL_RAND {

  public:

    /**
     * A point in the n*n grid of the NTIAL_RAND problem
     */
    class Point {
      public:
        /**
         * Prints a point to the output stream
         */
        friend std::ostream& operator<<(std::ostream &output, const Point &p);

        int x;            // x-coordinate
        int y;            // y-coordinate
        int available;    // whether this point is available
        int chosen;       // whether this point has been chosen
    };

    /**
     * Constructs an instance of the No-Three-In-A-Line problem
     */
    explicit NTIAL_RAND(int n);

    /**
     * Destroys an instance of the No-Three-In-A-Line problem
     */
    ~NTIAL_RAND();

    /**
     * Prints current state of the grid to stdout. 'x' indicates chosen points.
     */
    void print_grid();

    /**
     * Prints current list of available points.
     */
    void print_available();

    /**
     * Prints current list of chosen points.
     */
    void print_chosen();

    /**
     * FOR TESTING: retruns the internal grid
     */
    Point* get_grid();
    
    /**
     * Resets an instantiated NTIAL_RAND problem.
     */
    void reset();

    /**
     * Attempts to find a maximal solution for the n*n No-Three-In-A-Line problem.
     *   Returns number of points chosen
     */
    int solve();

    /**
     * Attempts to find a maximal diagonally symmetric solution for the n*n No-Three-In-A-Line problem
     *   Returns number of points chosen
     */
    int solve_diag_symm();

  private:

    /**
     * Initializes data structures. Assumes chosen is empty.
     */
    void init_data();

    /**
     * Chooses the point p
     */
    inline void choose_point(Point * p);

    /**
     * Chooses a random point from the available set
     */
    inline Point* choose_next();

    /**
     * Deletes points that became invalid after adding point p
     */
    void delete_invalid_points(Point * p);

    /**
     * Deletes the point p
     */
    inline void delete_point(Point * p);

    /**
     * Deletes point along line through p with given slope
     */
    inline void delete_line(Point *p, int rise, int run);

    /**
     * Removes invalid entries from the available list
     */
    void clear_unavailable();

    /**
     * Chooses point p and updates state. Does not modify available.
     */
    inline void process_point(Point* p);

    /**
     * Checks whether the point (x, y) is in the grid
     */
    inline bool in_grid(int x, int y);

    /**
     * Returns the point that is diagonally symmetric to p
     */
    inline Point* get_diag_symm(Point* p);

    int n;                                // The size of the No-Three-In-A-Line problem
    Point* grid;                          // an n*n grid of Points
    std::vector<Point*> available;          // a list of the available points
    std::vector<Point*> chosen;           // a list of the chosen points
    std::default_random_engine generator; // the random number generator
};

#endif /* NTIAL_RAND_H */
