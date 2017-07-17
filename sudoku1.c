// Sudoku solver
#include <stdio.h>      // For standard IO
#include <stdlib.h>     // For standard stuff
#include <string.h>     // strlen
#include <getopt.h>     // getopt_long

#include <time.h>       // For program timing

#include "sudoku_general_tools.h"
/**
    Main
*/
int main(int argc, char* argv[])
{

    // Input_handling
    int option_info;// -1 when there are no more to handle.
    int option_index;
    static struct option long_options[] =
        {
          {"size",     required_argument,      NULL, 's'},
          {"sudoku",  required_argument,       NULL, 'f'},
          {"iterations",  optional_argument, NULL, 'i'},
          {NULL, 0, NULL, 0}
        };
    // Input variables (aka, almost flags or something)
    int iterations = 1;
    int i;

    // Sudoku
    sudoku_field* s = NULL;
    sudoku_field* s_solved = NULL;
    // Timing
    clock_t sudoku_timing_start; // Counts CPU cycles. Use the macro CLOCKS_PER_SEC to convert to real time
    clock_t sudoku_timing_end;

    // INPUT-HANDLING HERE

    while(1){
    /* getopt_long stores the option index here. */
      option_index = 0;

      option_info = getopt_long (argc, argv, "s:f:i::",
                       long_options, &option_index);

      /* Detect the end of the options. */
      if (option_info == -1)
        break;

      switch (option_info)
        {
        case 0:
          /* If this option set a flag, do nothing else now. */
          if (long_options[option_index].flag != 0)
            break;
          printf ("The option set a flag, which it shouldn't since I didn't add\n any option that does. Name of the options:%s \n", long_options[option_index].name);
          if (optarg) // The options argument is stored in optarg
            printf (" with arg %s", optarg);
          printf ("\n");
          break;

        case 's':
            printf("Sudoku size set to %d, but will be 9 anyway since this\n options hasn't been properly implemented yet.\n", atoi(optarg) );
            break;

        case 'f':
          printf("Reading Sudoku from input\n");
          if(strlen(optarg) != (SUDOKU_SIZE * SUDOKU_SIZE)){
            fprintf(stderr, "Input Sudoku not correct size. Line %d. Exiting...\n",__LINE__);
            exit(1);
          }
          s = sudoku_from_input(optarg);
          break;

        case 'i':
            if(optarg != NULL){
                iterations = atoi(optarg);
                printf ("Number of iterations set to %d.\n", iterations);

            }else{
                printf("Number of iterations set to its default value (30000).\n");
                iterations = 30000;
            }
            break;
        case '?':
          /* getopt_long already printed an error message. */
            fprintf(stderr, "Failure reading program options. Line %d. Exiting...\n",__LINE__);
            exit(1);
          break;

        default:
          exit(1);
        }
    }


    // The real program here.
    printf("Sudoku pre solving:\n");
    print_sudoku_from_field(s);
    if(iterations == 1){
        printf("Solving started...\n");
        sudoku_timing_start = clock();
        s_solved = init_sudoku_solve(s);
        sudoku_timing_end = clock();
    }else{
        printf("Solving started. A total of %d iterations will be made...\n", iterations);
        sudoku_timing_start = clock();
        for(i = 0; i < iterations; i++){
            free_sudoku_field(init_sudoku_solve(s));
        }
        s_solved = init_sudoku_solve(s);
        sudoku_timing_end = clock();
    }

    if(s_solved == NULL){
        printf("Sudoku not solvable.\n");
    }else{
        printf("Sudoku solved:\n");
        print_sudoku_from_field(s_solved);
        free_sudoku_field(s_solved);
    }
    // Cleanup
    printf("Releasing allocated memory...\n");
    fflush(stdout);
    free_sudoku_field(s);


    printf("Execution time: %.3f In clock_t: %ju\n", (double) (sudoku_timing_end - sudoku_timing_start)/CLOCKS_PER_SEC,  (sudoku_timing_end - sudoku_timing_start) );
    printf("Program ran successfully (it seems). Exiting.\n");
    exit(0);
    /*
        0 is a successful exit. Non-zero numbers indicate error or other
        abnormal exit. No standard for non-zero exit values seem to exist.
        Some use -1 for errors and 1 for fails.
    */
}
