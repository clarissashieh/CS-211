/*execute.c*/

//
// Executes nuPython program, given as a Program Graph.
// 
// Clarissa Shieh
// Based on Solution by Prof. Joe Hummel
// Northwestern University
// CS 211
// Winter Quarter
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>  // true, false
#include <string.h>
#include <assert.h>
#include <math.h>

#include "programgraph.h"
#include "ram.h"
#include "execute.h"


//
// Private functions:
//

//
// get_element_value
//
// Given a basic element of an expression --- an identifier
// "x" or some kind of literal like 123 --- the value of 
// this identifier or literal is returned via the reference 
// parameter. Returns ram value if successful, NULL if not.
//
// Why would it fail? If the identifier does not exist in 
// memory. This is a semantic error, and an error message is 
// output before returning.
//
static struct RAM_VALUE* get_element_value(struct STMT* stmt, struct RAM* memory, struct ELEMENT* element, struct RAM_VALUE* ram_value)
{
  char* literal = element->element_value;
  ram_value = (struct RAM_VALUE*)malloc(sizeof(struct RAM_VALUE));

  if (element->element_type == ELEMENT_INT_LITERAL) { 
    ram_value->types.i = atoi(literal);
    ram_value->value_type = RAM_TYPE_INT;
  }
  else if (element->element_type == ELEMENT_REAL_LITERAL) { 
    ram_value->types.d = atof(literal);
    ram_value->value_type = RAM_TYPE_REAL;
  }
  else if (element->element_type == ELEMENT_STR_LITERAL) { 
    ram_value->types.s = literal;
    ram_value->value_type = RAM_TYPE_STR;
  }
  else if (element->element_type == ELEMENT_TRUE) {
    ram_value->types.i = true;
    ram_value->value_type = RAM_TYPE_BOOLEAN;
  }
  else if (element->element_type == ELEMENT_FALSE) {
    ram_value->types.i = false;
    ram_value->value_type = RAM_TYPE_BOOLEAN;
  }
  else if (element->element_type == ELEMENT_IDENTIFIER){
    //
    // identifier => variable
    //

    char* var_name = element->element_value;
    ram_value = ram_read_cell_by_id(memory, var_name);

    if (ram_value == NULL) {
      printf("**SEMANTIC ERROR: name '%s' is not defined (line %d)\n", var_name, stmt->line);
      return NULL;
    }
  }
  else return NULL;

  return ram_value;
}


//
// get_unary_value
//
// Given a unary expr, returns the value that it represents.
// This could be the result of a literal 123 or the value
// from memory for an identifier such as "x". Unary values
// may have unary operators, such as + or -, applied.
// This value is "returned" via the reference parameter.
// Returns ram value if successful, NULL if not.
//
// Why would it fail? If the identifier does not exist in 
// memory. This is a semantic error, and an error message is 
// output before returning.
//
static struct RAM_VALUE* get_unary_value(struct STMT* stmt, struct RAM* memory, struct UNARY_EXPR* unary)
{
  //
  // we only have simple elements so far (no unary operators):
  //
  assert(unary->expr_type == UNARY_ELEMENT);

  struct ELEMENT* element = unary->element;

  struct RAM_VALUE ram_value;

  return get_element_value(stmt, memory, element, &ram_value);
}

//
// execute_real
//
// Given doubles from both sides of an expression and its operator, executes 
// the given operation. Works for ints and reals and returns false if 
// operator is not recognized. Otherwise, returns true if successful.
//
static bool execute_real(double* left, double* right, int operator) {
  switch (operator)
    {
      case OPERATOR_PLUS:
        *left = *left + *right;
        break;

      case OPERATOR_MINUS:
        *left = *left - *right;
        break;

      case OPERATOR_ASTERISK:
        *left = *left * *right;
        break;

      case OPERATOR_POWER:
        *left = pow(*left, *right);
        break;

      case OPERATOR_MOD:
        *left = fmod(*left, *right);
        break;

      case OPERATOR_DIV:
        *left = *left / *right;
        break;

      case OPERATOR_EQUAL:
        *left = (*left == *right);
        break;

      case OPERATOR_NOT_EQUAL:
        *left = (*left != *right);
        break;

      case OPERATOR_LT:
        *left = (*left < *right);
        break;

      case OPERATOR_LTE:
        *left = (*left <= *right);
        break;

      case OPERATOR_GT:
        *left = (*left > *right);
        break;

      case OPERATOR_GTE:
        *left = (*left >= *right);
        break;

      default:
        printf("**EXECUTION ERROR: unexpected operator (%d) in execute_binary_expr\n", operator);
        return false;
    }
  return true;
}

