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
    int delete_rank;  // number of points become invalid after chosing this point. 
                      //   A delete_rank of INT_MAX indicates that a point is unavailable 
                      //   (either deleted or already chosen)
    bool chosen;      // whether this point has been chosen
};

/**
 * An instance of the No-Three-In-A-Line problem
 */
class NTIAL {

  public:
    /**
     * Constructs an instance of the No-Three-In-A-Line problem
     */
    explicit NTIAL(unsigned int n);

    /**
     * Destroys an instance of the No-Three-In-A-Line problem
     */
    ~NTIAL();

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
     * Sorts the list of available points.
     */
    void sort_available();
    
    
    /**
     * Deletes points that became invalid after adding point p
     */
    // TODO Shida do your thing
    void delete_invalid_points(Point * p);

    /**
     * Updates the delete rank after choosing point p
     */
    void update_delete_rank(Point * p);
    
    /**
     * Attempts to find a maximal solution for the n*n No-Three-In-A-Line problem.
     *   Returns true if a maximal solution was found.
     */
    bool solve();

    /**
     * Resets an instantiated NTIAL problem.
     */
    void reset();

  private:

    /**
     * Initializes data structures. Assumes chosen is empty.
     */
    void init_data();

    /**
     * Deletes the point p
     */
    inline void delete_point(Point * p);

    /**
     * Chooses the point p
     */
    inline void choose_point(Point * p);

    /**
     * Chooses a random point from those with the least delete rank.
     */
    Point* choose_next();

    /**
     * Checks whether the point (x, y) is in the grid
     */
    inline bool in_grid(int x, int y);

    unsigned int n;                       // The size of the No-Three-In-A-Line problem
    Point* grid;                          // an n*n grid of Points
    std::vector<Point*> available;        // a list of the available points
    std::vector<Point*> chosen;           // a list of the chosen points
    std::default_random_engine generator; // the random number generator
};

#endif /* NTIAL_H */
