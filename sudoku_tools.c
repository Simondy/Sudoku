// Sudoku solver support tools
#include <stdio.h>      // For standard IO
#include <stdlib.h>     // For standard stuff
#include <string.h> // memcpy


#include "sudoku_general_tools.h"

/**
    Supporting functions for the sudoku solver, ie things not needed for the
    solving itself.
*/


/**
    Get a sudoku field from a string or char array. Currently only designed
    for 9x9 or 16x16 sudokus.

    @param {char*} s - The input string.
    @return {sudoku_field} - The return sudoku field.
*/
sudoku_field*    sudoku_from_input(const char* sudoku_str){
    int rc;
    sudoku_field* ret = alloc_sudoku_field();
    for(rc = 0; rc < SUDOKU_SIZE*SUDOKU_SIZE; rc++){
        switch( *(sudoku_str + rc) ){
            case('0'):
                set_cell_lin(ret, rc, ZERO);
              //  printf("Cell %d set to: %x New val: %x \n", rc, ZERO, get_cell_lin(ret, rc));
                break;
            case('1'):
                set_cell_lin(ret, rc, ONE);
              //  printf("Cell %d set to: %x New val: %x \n", rc, ONE, get_cell_lin(ret, rc));
                break;
            case('2'):
                set_cell_lin(ret, rc, TWO);
              //  printf("Cell %d set to: %x New val: %x \n", rc, TWO, get_cell_lin(ret, rc));
                break;
            case('3'):
                set_cell_lin(ret, rc, THREE);
            // printf("Cell %d set to: %x New val: %x \n", rc, THREE, get_cell_lin(ret, rc));
                break;
            case('4'):
                set_cell_lin(ret, rc, FOUR);
                //printf("Cell %d set to: %x New val: %x \n", rc, FOUR, get_cell_lin(ret, rc));
                break;
            case('5'):
                set_cell_lin(ret, rc, FIVE);
               // printf("Cell %d set to: %x New val: %x \n", rc, FIVE, get_cell_lin(ret, rc));
                break;
            case('6'):
                set_cell_lin(ret, rc, SIX);
               // printf("Cell %d set to: %x New val: %x \n", rc, SIX, get_cell_lin(ret, rc));
                break;
            case('7'):
                set_cell_lin(ret, rc, SEVEN);
               // printf("Cell %d set to: %x New val: %x \n", rc, SEVEN, get_cell_lin(ret, rc));
                break;
            case('8'):
                set_cell_lin(ret, rc, EIGHT);
               // printf("Cell %d set to: %x New val: %x \n", rc, EIGHT, get_cell_lin(ret, rc));
                break;
            case('9'):
                set_cell_lin(ret, rc, NINE);
               // printf("Cell %d set to: %x New val: %x \n", rc, NINE, get_cell_lin(ret, rc));
                break;
            case('A'):
                set_cell_lin(ret, rc, TEN);
               // printf("Cell %d set to: %x New val: %x \n", rc, TEN, get_cell_lin(ret, rc));
                break;
            case('B'):
                set_cell_lin(ret, rc, ELEVEN);
               // printf("Cell %d set to: %x New val: %x \n", rc, ELEVEN, get_cell_lin(ret, rc));
                break;
            case('C'):
                set_cell_lin(ret, rc, TWELVE);
               // printf("Cell %d set to: %x New val: %x \n", rc, TWELVE, get_cell_lin(ret, rc));
                break;
            case('D'):
                set_cell_lin(ret, rc, THIRTEEN);
               // printf("Cell %d set to: %x New val: %x \n", rc, THIRTEEN, get_cell_lin(ret, rc));
                break;
            case('E'):
                set_cell_lin(ret, rc, FOURTEEN);
               // printf("Cell %d set to: %x New val: %x \n", rc, FOURTEEN, get_cell_lin(ret, rc));
                break;
            case('F'):
                set_cell_lin(ret, rc, FIFTEEN);
               // printf("Cell %d set to: %x New val: %x \n", rc, FIFTEEN, get_cell_lin(ret, rc));
                break;
            default:
                fprintf(stderr, "Could not read sudoku from input, %d", __LINE__);
                exit(1);
        }
    }
    print_sudoku_from_field(ret);
    return ret;
}
/**
    Allocates the memory for a sudoku field, including the internal arena.

    @return {sudoku_field*} - A pointer to the new arena.
*/
sudoku_field* alloc_sudoku_field(){
    sudoku_field* ret = malloc( sizeof(sudoku_field) );
    if(ret == NULL){
        fprintf(stderr, "Could not allocate memory for the sudoku field, %d", __LINE__);
        exit(1);
    }

    alloc_sudoku_field_internal_arena(ret);
    return ret;
}
/**
    Allocates the  internal memory of the sudoku field as a cleared arena.

    @param {sudoku_field*} s - The field to get a new arena.
    @return {int} - 0 if successful. Non zero otherwise..
*/
int alloc_sudoku_field_internal_arena(sudoku_field* s){
    su_cell_t* tmp;
    tmp = (su_cell_t*) calloc( 3 * SUDOKU_SIZE + SUDOKU_SIZE * SUDOKU_SIZE
                                                        ,sizeof(su_cell_t));
    if(tmp == NULL){
        fprintf(stderr, "Could not allocate sudoku field memory, %d", __LINE__);
        exit(1);
    }
    s->row_contains    = tmp;
    s->col_contains    = tmp + 1 * SUDOKU_SIZE;
    s->box_contains    = tmp + 2 * SUDOKU_SIZE;
    s->field           = tmp + 3 * SUDOKU_SIZE;
    return 0;
}
/**
    Frees the memory of  a sudoku field.
    @param {sudoku_field*} f - A pointer to the field to be freed.
*/
void free_sudoku_field(sudoku_field* f){
    free_sudoku_field_internal_arena(f);
    free(f);
}
/**
    Frees the internal memory of a sudoku field.
    @param {sudoku_field*} f - A pointer to the field.
*/
void free_sudoku_field_internal_arena(sudoku_field* f){
    free(f->row_contains);
}
/**
    Returns a pointer to a complete copy of the input sudoku field.
    @param {sudoku_field*} f - A pointer to the field to be copied.
*/
sudoku_field* copy_sudoku_field(const sudoku_field* f){
    sudoku_field* ret = alloc_sudoku_field();
    memcpy(ret->row_contains, f->row_contains,
                        (3 * SUDOKU_SIZE + SUDOKU_SIZE * SUDOKU_SIZE)*sizeof(su_cell_t));
    return ret;
}