//
// execute_binary_expr
//
// Given two values and an operator, performs the operation
// and updates the value in the lhs (which can be updated
// because a pointer to the value is passed in). Returns
// true if successful and false if not.
//
static bool execute_binary_expr(struct STMT* stmt, struct RAM_VALUE* lhs, int operator, struct RAM_VALUE rhs)
{
  assert(operator != OPERATOR_NO_OP);

  //go through different operand combinations
  if (lhs->value_type == RAM_TYPE_INT && rhs.value_type == RAM_TYPE_INT) {
    lhs->types.d = lhs->types.i;
    rhs.types.d = rhs.types.i;

    if(!execute_real(&lhs->types.d, &rhs.types.d, operator))
      return false;
    
    lhs->types.i = lhs->types.d;
  }
  else if ((lhs->value_type == RAM_TYPE_REAL || lhs->value_type == RAM_TYPE_INT) && (rhs.value_type == RAM_TYPE_REAL || rhs.value_type == RAM_TYPE_INT)) {
    if(lhs->value_type == RAM_TYPE_INT) {
      lhs->value_type = RAM_TYPE_REAL;
      lhs->types.d = lhs->types.i;
    }
    if(rhs.value_type == RAM_TYPE_INT) 
      rhs.types.d = rhs.types.i;

    if(!execute_real(&lhs->types.d, &rhs.types.d, operator))
      return false;
  }
  else if (lhs->value_type == RAM_TYPE_STR && rhs.value_type == RAM_TYPE_STR) {
    if (operator == OPERATOR_PLUS) {
      size_t len = strlen(lhs->types.s) + strlen(rhs.types.s);
      char* new_str = (char*)malloc(len + 1);

      strcpy(new_str, lhs->types.s);
      strcat(new_str, rhs.types.s);

      lhs->types.s = new_str;
    }
    else {
      int comp = strcmp(lhs->types.s, rhs.types.s);
      switch (operator){
        case OPERATOR_EQUAL:
          lhs->types.i = (comp == 0);
          break;

        case OPERATOR_NOT_EQUAL:
          lhs->types.i = (comp != 0);
          break;

        case OPERATOR_LT:
          lhs->types.i = (comp < 0);
          break;

        case OPERATOR_LTE:
          lhs->types.i = (comp <= 0);
          break;

        case OPERATOR_GT:
          lhs->types.i = (comp > 0);
          break;

        case OPERATOR_GTE:
          lhs->types.i = (comp >= 0);
          break;

        default:
          printf("**EXECUTION ERROR: unexpected operator (%d) in execute_binary_expr\n", operator);
          return false;
      }
    }
  }
  else {
    printf("**SEMANTIC ERROR: invalid operand types (line %d)\n", stmt->line);
    return false;
  }

  //checks if the operator is a relational operator
  enum OPERATORS relational_ops[6] = {OPERATOR_EQUAL, OPERATOR_NOT_EQUAL, OPERATOR_LT, OPERATOR_LTE, OPERATOR_GT, OPERATOR_GTE};
  bool is_relational = false;
  for (int i = 0; i < 6; i++) {
    if (operator == relational_ops[i]) { 
      is_relational = true;
      break;
    }
  }

  if (is_relational) { //if relational operator, change lhs type
    if (lhs->value_type != RAM_TYPE_STR) {
      if (lhs->types.d == 0) lhs->types.i = false;
      else if (lhs->types.d == 1) lhs->types.i = true;
    }
    lhs->value_type = RAM_TYPE_BOOLEAN;
  }
  return true;
}

//
// execute_function
//
// Given a function call and ram value, executes given function and returns a pointer
// to the ram_value with the result of the function. Supports 3 types of functions:
// 1) input(): takes in a string literal from input, removes the EOL characters
//    and saves it into memory.
// 2) int(): takes in a string literal and converts it to an int. Returns an error
//    if conversion is unsuccessful.
// 3) float(): takes in a string literal and converts it to a real. Returns an error
//    if conversion is unsuccessful.
//
static struct RAM_VALUE* execute_function(struct STMT* stmt, struct RAM* memory, struct VALUE_FUNCTION_CALL* function_call, struct RAM_VALUE* ram_value) 
{
  ram_value = (struct RAM_VALUE*)malloc(sizeof(struct RAM_VALUE));

  char* function_name = function_call->function_name;
  struct ELEMENT* param = function_call->parameter;
  struct RAM_VALUE* value = get_element_value(stmt, memory, param, ram_value);

