// Sudoku solver solving tools
#include <stdio.h>      // For standard IO
#include <stdlib.h>     // For standard stuff


#include "sudoku_general_tools.h"

/**
    Tools directly involved in the sudoku solving.
*/


/**
    Checks wether or not a row already contain a number of a certain value.
    @param {sudokufield} f - The input sudoku field.
    @param {su_rcb_t} r - The row to be checked.
    @param {su_cell_t} input - The input to be looked for.
    @return {su_cell_t} 0 if the number doesn't exist in the row. Non-zero otherwise.
*/
su_cell_t check_row(    const sudoku_field* f, su_rcb_t r, su_cell_t input){
    return ( *(f->row_contains + r) & input);
}
/**
    Checks wether or not a column already contain a number of a certain value.
    @param {sudokufield} f - The input sudoku field.
    @param {su_rcb_t} c - The column to be checked.
    @param {su_cell_t} input - The input to be looked for.
    @return {su_cell_t} 0 if the number doesn't exist in the column. Non-zero otherwise.
*/
su_cell_t check_column( const sudoku_field* f, su_rcb_t c, su_cell_t input){
    return ( *(f->col_contains + c) & input);
}
/**
    Checks wether or not a box already contain a number of a certain value.
    @param {sudokufield} f - The input sudoku field.
    @param {su_rcb_t} b - The box to be checked.
    @param {su_cell_t} input - The input to be looked for.
    @return {su_cell_t} 0 if the number doesn't exist in the box. Non-zero otherwise.
*/
su_cell_t check_box(    const sudoku_field* f, su_rcb_t b, su_cell_t input){
    return ( *(f->box_contains + b) & input);
}

/**
    Checks row, column and box as if rows and columns are stored in
    a single long array.
    @param {sudokufield} f - The input sudoku field.
    @param {int} rc - The position of the cell in a linear memory arena.
    @param {su_cell_t} input - The input to be looked for.
    @return {su_cell_t} 0 if the number doesn't exist in the box. Non-zero otherwise.
*/
su_cell_t check_all_lin(    const sudoku_field* f, int rc, su_cell_t input){
    return (check_row(      f,  rc_to_r(rc), input)    &&
            check_column(   f,  rc_to_c(rc), input)    &&
            check_box(      f,  rc_to_b(rc), input)    );
}

/**
    Gets the value in a given cell.
    @param {sudokufield} f - The input sudoku field.
    @param {su_rcb_t} r - The row of the cell.
    @param {su_rcb_t} c - The column of the cell.
    @return {su_cell_t} - The value of the cell.
*/
su_cell_t get_cell( const sudoku_field* f, su_rcb_t r, su_rcb_t c){
    return get_cell_lin(f, r * SUDOKU_SIZE + c);
}
/**
    Gets the value in a given cell.
    @param {sudokufield} f - The input sudoku field.
    @param {int} rc - The position of the cell in a linear memory arena.
    @return {su_cell_t} - The value of the cell.
*/
su_cell_t get_cell_lin( const sudoku_field* f, int rc){
    return *(f->field + rc);
}

/**
    Converts the r and c values to the box containing the cell at positon r,c
    @param {su_rcb_t} r - The row of the cell.
    @param {su_rcb_t} c - The column of the cell.
    @return {su_rcb_t} - The value of box.
*/
su_rcb_t r_and_c_to_b(su_rcb_t r, su_rcb_t c){
    return SUDOKU_BOX_WIDTH*(r/SUDOKU_BOX_WIDTH) + c/SUDOKU_BOX_WIDTH;
}
su_rcb_t rc_to_b(int rc){
    return r_and_c_to_b(rc_to_r(rc), rc_to_c(rc));
}
su_rcb_t rc_to_r(int rc){
    return rc/SUDOKU_SIZE;
}
su_rcb_t rc_to_c(int rc){
    return rc%SUDOKU_SIZE;
}
int r_and_c_to_rc(su_rcb_t r, su_rcb_t c){
    return (int) r*SUDOKU_SIZE + c;
}

// Solving

/**
    Tries to set the value in a given cell. Fails if the number is already
    present in the row, column or box.
    @param {sudokufield} f - The input sudoku field.
    @param {su_rcb_t} r - The row of the cell.
    @param {su_rcb_t} c - The column of the cell.
    @param {su_cell_t} - The value of the cell.
    @return {int} - 0 if successful. Non-zero otherwise.
*/
int set_cell(sudoku_field* f, su_rcb_t r, su_rcb_t c, su_cell_t input){
    int b = r_and_c_to_b(r,c);
    if( check_row(      f, r, input) ||
        check_column(   f, c, input) ||
        check_box(      f, b, input) ){
            return 1;
    }
    *(f->row_contains + r)           = *(f->row_contains + r) | input;
    *(f->col_contains + c)           = *(f->col_contains + c) | input;
    *(f->box_contains + b)           = *(f->box_contains + b) | input;
    *(f->field + r_and_c_to_rc(r,c)) = input;
    return 0;
}
/**
    Tries to set the value in a given cell as if rows and columns are stored in
    a single long array. Fails if the number is already present
    in the row, column or box.

    @param {sudokufield} f - The input sudoku field.
    @param {int} rc - The position of the cell in a linear memory arena.
    @param {su_cell_t} - The value of the cell.
    @return {int} - 0 if successful. Non-zero otherwise.
*/
int set_cell_lin(sudoku_field* f, int rc, su_cell_t input){
    return set_cell(f, rc_to_r(rc), rc_to_c(rc), input);
}

