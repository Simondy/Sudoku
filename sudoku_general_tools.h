#ifndef SUDOKU_GENERAL_TOOLS_H_INCLUDED
#define SUDOKU_GENERAL_TOOLS_H_INCLUDED

/**
   The Sudoku is assumed to be stored in a bitfield containing the data.
   In a given cell represented by an unsigned number (preliminary a short)
   a 1 is represented by a non-zero LSB (0b00000001), a 2 a non-zero second
   LSB (0b00000010) and so on. The data is stored row-wise with three
   special unsigned numbers for Row, Column and Box-data.
   [Row Contains][Column Contains][Box ][Row1[Cell 1..9]][Row2[..]][Last Row[1..9]]
*/

// Defining numbers
#define ZERO        (0x0)
#define ONE         (0x1)
#define TWO         (0x2)
#define THREE       (0x4)
#define FOUR        (0x8)
#define FIVE        (0x10)
#define SIX         (0x20)
#define SEVEN       (0x40)
#define EIGHT       (0x80)
#define NINE        (0x100)
#define TEN         (0x200)
#define ELEVEN      (0x400)
#define TWELVE      (0x800)
#define THIRTEEN    (0x1000)
#define FOURTEEN    (0x2000)
#define FIFTEEN     (0x4000)

// These two could easily be flags set at the beginning of the program rather
// than at pre processing time.
#define SUDOKU_SIZE (9)
#define SUDOKU_BOX_WIDTH (3)

// FLAGS

// Checks
typedef unsigned short su_cell_t; // Type used for storing cell values.
typedef unsigned int su_rcb_t;  // Type used to store row, column or box values.

/**

*/
typedef struct sudoku_field sudoku_field;
struct sudoku_field {
    su_cell_t* row_contains;    // is used to reference the whole internal  memory arena.
    su_cell_t* col_contains;
    su_cell_t* box_contains;
    su_cell_t* field;
};

/**
    Checks wether or not a row already contain a number of a certain value.
    @param {sudokufield} f - The input sudoku field.
    @param {su_rcb_t} r - The row to be checked.
    @param {su_cell_t} input - The input to be looked for.
    @return {su_cell_t} 0 if the number doesn't exist in the row. Non-zero otherwise.
*/
su_cell_t check_row(     const sudoku_field*, su_rcb_t r, su_cell_t);
/**
    Checks wether or not a column already contain a number of a certain value.
    @param {sudokufield} f - The input sudoku field.
    @param {su_rcb_t} c - The column to be checked.
    @param {su_cell_t} input - The input to be looked for.
    @return {su_cell_t} 0 if the number doesn't exist in the column. Non-zero otherwise.
*/
su_cell_t check_column(  const sudoku_field*, su_rcb_t c, su_cell_t);
/**
    Checks wether or not a box already contain a number of a certain value.
    @param {sudokufield} f - The input sudoku field.
    @param {su_rcb_t} b - The box to be checked.
    @param {su_cell_t} input - The input to be looked for.
    @return {su_cell_t} 0 if the number doesn't exist in the box. Non-zero otherwise.
*/
su_cell_t check_box(     const sudoku_field*, su_rcb_t b, su_cell_t);
/**
    Checks row, column and box as if rows and columns are stored in
    a single long array.
    @param {sudokufield} f - The input sudoku field.
    @param {int} rc - The position of the cell in a linear memory arena.
    @param {su_cell_t} input - The input to be looked for.
    @return {su_cell_t} 0 if the number doesn't exist in the box. Non-zero otherwise.
*/
su_cell_t check_all_lin(const sudoku_field*, int rc, su_cell_t);

/**
    Gets the value in a given cell.
    @param {sudokufield} f - The input sudoku field.
    @param {su_rcb_t} r - The row of the cell.
    @param {su_rcb_t} c - The column of the cell.
    @return {su_cell_t} - The value of the cell.
*/
su_cell_t get_cell( const sudoku_field*, su_rcb_t r, su_rcb_t c);
/**
    Gets the value in a given cell.
    @param {sudokufield} f - The input sudoku field.
    @param {int} rc - The position of the cell in a linear memory arena.
    @return {su_cell_t} - The value of the cell.
*/
su_cell_t get_cell_lin( const sudoku_field*, int rc);


/**
    Converts the rc value to row index, where rc is the cell position in a
    linear memory arena.

    @param {int} rc - Cell position in the arena.
    @return {su_cell_t} - Row index.
*/
su_rcb_t rc_to_r(int rc);
/**
    Converts the rc value to column index, where rc is the cell position in a
    linear memory arena.

    @param {int} rc - Cell position in the arena.
    @return {su_cell_t} - Column index.
*/
su_rcb_t rc_to_c(int rc);
/**
    Converts the rc value to box index, where rc is the cell position in a
    linear memory arena.

    @param {int} rc - Cell position in the arena.
    @return {su_cell_t} - Box index.
*/
su_rcb_t rc_to_b(int rc);
/**
    Converts the row and column indices to rc index, where rc is the cell
    position in a linear memory arena.

    @param {su_cell_t} r - Row index.
    @param {su_cell_t} b - Column index.
    @return {int} - rc index.
*/
int      r_and_c_to_rc(su_rcb_t r, su_rcb_t c);

/**
    Converts the r and c values to the box containing the cell at positon r,c
    @param {su_rcb_t} r - The row of the cell.
    @param {su_rcb_t} c - The column of the cell.
    @return {su_rcb_t} - The value of box.
*/
su_rcb_t r_and_c_to_b(su_rcb_t r, su_rcb_t c);

// Solving