  if (strcmp(function_name, "input") == 0) {
    printf(value->types.s);
    char line[256];

    fgets(line, sizeof(line), stdin);
    // delete EOL chars from input:
    line[strcspn(line, "\r\n")] = '\0';
    
    size_t bytes = sizeof(char) * (strlen(line) + 1);
    char* s = (char*) malloc(bytes);
    strcpy(s, line);
    ram_value->value_type = RAM_TYPE_STR;
    ram_value->types.s = s;
  }

  else if (strcmp(function_name, "int") == 0) {
    int i = atoi(value->types.s);
    if((strchr(value->types.s, '0') != NULL) || i != 0) {
      ram_value->value_type = RAM_TYPE_INT;
      ram_value->types.i = i;
    }
    else {
      printf("**SEMANTIC ERROR: invalid string for %s() (line %d)\n", function_name, stmt->line);
      return NULL;
    }
  }
  
  else if (strcmp(function_name, "float") == 0) {
    //check if string has any zeros
    double d = atof(value->types.s);
    if((strchr(value->types.s, '0') != NULL) || (d != 0)) {
      ram_value->value_type = RAM_TYPE_REAL;
      ram_value->types.d = d;
    }
    else {
      printf("**SEMANTIC ERROR: invalid string for %s() (line %d)\n", function_name, stmt->line);
      return NULL;
    }
  }
  else {
    printf("**EXECUTION ERROR: unexpected function (%s) in execute_function\n", function_name);
    return NULL;
  }

  return ram_value;
}

//
// execute_assignment
//
// Executes an assignment statement, returning true if 
// successful and false if not (an error message will be
// output before false is returned, so the caller doesn't
// need to output anything).
// 
// Examples: x = 123
//           y = x ** 2
//
static bool execute_assignment(struct STMT* stmt, struct RAM* memory)
{
  struct STMT_ASSIGNMENT* assign = stmt->types.assignment;

  char* var_name = assign->var_name;
  //
  // no pointers yet:
  //
  assert(assign->isPtrDeref == false);
  //
  // we only have expressions on the RHS, no function calls:
  //
  if (assign->rhs->value_type == VALUE_EXPR) {
    struct VALUE_EXPR* expr = assign->rhs->types.expr;
    //
    // we always have a LHS:
    //
    assert(expr->lhs != NULL);

    struct RAM_VALUE* value = get_unary_value(stmt, memory, expr->lhs);

    if (value == NULL)  // semantic error? If so, return now:
      return false;

    //
    // do we have a binary expression?
    //
    if (expr->isBinaryExpr)
    {
      assert(expr->rhs != NULL);  // we must have a RHS
      assert(expr->operator != OPERATOR_NO_OP);  // we must have an operator

      struct RAM_VALUE* rhs_value = get_unary_value(stmt, memory, expr->rhs);

      if (rhs_value == NULL) {  // semantic error? If so, return now:
        return false;
      }
      //
      // perform the operation, updating value:
      //
      bool success = execute_binary_expr(stmt, value, expr->operator, *rhs_value);

      if (!success) {
        return false;
      }
      //
      // success! Fall through and write value to memory:
      //
    }
    return ram_write_cell_by_id(memory, *value, var_name);
  }
  else {
    assert(assign->rhs->value_type == VALUE_FUNCTION_CALL);

    struct VALUE_FUNCTION_CALL* function_call = assign->rhs->types.function_call;
    struct RAM_VALUE ram_value;
    struct RAM_VALUE* value = execute_function(stmt, memory, function_call, &ram_value);

    if (value == NULL) return false;

    return ram_write_cell_by_id(memory, *value, var_name); 
  }
  return false;
}


//
// execute_function_call
//
// Executes a function call statement, returning true if 
// successful and false if not (an error message will be
// output before false is returned, so the caller doesn't
// need to output anything).
// 
// Examples: print()
//           print(x)
//           print(123)
//
static bool execute_function_call(struct STMT* stmt, struct RAM* memory)
{
  struct STMT_FUNCTION_CALL* call = stmt->types.function_call;

  //
  // for now we are assuming it's a call to print:
  //
  char* function_name = call->function_name;

  if (strcmp(function_name, "print") == 0){
    if (call->parameter == NULL) {
      printf("\n");
    }
    else {
      //
      // we have a parameter, which type of parameter?
      // Note that a parameter is a simple element, i.e.
      // identifier or literal (or True, False, None):
      //
      char* element_value = call->parameter->element_value;

      if (call->parameter->element_type == ELEMENT_STR_LITERAL) {
        printf("%s\n", element_value);
      }
      else {
        //
        // right now we are assuming ints or variables containing
        // ints, so call our get_element function to obtain the
        // integer value:
        struct RAM_VALUE ram_value;
        struct RAM_VALUE* value = get_element_value(stmt, memory, call->parameter, &ram_value);

        if (value == NULL)
          return false;

        int element_type = call->parameter->element_type;
        if (element_type == ELEMENT_IDENTIFIER) 
          value = ram_read_cell_by_id(memory, call->parameter->element_value);
        if (value->value_type == RAM_TYPE_INT) 
          printf("%d\n", value->types.i);
        else if (value->value_type == RAM_TYPE_REAL) 
          printf("%lf\n", value->types.d);
        else if (value->value_type == RAM_TYPE_STR) 
          printf("%s\n", value->types.s);
        else if (value->value_type == RAM_TYPE_BOOLEAN) 
          printf("%s\n",value->types.i?"True":"False");
      }
    }//else
  }
  else return false;

  return true;
}