/**
    Unsets the value in a given cell.

    WARNING! Assumes the value is already validly placed.

    @param {sudokufield} f - The input sudoku field.
    @param {su_rcb_t} r - The row of the cell.
    @param {su_rcb_t} c - The column of the cell.
    @param {su_cell_t} - The value to unset from the cell.
*/
void unset_cell(sudoku_field* f, su_rcb_t r, su_rcb_t c, su_cell_t nbr){
    int rc = r_and_c_to_rc(r, c);
    int b = r_and_c_to_b(r,c);
    *(f->row_contains + r) = *(f->row_contains + r)  ^ nbr; // xor
    *(f->col_contains + c) = *(f->col_contains + c)  ^ nbr;
    *(f->box_contains + b) = *(f->box_contains + b)  ^ nbr;
    *(f->field+ rc)        = *(f->field        + rc) ^ nbr;
}
/**
    Unsets the value in a given cell.

    WARNING! Assumes the value is already validly placed.

    @param {sudokufield} f - The input sudoku field.
    @param {int} r - The position of the cell in a linear memory arena.
    @param {su_cell_t} - The value to unset from the cell.
*/
void unset_cell_lin(sudoku_field* f, int rc, su_cell_t nbr){
    int r = rc_to_r(rc);
    int c = rc_to_c(rc);
    int b = rc_to_b(rc);

    *(f->row_contains + r) = *(f->row_contains + r)  ^ nbr; // xor
    *(f->col_contains + c) = *(f->col_contains + c)  ^ nbr;
    *(f->box_contains + b) = *(f->box_contains + b)  ^ nbr;
    *(f->field+ rc)        = *(f->field        + rc) ^ nbr;
}

/**
    Initiates a solving of a sudoku field. Returns a new sudoku field.

    @param {sudoku_field*} f - Input sudoku to solve.
    @return {sudoku_field*} - A solved sudoku if successful. NULL otherwise.
*/
sudoku_field*    init_sudoku_solve(const sudoku_field* f){
    sudoku_field* ret, *tmp;
    if(!sudoku_valid(f)){
        printf("Input Sudoku not a valid Sudoku. Returning NULL.");
        return NULL;
    }
    ret = copy_sudoku_field(f);
    tmp = sudoku_solve_rec_lin(ret, 0);
    if(tmp == NULL){
        free_sudoku_field(ret);
        return tmp;
    }else{
        return ret; // Since ret's contents is modified in solve_rec
    }
}
/**
    Tries to solve the input sudoku recursively. Returns the input sudoku field
    (solved) if successful and NULL otherwise. Caution is therefore needed to
    avoid memory leaks.

    NOT CURRENTLY IMPLEMENTED!

    @param {sudoku_field*} f - Input sudoku to solve.
    @param {su_cell_t} next_r - Row index of the next cell to test.
    @param {su_cell_t} next_c - Row index of the next cell to test.
    @return {sudoku_field*} - A solved sudoku if successful. NULL otherwise.
*/
sudoku_field*   sudoku_solve_rec(sudoku_field*, su_rcb_t next_x, su_rcb_t next_y);
/**
    Tries to solve the input sudoku recursively. Returns the input sudoku field
    (solved) if successful and NULL otherwise. Caution is therefore needed to
    avoid memory leaks.

    Assumes that the sudoku field data is stored in a single array.

    @param {sudoku_field*} f - Input sudoku to solve.
    @param {int} rc - rc index of the next cell to test.
    @return {sudoku_field*} - A solved sudoku if successful. NULL otherwise.
*/
sudoku_field*   sudoku_solve_rec_lin(sudoku_field* f,int rc){
    su_cell_t cell_test_value;
    sudoku_field* ret;

    if(rc >= SUDOKU_SIZE*SUDOKU_SIZE)
        return f; // If we made it this far the sudoku is valid and solved.
    if(!get_cell_lin(f, rc)){ // If current cell is empty do.
        for(cell_test_value = ONE; cell_test_value < (ONE << SUDOKU_SIZE);
                                    cell_test_value=(cell_test_value << 1) ){
            /* ------------------DEBUG--------------------------------------- */
//            printf("RC: %d \n", rc);
//            for(i = 0; i < SUDOKU_SIZE; i++){
//                printf("Rows: %x Cols: %x Box:%x\n", *(f->row_contains +i),*(f->col_contains +i),*(f->box_contains +i));
//            }
//            print_sudoku_from_field(f);
//            printf("solve_rec_lin rc: %d Test val: %x\n",rc, cell_test_value); fflush(stdout);
            /* ------------------DEBUG----------------------------------------*/
            if( !set_cell_lin(f, rc, cell_test_value) ){ // Current row, column and box does not contain cell_test_value if the set succeed (return val 0).
                ret = sudoku_solve_rec_lin(f, rc+1);
                if(ret == NULL){
                    unset_cell_lin(f, rc, cell_test_value);
                }else{
                    return ret;
                }
            }
        }
        // Testing finished. If we come here no number could be placed in the
        // empty cell. Ie there is no solution.
        return NULL;
    }else{ // else try next cell
        return sudoku_solve_rec_lin(f, rc +1);
    }
    //The code shouldn't end up here.
    fprintf(stderr,"Oj, vilken overaskning?");
    exit(1);
}
