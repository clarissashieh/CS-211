/*main.c*/

//
// Project 02: main program to test execution environment for nuPython
// 
// Clarissa Shieh
// Northwestern University
// CS 211
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>  // true, false
#include <string.h>   // strcspn

#include "token.h"    // token defs
#include "scanner.h"  // scanner

#include "parser.h"
#include "programgraph.h"
#include "ram.h"
#include "execute.h"

//
// main
//
// usage: program.exe [filename.py]
// 
// If a filename is given, the file is opened and serves as
// input to the scanner. If a filename is not given, then 
// input is taken from the keyboard until $ is input.
//
int main(int argc, char* argv[])
{
  FILE* input = NULL;
  bool  keyboardInput = false;

  if (argc < 2) {
    //
    // no args, just the program name:
    //
    input = stdin;
    keyboardInput = true;
  }
  else {
    //
    // assume 2nd arg is a nuPython file:
    //
    char* filename = argv[1];

    input = fopen(filename, "r");

    if (input == NULL) // unable to open:
    {
      printf("**ERROR: unable to open input file '%s' for input.\n", filename);
      return 0;
    }

    keyboardInput = false;
  }

  
  if (keyboardInput)  // prompt the user if appropriate:
  {
    printf("nuPython input (enter $ when you're done)>\n");
  }

  //
  // call parser to check program syntax:
  //
  parser_init();

  struct TokenQueue* tokens = parser_parse(input);

  if(tokens == NULL)
  {
    //
    // program has a syntax error, error msg already output:
    //
  }
  else
  {
    //
    // parsing successful, now build program graph:
    //
    struct STMT* program = programgraph_build(tokens);
    printf("**no syntax errors...\n");
    printf("**building program graph...\n");
    programgraph_print(program);
    printf("**executing...\n");
    struct RAM* memory = ram_init();
    execute(program, memory);
    printf("**done\n");
    ram_print(memory);
  }

   return 0;
}