//
// execute_condition
//
// Given a statement, memory, and a while loop condition, extracts the lhs, rhs,
// and operator of a condition. It executes the condition and returns NULL if
// the condition is no longer true.
//
static struct RAM_VALUE* execute_condition(struct STMT* stmt, struct RAM* memory, struct VALUE_EXPR* condition, struct RAM_VALUE* value) {
  value = get_unary_value(stmt, memory, condition->lhs);
  
  if (value == NULL)  
    return NULL;

  assert(condition->rhs != NULL);  
  assert(condition->operator != OPERATOR_NO_OP); 

  struct RAM_VALUE* rhs_value = get_unary_value(stmt, memory, condition->rhs);

  if (rhs_value == NULL) 
    return NULL;

  bool success = execute_binary_expr(stmt, value, condition->operator, *rhs_value);
  if (!success)
    return NULL;

  return value;
}

//
// execute_while_loop
// 
// Executes a while loop, returning true if 
// successful and false if not (an error message will be
// output before false is returned, so the caller doesn't
// need to output anything).
//
// Example:
// while //condition:
// {
//  //code
// }
//
static bool execute_while_loop(struct STMT* stmt, struct RAM* memory) {
  struct STMT_WHILE_LOOP* while_loop = stmt->types.while_loop;
  struct VALUE_EXPR* condition = while_loop->condition;
  
  struct STMT* body = while_loop->loop_body;
  struct RAM_VALUE value; 
  struct RAM_VALUE* ram_value = execute_condition(stmt, memory, condition, &value);

  while ((ram_value != NULL) && ram_value->types.i) {
    body = while_loop->loop_body;
    while (body->types.while_loop->condition != condition)
    {
      if (body->stmt_type == STMT_ASSIGNMENT) {
        bool success = execute_assignment(body, memory);
        if (!success)
          return false;
        
        body = body->types.assignment->next_stmt;
      }
      else if (body->stmt_type == STMT_FUNCTION_CALL) {
        bool success = execute_function_call(body, memory);
        if (!success)
          return false;
        
        body = body->types.function_call->next_stmt;
      }
      else if (body->stmt_type == STMT_WHILE_LOOP) {
        bool success = execute_while_loop(body, memory);
        if (!success)
          return false;

        body = body->types.while_loop->next_stmt;
      }
      else if (body->stmt_type == STMT_PASS)
        body = body->types.pass->next_stmt;
      else 
        return false;
    }
    ram_value = execute_condition(stmt, memory, condition, &value);
  }
  if (ram_value == NULL) 
    return false;

  return true;
}

//
// Public functions:
//

//
// execute
//
// Given a nuPython program graph and a memory, 
// executes the statements in the program graph.
// If a semantic error occurs (e.g. type error),
// an error message is output, execution stops,
// and the function returns.
//
void execute(struct STMT* program, struct RAM* memory)
{
  struct STMT* stmt = program;

  //
  // traverse through the program statements:
  //
  while (stmt != NULL) {

    if (stmt->stmt_type == STMT_ASSIGNMENT) {

      bool success = execute_assignment(stmt, memory);

      if (!success)
        return;

      stmt = stmt->types.assignment->next_stmt;  // advance
    }
    else if (stmt->stmt_type == STMT_FUNCTION_CALL) {

      bool success = execute_function_call(stmt, memory);

      if (!success)
        return;

      stmt = stmt->types.function_call->next_stmt;
    }
    else if (stmt->stmt_type == STMT_WHILE_LOOP) {
      bool success = execute_while_loop(stmt, memory);

      if (!success)
        return;

      stmt = stmt->types.while_loop->next_stmt;
    }
    else {
      assert(stmt->stmt_type == STMT_PASS);

      //
      // nothing to do!
      //

      stmt = stmt->types.pass->next_stmt;
    }
  }//while
  
  //
  // done:
  //
  
  return;
}