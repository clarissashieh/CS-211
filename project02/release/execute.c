/*execute.c*/

//
// Given a nuPy program and a memory, executes function calls and
// assignments in the program graph.
//
// Clarissa Shieh
// Northwestern University
// CS 211
// Winter Quarter 2024
// 
// Starter code: Prof. Joe Hummel
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>  // true, false
#include <string.h>
#include <assert.h>

#include "programgraph.h"
#include "ram.h"
#include "execute.h"
#include "math.h"


//
// Public functions:
//

//
// execute_function_call
//
// Given a statement and a memory, executes a function call
// based on its parameter and returns whether it was successfully
// put into memory. If there is a semantic error, returns false.
// 
bool execute_function_call(struct STMT* stmt, struct RAM* memory)
{
  int line_num = stmt->line;
  char* func_name = stmt->types.function_call->function_name;
  struct ELEMENT* param = stmt->types.function_call->parameter;
  if(param == NULL) printf("\n"); //check if there's a parameter
  else 
  {
    int type = param->element_type;
    char* value = param->element_value;

    if(type == 1){ //check if param is int literal
      int int_param = atoi(value);
      printf("%d\n", int_param);
    }
    else if(type == 0) //check if param is identifier
    {
      if(ram_read_cell_by_id(memory, value))
      {
        int int_value = ram_read_cell_by_id(memory, value)->types.i;
        printf("%d\n", int_value);
      }
      else
      { 
        printf("**SEMANTIC ERROR: name '%s' is not defined (line %d)\n", value, line_num);
        return false;
      }
    }
    else printf("%s\n", value);
  }
  return true;
}

//
// read
//
// Given a unary expression, a memory, and a line number, reads
// and returns the value of the given expression. A helper function
// for the execute_assignment function by reading both sides of
// an expression.
// 
struct RAM_VALUE read(struct UNARY_EXPR* expr, struct RAM* memory, int line_num)
{
  struct RAM_VALUE ram_value;
  ram_value.value_type = RAM_TYPE_NONE;

  if(expr->element != NULL) //check if value has an element
  {
    int type = expr->element->element_type;
    char* value = expr->element->element_value;

    if(type == 1) //check if element type is int literal
    {
      ram_value.value_type = RAM_TYPE_INT;
      ram_value.types.i = atoi(value);
      return ram_value;
    }
    else //assume type is identifier (for now)
    {
      if(ram_read_cell_by_id(memory, value)){ //check identifier has value
        return *ram_read_cell_by_id(memory, value);
      }
      else printf("**SEMANTIC ERROR: name '%s' is not defined (line %d)\n", value, line_num);
    }
  }
  return ram_value;
}

//
// perform_op
//
// Given the values of two sides of an expression and an operator,
// performs the given operation on the values of the expression
// and returns the result.
// 
struct RAM_VALUE perform_op(struct RAM_VALUE lhs, struct RAM_VALUE rhs, int operator)
{
  int lhs_value = lhs.types.i;
  int rhs_value = rhs.types.i;
  struct RAM_VALUE value;

  int result = -1;
  if(lhs.value_type == 5 || rhs.value_type == 5) 
  { 
    value.value_type = RAM_TYPE_NONE;
    value.types.i = result;
    return value;
  }
  value.value_type = RAM_TYPE_INT;

  if(operator == 0) result = lhs_value + rhs_value;
  else if(operator == 1) result = lhs_value - rhs_value;
  else if(operator == 2) result = lhs_value * rhs_value;
  else if(operator == 3) result = (int)pow(lhs_value, rhs_value);
  else if(operator == 4) result = lhs_value % rhs_value;
  else if(operator == 5) result = lhs_value / rhs_value;
  else
  { 
    printf("**ERROR: Invalid operator");
    value.value_type = RAM_TYPE_NONE;
  }
  
  value.types.i = result;
  return value;
}

//
// execute_assignment
//
// Given a statement and a memory, executes an assignment
// and returns whether it was successfully put into memory. 
// Distinguishes between unary and binary expressions and handles
// each appropriately. If there is a semantic error, returns false.
// 
bool execute_assignment(struct STMT* stmt, struct RAM* memory)
{
  int line_num = stmt->line;
  char* lhs = stmt->types.assignment->var_name;
  struct VALUE* rhs = stmt->types.assignment->rhs;

  if(stmt->types.assignment->rhs != NULL){ //check if there's a rhs
    struct VALUE* rhs = stmt->types.assignment->rhs;
    if(rhs->value_type == 1) //check if rhs is an expr
    {
      struct VALUE_EXPR* expr = rhs->types.expr;
      if(expr->lhs != NULL) //check if rhs has lhs
      {
        struct RAM_VALUE lhs_value = read(expr->lhs, memory, line_num);
        if(expr->isBinaryExpr) //check if expr has lhs, operator, rhs
        {
          struct RAM_VALUE rhs_value = read(expr->rhs, memory, line_num);
          int operator = expr->operator;
          struct RAM_VALUE result = perform_op(lhs_value, rhs_value, operator);
          if(result.value_type != 5) ram_write_cell_by_id(memory, result, lhs);
        }
        else ram_write_cell_by_id(memory, lhs_value, lhs);
      }
    }
  }
  
  struct RAM_VALUE* check = ram_read_cell_by_id(memory, lhs);
  if(check != NULL && check->value_type != RAM_TYPE_NONE) return true;
  else return false;
}

//
// execute
//
// Given a nuPython program graph and a memory, 
// executes the statements in the program graph.
// If a semantic error occurs (e.g. type error),
// and error message is output, execution stops,
// and the function returns.
//
void execute(struct STMT* program, struct RAM* memory)
{
  // printf("\nexecute(): TODO\n\n");
  struct STMT* stmt = program;
  while (stmt != NULL) {
    if (stmt->stmt_type == STMT_ASSIGNMENT) {
      bool execution = execute_assignment(stmt, memory);
      if(!execution) break;
      stmt = stmt->types.assignment->next_stmt;
    }
    else if (stmt->stmt_type == STMT_FUNCTION_CALL) {
      bool execution = execute_function_call(stmt, memory);
      if(!execution) break;
      stmt = stmt->types.function_call->next_stmt;
    }
    else{
      assert(stmt->stmt_type == STMT_PASS);
      stmt = stmt->types.pass->next_stmt;
    }
  }
}
