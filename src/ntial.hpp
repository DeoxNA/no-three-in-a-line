#ifndef NTIAL_H
#define NTIAL_H

#include <vector>
#include <random>

#define IDX2(i,j,i_stride) ((i)+(j*i_stride))

/**
 * A point in the n*n grid of the NTIAL problem
 */
class Point {
  public:
    /**
     * Prints a point to the output stream
     */
    friend std::ostream& operator<<(std::ostream &output, const Point &p);
    
    /**
     * Compares references to Points by comparing the delete_rank of the objects they point to.
     */
    static bool PointCompare(Point* a, Point* b);

    int x;            // x-coordinate
    int y;            // y-coordinate
    int delete_rank;  // number of points become invalid after chosing this point
    bool deleted;     // whether this point has been deleted
};

/**
 * An instance of the No-Three-In-A-Line problem
 */
class NTIAL {

  public:
    /**
     * Constructs an instance of the No-Three-In-A-Line problem
     */
    explicit NTIAL(int n);

    /**
     * Destroys an instance of the No-Three-In-A-Line problem
     */
    ~NTIAL();

    /**
     * Prints current state of the grid to the terminal. 'x' indicates chosen points.
     */
    void print_grid();

    /**
     * Prints current list of available points.
     */
    void print_available();

    /**
     * Sorts the list of available points.
     */
    void sort_available();

    /**
     * FOR TESTING: retruns the internal grid
     */
    Point* get_grid();

    /**
     * Deletes the point at (i,j)
     */
    void delete_point(const int i, const int j);
    
    /**
     * Attempts to find a maximal solution for the n*n No-Three-In-A-Line problem
     */
    void solve();

  private:

    int n;                                // The size of the No-Three-In-A-Line problem
    Point* grid;                          // an n*n grid of Points
    std::vector<Point*> available;        // a list of the available points
    std::vector<Point*> chosen;           // a list of the chosen points
    std::default_random_engine generator; // the random number generator
};

#endif /* NTIAL_H */
