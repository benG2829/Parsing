#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define MAX_LENGTH 10
#define MAX_INPUT 5
#define MAX_CAPACITY 0.8
#define INITIAL 1
#define PG 2
#define A_NUM 26
#define DOUBLE 2
#define NEXT_STRING tg->now++;

struct pol{
  char polish[PG][MAX_INPUT];
};
typedef struct pol polish_t;

struct turtle{
  double num[A_NUM];
  polish_t *pg;
  char **wd;
  int array;
  int var_cnt;
  int now, max;
  int close, set_var;
  int p, p_max;
  double length;
  double angle, angle_prev;
};
typedef struct turtle turtle_t;

void file_test(int user_defined_file);
void file_check(FILE *turtle);
void read_file(const char **argv);
turtle_t allocate_space(turtle_t *tg);
turtle_t enlarge_array(turtle_t *tg, int increase);
turtle_t initialise_struct(turtle_t *tg);
int new_array(turtle_t *tg);
void turtle_prog(FILE *turtle);
void initialise_parser(turtle_t *tg);
void code(turtle_t *tg);
void instruction_statement(turtle_t *tg);
void instruct_list_check(turtle_t *tg);
void do_grammar(turtle_t *tg);
void check_brace(turtle_t *tg);
void check_set(turtle_t *tg);
void assign_var(turtle_t *tg);
void polish_grammar(turtle_t *tg);
void SET_VAR(turtle_t *tg);
void skip_string(turtle_t *tg);
void check_VAR(turtle_t *tg);
void allocate_VAR(turtle_t *tg);
void check_varnum(turtle_t *tg);
void find_angle(turtle_t *tg);
void find_length(turtle_t *tg);
