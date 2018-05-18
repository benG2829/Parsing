#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

#define MAX_LENGTH 10
#define MAX_INPUT 100
#define TG_START 0
#define INITIAL 1
#define PG 2
#define MILLISECONDDELAY 150
#define A_NUM 26
#define NEXT_STRING tg->now++;
#define MAX_CAPACITY 0.8
#define DOUBLE 2

struct SDL{
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_bool finished;
};
typedef struct SDL SDL_t;

struct stackelem{
  double i;
  struct stackelem *prev;
};
typedef struct stackelem Elem_t;

struct the_stack{
  Elem_t *tp;
};
typedef struct the_stack Stack_t;

struct do_stack{
  double state;
  struct do_stack *prev;
};
typedef struct do_stack DoStack_t;

struct do_loop{
  DoStack_t *tp;
};
typedef struct do_loop Loop_t;

struct var{
  double num[A_NUM];
};
typedef struct var var_t;

struct pol{
  char polish[MAX_LENGTH][MAX_LENGTH];
};
typedef struct pol polish_t;

struct turtle{
  var_t *t;
  polish_t *pg;
  char wd[MAX_INPUT][MAX_LENGTH];
  int var_cnt;
  int now;
  int array;
  int max;
  int close;
  int set_var;
  int p, p_max;
  double length;
  double angle, angle_prev;
  double x, x1, y, y1;
};
typedef struct turtle turtle_t;

void file_test(int user_defined_file);
void file_check(FILE *turtle);
void read_file(const char **argv);
turtle_t allocate_space(turtle_t *tg);
void turtle_prog(FILE *turtle);
void initialise_parser(turtle_t *tg, SDL_t *start);
void code(turtle_t *tg, SDL_t *start, Loop_t *dl);
void instruction_statement(turtle_t *tg, SDL_t *start, Loop_t *dl);
double check_varnum(turtle_t *tg);
void check_VAR(turtle_t *tg);
void check_set(turtle_t *tg, Stack_t *s);
void do_grammar(turtle_t *tg, SDL_t *start, Loop_t *dl);
void loop(turtle_t *tg, SDL_t *start, Loop_t *dl);
void polish_grammar(turtle_t *tg, Stack_t *s);
void convert_set(turtle_t *tg, Stack_t *s);
void check_brace(turtle_t *tg);
int ALPHA_CHECK (turtle_t *tg);
turtle_t enlarge_array(turtle_t *tg, int increase);
int new_array(turtle_t *tg);
void free_space(turtle_t *tg);

void start_SDL(SDL_t *start);
turtle_t begin_SDL(FILE *turtle);
void destroy_SDL(SDL_t *start);
void turtle_graphic_event(SDL_t *start);
void turtle_go(turtle_t *tg, SDL_t *start);
turtle_t turtle_print(turtle_t *tg, SDL_t *start);
turtle_t start(turtle_t *tg);
turtle_t co_ordinate(turtle_t *tg, double angle);
turtle_t copy(turtle_t *tg);
turtle_t move_iteration(turtle_t *tg);
double find_angle(turtle_t *tg);
double reverse_angle(double angle);
int find_length(turtle_t *tg);
double convert_to_radian(int angle);
void turtle_draw(turtle_t *tg);
void allocate_VAR(turtle_t *tg);
void find_var(turtle_t *tg);
void assign_var(turtle_t *tg, Stack_t *s);
void skip_string(turtle_t *tg);
void update_var(turtle_t *tg);
void instruct_list_check(turtle_t *tg, SDL_t *start, Loop_t *dl);

void initialise_stack(Stack_t *stack);
void push(Stack_t *s, double n);
double pop(Stack_t *s);
void initialise_loop_stack(Loop_t *s);
void stack_push_var(Loop_t *s, double n);
double stack_pop(Loop_t *s);
void refresh(turtle_t *tg, Loop_t *dl);
void push_down(turtle_t *tg, Loop_t *dl);
void check_stack(turtle_t *tg, SDL_t *start, Loop_t *dl);
void check_complete(turtle_t *tg, Loop_t *dl);
void SET_VAR(turtle_t *tg);
turtle_t initialise_struct(turtle_t *tg);