/**
    Tries to set the value in a given cell. Fails if the number is already
    present in the row, column or box.
    @param {sudokufield} f - The input sudoku field.
    @param {su_rcb_t} r - The row of the cell.
    @param {su_rcb_t} c - The column of the cell.
    @param {su_cell_t} - The value of the cell.
    @return {int} - 0 if unsuccessful. Non-zero otherwise.
*/
int set_cell(sudoku_field* f, su_rcb_t r, su_rcb_t c, su_cell_t);
/**
    Tries to set the value in a given cell as if rows and columns are stored in
    a single long array. Fails if the number is already present
    in the row, column or box.
    @param {sudokufield} f - The input sudoku field.
    @param {int} rc - The position of the cell in a linear memory arena.
    @param {su_cell_t} - The value of the cell.
    @return {int} - 0 if unsuccessful. Non-zero otherwise.
*/
int set_cell_lin(sudoku_field*, int rc, su_cell_t nbr);

/**
    Unsets the value in a given cell.

    WARNING! Assumes the value is already validly placed.

    @param {sudokufield} f - The input sudoku field.
    @param {su_rcb_t} r - The row of the cell.
    @param {su_rcb_t} c - The column of the cell.
    @param {su_cell_t} - The value to unset from the cell.
*/
void unset_cell(sudoku_field* f, su_rcb_t r, su_rcb_t c, su_cell_t nbr);
/**
    Unsets the value in a given cell.

    WARNING! Assumes the value is already validly placed.

    @param {sudokufield} f - The input sudoku field.
    @param {int} r - The position of the cell in a linear memory arena.
    @param {su_cell_t} - The value to unset from the cell.
*/
void unset_cell_lin(sudoku_field* f, int rc, su_cell_t nbr);

/**
    Initiates and starts solving of a sudoku field. Returns a new sudoku field.

    @param {sudoku_field*} f - Input sudoku to solve.
    @return {sudoku_field*} - A solved sudoku if successful. NULL otherwise.
*/
sudoku_field*   init_sudoku_solve( const sudoku_field*);
/**
    Tries to solve the input sudoku recursively. Returns the input sudoku field
    (solved) if successful and NULL otherwise. Caution is therefore needed to
    avoid memory leaks.

    NOT CURRENTLY IMPLEMENTED! Use sudoku_solve_rec instead.

    @param {sudoku_field*} f - Input sudoku to solve.
    @param {su_cell_t} next_r - Row index of the next cell to test.
    @param {su_cell_t} next_c - Row index of the next cell to test.
    @return {sudoku_field*} - A solved sudoku if successful. NULL otherwise.
*/
sudoku_field*   sudoku_solve_rec(sudoku_field* f, su_rcb_t next_r, su_rcb_t next_c);
/**
    Tries to solve the input sudoku recursively. Returns the input sudoku field
    (solved) if successful and NULL otherwise. Caution is therefore needed to
    avoid memory leaks.

    Assumes that the sudoku field data is stored in a single array.

    @param {sudoku_field*} f - Input sudoku to solve.
    @param {int} rc - rc index of the next cell to test.
    @return {sudoku_field*} - A solved sudoku if successful. NULL otherwise.
*/
sudoku_field*   sudoku_solve_rec_lin(sudoku_field* ,int);

// Support functions
/**
    This function is supposed check wether or not a sudoku is valid before even
    trying to complete it.
    IT DOES NOT CURRENTLY DO THAT.
    @param {sudoku_field*} f - A pointer to the field.s
    @return {int} - 1 if seemingly valid, 0 otherwise.
*/
int             sudoku_valid(const sudoku_field*);
/**
    Get a sudoku field from a string or char array. Currently only designed
    for 9x9 or 16x16 sudokus. Also the sudoku size currently set by a size
    define in the sudoku_general_tools header

    @param {char*} s - The input string.
    @return {sudoku_field} - The return sudoku field.
*/
sudoku_field*   sudoku_from_input(const char*);
/**
    This function is supposed to return a string containing the sudoku.
    IT DOES NOT CURRENTLY DO THAT.
    @param {sudoku_field*} f - A pointer to the field.s
    @return {char*} - A string containing the sudoku.
*/
char*           sudoku_to_string(sudoku_field*);
/**
    Print a sudoku field. Currently only designed for 9x9 or 16x16 sudokus.

    @param {sudoku_field} s - The sudoku field to be printed.
    @return {int} - 0 if successful. Non zero otherwise.
*/
int print_sudoku_from_field(const sudoku_field*);

/**
    Allocates the memory for a sudoku field, including the internal arena.

    @return {sudoku_field*} - A pointer to the new arena.
*/
sudoku_field* alloc_sudoku_field();
/**
    Allocates the  internal memory of the sudoku field as a cleared arena.

    @param {sudoku_field*} s - The field to get a new arena.
    @return {int} - 0 if successful. Non zero otherwise..
*/
int alloc_sudoku_field_internal_arena(sudoku_field*);
/**
    Frees the memory of  a sudoku field.
    @param {sudoku_field*} f - A pointer to the field to be freed.
*/
void free_sudoku_field(sudoku_field*);
/**
    Frees the internal memory of a sudoku field.
    @param {sudoku_field*} f - A pointer to the field.
*/
void free_sudoku_field_internal_arena(sudoku_field*);
/**
    Returns a pointer to a complete copy of the input sudoku field.
    @param {sudoku_field*} f - A pointer to the field to be copied.
*/
sudoku_field* copy_sudoku_field(const sudoku_field* from);

#endif // SUDOKU_GENERAL_TOOLS_H_INCLUDED