/**
    This function is supposed to return a string containing the sudoku.
    IT DOES NOT CURRENTLY DO THAT.

    @param {sudoku_field*} f - A pointer to the field.s
    @return {char*} - A string containing the sudoku.
*/
char*           sudoku_to_string(sudoku_field* f){
    return "Function not yet implemented.";
}

/**
    This function is supposed check wether or not a sudoku is valid before even
    trying to complete it. IT DOES NOT CURRENTLY DO THAT.
    However if the sudoku is constructed using the
    set_cell-functions it will be a correct sudoku.

    @param {sudoku_field*} f - A pointer to the field.s
    @return {int} - 1 if seemingly valid, 0 otherwise.
*/
int          sudoku_valid(const sudoku_field* f){
    return 1;
}

/**
    Print a sudoku field. Currently only designed for 9x9 or 16x16 sudokus.

    @param {sudoku_field} s - The sudoku field to be printed.
    @return {int} - 0 if successful. Non zero otherwise.
*/
int print_sudoku_from_field(const sudoku_field* s){
    su_rcb_t r;
    su_rcb_t c;

    printf("Printing sudoku:\n");
    for(r = 0; r < SUDOKU_SIZE; r++){ // Sudoku size should maybe be switched to a flag instead of a define.
        printf(" ");// Change of line come after the next for-loop.
        for(c = 0; c < SUDOKU_SIZE; c++){
          //  printf("\nRC: %d R: %d C:%d Cell val: %x ", r_and_c_to_rc(r, c),r, c, *(s->field + r_and_c_to_rc(r,c))); // DEBUG
            switch( get_cell(s, r, c) ){
                case(ZERO):
                    printf("0");
                    break;
                case(ONE):
                    printf("1");
                    break;
                case(TWO):
                    printf("2");
                    break;
                case(THREE):
                    printf("3");
                    break;
                case(FOUR):
                    printf("4");
                    break;
                case(FIVE):
                    printf("5");
                    break;
                case(SIX):
                    printf("6");
                    break;
                case(SEVEN):
                    printf("7");
                    break;
                case(EIGHT):
                    printf("8");
                    break;
                case(NINE):
                    printf("9");
                    break;
                case(TEN):
                    printf("A");
                    break;
                case(ELEVEN):
                    printf("B");
                    break;
                case(TWELVE):
                    printf("C");
                    break;
                case(THIRTEEN):
                    printf("D");
                    break;
                case(FOURTEEN):
                    printf("E");
                    break;
                case(FIFTEEN):
                    printf("F");
                    break;
                default:
                    printf("X");
            }
        }
        printf("\n");
    }
    return 0;
}
