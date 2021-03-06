/* 
 * Name: Aditya Varma Vetukuri
 * GNumber: G01213246
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "token.h"
#include "hash.h"

/* Local Function Declarations (that you need to finish implementing) */
static int read_file(char *filename, char *line);
static int parse_token(Symtab *symtab, Stack_head *stack, Token *tok);

/* Pre-Written Local Function Declarations */
static void print_header(char *filename, int step);
static void print_step_header(int step);
static void print_step_footer(Symtab *symtab, Stack_head *stack);
static void print_step_output(int val);

/* Defines the largest line that can be read from a file */
#define MAX_LINE_LEN 255

/* This has been written for you.
 * Main function to run your program (written for you).
 * 1) Opens the file using the passed in filename. (your function)
 * -- If the file is not found (ie. fopen returns NULL), then exit(-1);
 * 2) Reads one line from the file.
 * -- The programs will all only be one line in size.
 * -- That line may be up to MAX_LINE_LEN long.
 * 3) Closes the file.
 * 4) Calls token_read_line(line, strlen(line))
 * -- This parses the line and prepares the tokens to be ready to get.
 * 5) While there are tokens remaining to parse: token_has_next() != 0
 * -- a) Get the next token: token_get_next()
 * 6) Parse the token (your function)
 * 7) Print out all of the relevant information
 */
int rpn(Stack_head *stack, Symtab *symtab, char *filename) {
  int step = 0; /* Used to track the program steps */
  int ret = 0;
  char line[MAX_LINE_LEN];
  Token *tok = NULL;

  /* Complete the read_file function that is defined later in this file. */
  ret = read_file(filename, line);
  if(ret != 0) {
    printf("Error: Cannot Read File %s.  Exiting\n", filename);
    exit(-1);

  }

  /* Pass the line into the tokenizer to initialize that system */
  token_read_line(line, strlen(line));

  /* Prints out the nice program output header */
  print_header(filename, step);

  /* Iterate through all tokens */
  while(token_has_next()) {
    /* Begin the next step of execution and print out the step header */
    step++; /* Begin the next step of execution */
    print_step_header(step);

    /* Get the next token */
    tok = token_get_next();
    /* Complete the implementation of this function later in this file. */
    ret = parse_token(symtab, stack, tok);
    if(ret != 0) {
      printf("Critical Error in Parsing.  Exiting Program!\n");
      exit(-1);
    }

    /* Prints out the end of step information */
    print_step_footer(symtab, stack);
  }

  return 0;
}

/* (IMPLEMENT THIS FUNCTION)
 * Local function to open a file or exit.
 * Follow the Directions in the Project Documentation for this Function
 * Open filename, read its contents (up to MAX_LINE_LEN) into line, then
 *   close the file and return 0.
 * On any file error, return -1.
 */
static int read_file(char *filename, char *line) {
 /* Implement This Function */
 //Creating a FILE pointer and open the file using fopen with read operation
  	FILE *fp =fopen(filename,"r");
	if(fp==NULL)
	{
		printf("File Not Found\n");
		return -1;
	}
  //read the file line by line into "line" using fgets
	char * readfile =fgets(line,MAX_LINE_LEN,fp);
	if(readfile==NULL)
	{
		printf("Nothing to read from file");
	}
  //closing the file
  fclose(fp);
	return 0;
}

/* (IMPLEMENT THIS FUNCTION)
 * Parses the Token to implement the rpn calculator features
 * Follow the Directions in the Project Documentation for this Function
 * You may implement this how you like, but many small functions would be good!
 * If the token you are passed in is NULL, return -1.
 * If there are any memory errors, return -1.
 */
static int parse_token(Symtab *symtab, Stack_head *stack, Token *tok) {
 /* Implement This Function */
   if (stack == NULL || tok == NULL) {
        return -1;
    } 
    //Control statements with logic based on the type of token    
    else {
        if (tok->type == TYPE_ASSIGNMENT) {
            //pop the tokens store them and put in hash
            Token *token1 = stack_pop(stack);
            Token *token2 = stack_pop(stack);
            hash_put(symtab, token2->variable, token1->value);
		        token_free(token1);
		        token_free(token2);
		        token_free(tok);			
        } 
        else if (tok->type == TYPE_OPERATOR) {
            int pop1 = 0,pop2 =0,result = 0;
            Token *token1 = stack_pop(stack);
            Token *token2 = stack_pop(stack);
            if (token1->type == TYPE_VARIABLE) {
                Symbol *temp = hash_get(symtab, token1->variable);
                pop1 = temp->val;
            } 
            else {
                pop1 = token1->value;
            }
            if (token2->type == TYPE_VARIABLE) {
                Symbol *temp_symbol = hash_get(symtab, token2->variable);
                pop2 = temp_symbol->val;
            } else {
                pop2 = token2->value;
            }
			
      //OPERATIONS BASED ON TOKEN OPERATOR USING SWITCH CASE
            switch (tok->oper) {
                case OPERATOR_PLUS:
                    result = pop2 + pop1;
				          	token_free(token1);
		                token_free(token2);
                    break;

                case OPERATOR_MINUS:
                    result = pop2 - pop1;
			          		token_free(token1);
                		token_free(token2);
                    break;

                case OPERATOR_MULT:
                    result = pop2 * pop1;
				          	token_free(token1);
		                token_free(token2);
                    break;

                case OPERATOR_DIV:
                    result = pop2 / pop1;
				          	token_free(token1);
                		token_free(token2);
                    break;
            }
		//ASSIGNMENTS FOR TOK
            tok->oper = 0;
            tok->type = TYPE_VALUE;
            tok->value = result;
            stack_push(stack, tok);
        } 
        else if (tok->type == TYPE_VARIABLE) {
            stack_push(stack, tok);
        } 
        else if (tok->type == TYPE_VALUE) {
            stack_push(stack, tok);
        } 
		
        else if (tok->type == TYPE_PRINT) {
            Token *token1 = stack_pop(stack);

            if (token1->type == TYPE_VARIABLE) {
				       Symbol *temp_symbol = hash_get(symtab, token1->variable);
               print_step_output(temp_symbol->val);
			      	//freeing tokens after use
              token_free(token1);
		          token_free(tok);
               } else {
              //printing the token value
                print_step_output(token1->value);
			        	token_free(token1);
		            token_free(tok);
            }
        }
    }
    return 0;
}

/* This has been written for you.
 * Prints out the main output header
 */
static void print_header(char *filename, int step) {
  printf("######### Beginning Program (%s) ###########\n", filename);
  printf("\n.-------------------\n");
  printf("| Program Step = %2d\n", step);
  token_print_remaining();
  printf("o-------------------\n");
}

/* This has been written for you.
 * Prints out the information at the top of each step
 */
static void print_step_header(int step) {
  printf("\n.-------------------\n");
  printf("| Program Step = %2d\n", step++);
}

/* This has been written for you.
 * Prints out the output value (print token) nicely
 */
static void print_step_output(int val) {
  printf("|-----Program Output\n");
  printf("| %d\n", val);
}

/* This has been written for you.
 * Prints out the information at the bottom of each step
 */
static void print_step_footer(Symtab *symtab, Stack_head *stack) {
  hash_print_symtab(symtab);
  stack_print(stack);
  token_print_remaining();
  printf("o-------------------\n");
}
