#include "interpreter.h"

int main(int argc, const char **argv)
{
   turtle_t tg;
   file_test(argc);
   read_file(argv);
   free(&tg);
   return 0;
}

/*checking that the correct argc is being used by the user*/
void file_test(int user_defined_file)
{
   if(user_defined_file != 2){
    fprintf(stderr, "\nError! Try using ./parse turtle_draw.txt\n\n");
    exit(2);
   }
}

/*Checking that the file is not NULL. If it is reject*/
void file_check(FILE *turtle)
{
   if(turtle == NULL){
    fprintf(stderr, "\nError with file! This file does not support the request!\n\n");
    exit(2);
   }
}

/*Read file ensures that the correct file is passed into the program and read.
It also checks that the file is not null and then scans in the tgs one by one*/
void read_file(const char **argv)
{
   FILE *turtle;

   turtle = fopen(argv[1], "r");
   file_check(turtle);
   turtle_prog(turtle);
   fclose(turtle);
}

/*Allocate space ensures that there is enough memory allocated to undertake the
reuired functionality of the input file. This also initialises memory for other
structs which are pulled through turtle_t*/
turtle_t allocate_space(turtle_t *tg)
{
   tg = (turtle_t*)calloc(MAX_INPUT, sizeof(turtle_t));
   *tg = initialise_struct(tg);
   return *tg;
}

turtle_t initialise_struct(turtle_t *tg)
{
  int x = 0;
  tg->t = (var_t*)calloc(A_NUM, sizeof(var_t));
  tg->pg = (polish_t*)calloc(MAX_LENGTH, sizeof(polish_t));
  while(x < MAX_LENGTH){
   tg->pg->polish[x][0] = (char)malloc(sizeof(char)*PG);
   tg->pg->polish[x][0] = 0;
   x++;
  }

  return *tg;
}

/*This function serves to swap over arrays if the input file is more than
the initial allocated memory. In this instance when the array gets to 80%
capacity this fucntion will swap over the contents to a temporary array*/
turtle_t enlarge_array(turtle_t *tg, int increase)
{
  int j = 0;
  int old_array;
  turtle_t temp;

  temp = allocate_space(tg);

  old_array = tg->array;
  temp.array = increase;

  while(j < old_array){
   strcpy(temp.wd[j], tg->wd[j]);
   j++;
  }
  return temp;
}

/*This function doubles the size of the array when further capacity is
required.*/
int new_array(turtle_t *tg)
{
  int new_array = tg->array * DOUBLE;
  return new_array;
}

/*Turtle prog is the first part of the parser. It scans in the tgs from the
turtle file one by one and passes it to the next programme*/
void turtle_prog(FILE *turtle)
{
   turtle_t tg;
   SDL_t start;
   int cnt = 0;
   int i = 0;

   tg = allocate_space(&tg);

   while(fscanf(turtle, "%s", tg.wd[i++]) == 1){
     ++cnt;
   }

   tg.now = 0;
   turtle_go(&tg, &start);
}

/*Function checks that the first wd is a valid start to the parser. In
this instance an opening brace indicates this. If a do loop is requested
by the user, the program also initiates a stack for it*/
void initialise_parser(turtle_t *tg, SDL_t *start)
{
  Loop_t dl;
  initialise_loop_stack(&dl);

  if(strcmp(tg->wd[tg->now], "{") != 0){
    fprintf(stderr, "No opening brace?\n");
    exit(2);
  }
  NEXT_STRING;
  code(tg, start, &dl);
}

/*Function checks that whether the text file has been fully read, if not
it continues to loop, else it finishes the parsing by finding a closing brace.
This will also mean that the SDL functionality will cease*/
void code(turtle_t *tg, SDL_t *start, Loop_t *dl)
{
  if(strcmp(tg->wd[tg->now], "}") == 0){
    return;
  }
  instruction_statement(tg, start, dl);
  code(tg, start, dl);
}

/*This is the main body of the parser. Here the program sorts between instructions
and directs the next operation to the required function. A stack is also initialised
for polish grammar functionality*/
void instruction_statement(turtle_t *tg, SDL_t *start, Loop_t *dl)
{
  Stack_t s;
  initialise_stack(&s);

  if(strcmp(tg->wd[tg->now], "FD") == 0){
    tg->length = find_length(tg);
    *tg = turtle_print(tg, start);
  }
  else if(strcmp(tg->wd[tg->now], "DO") == 0){
    do_grammar(tg, start, dl);
  }
  /*Set has also been included in this function as i believe its prupose
  serves closely to that of what is required from the main instrcution list*/
  else if(strcmp(tg->wd[tg->now], "SET") == 0){
    check_set(tg, &s);
  }
  /*Both RT and LT functions contain much more functionality than that of FD
  or DO. This is because of the requirements to compute angles and convert
  them into a format which can be rrepresented on the SDL window (radians)*/
  else if(strcmp(tg->wd[tg->now], "RT") == 0){
   tg->angle_prev = tg->angle;
   tg->angle = find_angle(tg);
   tg->angle = convert_to_radian(tg->angle);
   tg->angle = tg->angle + tg->angle_prev;
  }
  else if(strcmp(tg->wd[tg->now], "LT") == 0){
   tg->angle_prev = tg->angle;
   tg->angle = find_angle(tg);
   tg->angle = convert_to_radian(tg->angle);
   tg->angle = tg->angle + tg->angle_prev;
  }
  instruct_list_check(tg, start, dl);
}

/*Issues were found in the parser that a correct check had to be made to
ensure that variable had not been mispelt - otherwise it would loop around and
the program would eventually segmentation fault. As a result this function
checks to ensure that the correct instruction set has been provided.*/
void instruct_list_check(turtle_t *tg, SDL_t *start, Loop_t *dl)
{
  if(strcmp(tg->wd[tg->now], "FD") == 0 || strcmp(tg->wd[tg->now], "DO") == 0
  || strcmp(tg->wd[tg->now], "SET") == 0 || strcmp(tg->wd[tg->now], "RT") == 0
  || strcmp(tg->wd[tg->now], "LT") == 0 || strcmp(tg->wd[tg->now], "}") == 0){
   code(tg, start, dl);
   return;
 }
 else{
   fprintf(stderr, "Incorrect Instruction [%s] PIC [%d]?\n", tg->wd[tg->now],
   tg->now);
   exit(2);
  }
}

/*The do grammar function serves to ensure that if a do loop is requested by the
user it is processed in a different manner to a more simple FD 100, LT 45,
statement. It works by pushing vital bits of information regarding the state
of the do loop, such as the position within the text file, current iteration of
loop, max and the current var. When it reaches the end (a closing brace), the
loop then resets itself back to the intitiation of the do loop.*/
void do_grammar(turtle_t *tg, SDL_t *start, Loop_t *dl)
{
   int i;

   stack_push_var(dl, tg->now);
   NEXT_STRING;
   check_VAR(tg);
   tg->max = check_varnum(tg);
   check_brace(tg);
   push_down(tg, dl);

   i = tg->t->num[tg->var_cnt];
   while(i <= tg->max){
    code(tg, start, dl);
      if(strcmp(tg->wd[tg->now], "}") == 0){
       tg->close = tg->now;
        if(dl->tp->prev != NULL){
         refresh(tg, dl);
        }
      }
   i++;
   }
   check_stack(tg, start, dl);
}

/*The check status function looks to see whether the current iteration has reached
the maximum for the loop. If it has, it will pop off the loop previous to it. If
the function then cycles through again and the cnt is one greater, then this means
that a further iteration needs to be popped off and the function moves up a loop
iteration. A value for each loop iteration is alwaya found.*/
void check_stack(turtle_t *tg, SDL_t *start, Loop_t *dl)
{
   if(tg->t->num[tg->var_cnt] >= tg->max){
     if(tg->t->num[tg->var_cnt] >= tg->max+1){
       if(dl->tp->prev != NULL){
       refresh(tg, dl);
      }
     }
     if(dl->tp->prev != NULL){
       tg->t->num[tg->var_cnt] = 0;
       refresh(tg, dl);
       return;
      }
    else{
      tg->now = tg->close;
      code(tg, start, dl);
    }
 }
}

/*Function pushes down information required to operate the fucntionality
for a nested loop*/
void push_down(turtle_t *tg, Loop_t *dl)
{
  stack_push_var(dl, tg->max);
  stack_push_var(dl, tg->t->num[tg->var_cnt]);
  stack_push_var(dl, tg->var_cnt);
}

/*Refresh pops an entire loop state at once, meaning that this can automatically
regenerate a previous loop iterations state*/
void refresh(turtle_t *tg, Loop_t *dl)
{
  tg->var_cnt = stack_pop(dl);
  tg->t->num[tg->var_cnt] = stack_pop(dl);
  tg->max = stack_pop(dl);
  tg->now = stack_pop(dl);
}

/*Function ensures that if the program reaches a brace and its either in the
wromg place or there isnt a brace, it will warn the user of an input error.*/
void check_brace(turtle_t *tg)
{
  if(strcmp(tg->wd[tg->now], "{") == 0 ||
     strcmp(tg->wd[tg->now], "}") == 0){
   NEXT_STRING;
   return;
  }
  else{
       fprintf(stderr, "Are braces correct?\n");
       exit(2);
   }
}

/*Check set undertakes the operations requried within "SET" including assigning
polish operators with VAR/ VARNUM. This is then returned as a value for a value
for SET which can be computed by the program*/
void check_set(turtle_t *tg, Stack_t *s)
{
  check_VAR(tg);
  SET_VAR(tg);
  tg->set_var = tg->var_cnt;
  NEXT_STRING;
  assign_var(tg, s);

  tg->p = 0;
  /*While loop ensures that polish grammar is undertaken until completed*/
  while(tg->p <= tg->p_max){
   convert_set(tg, s);
   tg->p++;
  }
  return;
}

/*Function assigns any variables input by the user are assigned to the SET
for this iteration. Function moves from the ";=" through to the closing semi colon.
Error can be caught either in the polish function or here*/
void assign_var(turtle_t *tg, Stack_t *s)
{
  NEXT_STRING;
  while(strcmp(tg->wd[tg->now], ";") != 0){
    polish_grammar(tg, s);
    if(strcmp(tg->wd[tg->now], ";") == 0){
    NEXT_STRING;
    return;
    }
  }
  if(strcmp(tg->wd[tg->now], ";") != 0){
   fprintf(stderr, "IS SET GRAMMAR CORRECT?\n");
   exit(2);
  }
}

/*Polish grammar function looks for an ascii code which corresponds to the
operator required [+*-/]. If this is not found, the function then searches
other functions (check_var and check_varnum) to ascertain whether they are
valid VAR inputs. If not it is then rejected.*/
void polish_grammar(turtle_t *tg, Stack_t *s)
{
  char *p = tg->wd[tg->now];
  int cnt = 42;
  int max = 48;
  double val;

  tg->p = 0;

  while(cnt <= max){
  /*If the value is found, the string is copied across to a polish grammar array,
  which is later used when converting the polish into a SET_VAR. If it */
   if(strchr(p, 43) || strchr(p, 45) || strchr(p, 42)|| strchr(p, 47)){
    if(tg->pg->polish[tg->p][0] == 0){
     strcpy(tg->pg->polish[tg->p], p);
     tg->p_max = tg->p;
     tg->p++;
     NEXT_STRING;
     return;
    }
    else{
      NEXT_STRING;
      return;
    }
   }
   else if(cnt == max){
     if(isdigit(tg->wd[tg->now][0])){
       val = check_varnum(tg);
       push(s, val);
       return;
     }
     if(isalpha(tg->wd[tg->now][0])){
       SET_VAR(tg);
       val = tg->t->num[tg->var_cnt];
       push(s, val);
       NEXT_STRING;
       return;
     }
     /*If conditions arent met then an error is thrown which will show the
     user the problem*/
     else {
     fprintf(stderr, "Incorrect POLISH grammar [%s]\n", tg->wd[tg->now]);
     exit(2);
    }
   }
   cnt++;
 }
}

/*The convert set takes the variables in between strings := and ;, and computes
the polish expression provided by the user. This is done on the basis of last
on the stack is the value with an operator is then applied to and the second
value is then computed. The combined value is then pushed back onto the stack
so that SET can be computed at a later point*/
void convert_set(turtle_t *tg, Stack_t *s)
{
   double value;
   double value_two;

   if(strcmp(tg->pg->polish[tg->p], "*") == 0){
     value = pop(s);
     value_two = pop(s);
     tg->t->num[tg->set_var] = value * value_two;
     push(s, tg->t->num[tg->set_var]);
   }
   else if(strcmp(tg->pg->polish[tg->p], "/") == 0){
     value = pop(s);
     value_two = pop(s);
     tg->t->num[tg->set_var] = value_two / value;
     push(s, tg->t->num[tg->set_var]);
   }
   else if(strcmp(tg->pg->polish[tg->p], "+") == 0){
     value = pop(s);
     value_two = pop(s);
     tg->t->num[tg->set_var] = value_two + value;
     push(s, tg->t->num[tg->set_var]);
   }
   else if(strcmp(tg->pg->polish[tg->p], "-") == 0){
     value = pop(s);
     value_two = pop(s);
     tg->t->num[tg->set_var] = value_two - value;
     push(s, tg->t->num[tg->set_var]);
   }
}

/*SET VAR finds the corresponding value of a VAR, it is simply to cyle from
A-Z. If a VAR is not found then an error statement is printed.*/
void SET_VAR(turtle_t *tg)
{
   int A = 65;
   int i = 65;
   int Z = 91;

   while(A < Z){
    if(strchr(tg->wd[tg->now], A)){
    tg->var_cnt = A - i;
    return;
    }
    else if(A == Z){
     fprintf(stderr, "VAR NOT FOUND [%s] ?\n", tg->wd[tg->now]);
     exit(2);
    }
    A++;
   }
   return;
}

/*Due to the level of words included in the formal grammar which atcually do
not represent computational variables, i felt that this function was necessary to
simply skip through any unwanted/ unnecessary grammar. The programs design is
such that should these words be missing they will be caught by another function.*/
void skip_string(turtle_t *tg)
{
  if(strcmp(tg->wd[tg->now], "TO") == 0){
    NEXT_STRING;
  }
  if(strcmp(tg->wd[tg->now], "FROM") == 0){
    NEXT_STRING;
  }
  if(strcmp(tg->wd[tg->now], ":=") == 0){
    NEXT_STRING;
  }
}

/*This function checks for a VARS corresponding value. VAR values are stored
in an array (see var_t) which cycles through the alphabet and stores the
corresponding value. If at any point a string is input taht contains a digit/ or
is not the expected format (i.e. not a capital letter), the function will reject
it on the basis of grammar deviation*/
void check_VAR(turtle_t *tg)
{
  /*AS VAR can also be numbers it is imperative
  that this is checked first*/
  if(isdigit(tg->wd[tg->now][0])){
    check_varnum(tg);
    return;
  }
  /*This checks whether if an alpha char has been found, it conforms to the
  requirements. Due to the complexity of the grammar i felt it necessary to
  retain simply pass through if SET was found*/
  if(!isupper(tg->wd[tg->now][0]) || strcmp(tg->wd[tg->now], "SET") == 0){
    if(strlen(tg->wd[tg->now]) > 1){
      NEXT_STRING;
      return;
    }
  }
  else{
    if(!isalpha(tg->wd[tg->now][0]) || strlen(tg->wd[tg->now]) > 1){
      fprintf(stderr, "Is VAR correct [%s] ?\n", tg->wd[tg->now]);
      exit(2);
    }
  }
  allocate_VAR(tg);
  skip_string(tg);
  return;
}

/*Once a var has been found, a value has to be assigned to it. In very few
instances will a VAR not have a value assigned to it. Similar to the set_var
function the program cycles through ALPHA char and when it finds a corresponding
letter, converts the next string (if required) and stores it as a value in
VAR. If it has been found previously then the value is updated*/
void allocate_VAR(turtle_t *tg)
{
   int A = 65;
   int i = 65;
   int Z = 90;
   char *p = tg->wd[tg->now];
   double val;

   while(A < Z){
    if(strchr(p, A)){
      tg->var_cnt = A - i;
     if(tg->t->num[tg->var_cnt] < 1){
       NEXT_STRING;
       skip_string(tg);
       val = check_varnum(tg);
       tg->t->num[tg->var_cnt] = val;
       return;
     }
     else{
       NEXT_STRING;
       skip_string(tg);
       val = check_varnum(tg);
       tg->t->num[tg->var_cnt] = val + tg->t->num[tg->var_cnt];
       return;
     }
    }
    A++;
   }
   return;
}

/*Function checks to establish whether a number (VARNUM) is valid. If it isn't
it will be rejected. Similar to the parser, comparison looks for a decimal point
for floating point numbers and will skip if found. This retains the capacity to
compute floating points, without compromising accuracy of number*/
double check_varnum(turtle_t *tg)
{
  int i;
  double val;
  int check;
  char d_point = '.';
  check = strlen(tg->wd[tg->now]);

  for(i = 0; i < check; i++){
     if(!isdigit(tg->wd[tg->now][i])){
        if(isalpha(tg->wd[tg->now][0]) && strlen(tg->wd[tg->now]) < 2){
         SET_VAR(tg);
         val = tg->t->num[tg->var_cnt];
         NEXT_STRING;
         return val;
        }
        if(strchr(tg->wd[tg->now], d_point)){
         i++;
        }
        else{
         fprintf(stderr, "Expecting a VARNUM [%s] PIC [%d] ?\n",
         tg->wd[tg->now], tg->now);
         exit(2);
        }
     }
  }
  /*Check finally to see if its a val(assumption is obviously correct)*/
  if(isdigit(tg->wd[tg->now][0])){
   val = atof(tg->wd[tg->now]);
   NEXT_STRING;
   return val;
  }
  return 0;
}

/*Function copies a previous iteration to the next points. Given the co-ordinates
required to draw with SDL (X, X1, Y and Y1), it is imperative that these are
updated after every iteration*/
turtle_t copy(turtle_t *tg)
{
   tg->x1 = tg->x;
   tg->y1 = tg->y;
   return *tg;
}

/*This function converts a value to a radian (necessary for plotting locations in
SDL). The function logic was found on stack overflow */
double convert_to_radian(int angle)
{
   double radians;
   radians = angle / (180 / M_PI);
   return radians;
}

/*logically when plotting points, left is the opposite to right. As a result
any left value provided by the user is computed initially as normal, but then
reversed. This ensures accuracy of direction of line*/
double reverse_angle(double angle)
{
   double new_angle;
   new_angle = angle - (angle*2);
   return new_angle;
}

/*Possibly the most important function of the entire interpreter, the
co-ordinate function correctly computes (using cos and sin) the next location
of the instruction to be drawn. This is implmented in the turtle print function
just before an instruction is printed.*/
turtle_t co_ordinate(turtle_t *tg, double angle)
{
   tg->x1 += cos(angle)*tg->length;
   tg->y1 += sin(angle)*tg->length;
   return *tg;
}

/*Find angle fucntion computes whether or not a variable found is RT or LT. The
variable provided will be a RT or LT instrcution and the value as an integer is
found in this function. The value is returned and converted to a radian in
instructlist*/
double find_angle(turtle_t *tg)
{
   double angle;
   double new_angle;

   if(strcmp(tg->wd[tg->now], "RT") == 0){
    NEXT_STRING;
    angle = check_varnum(tg);
    return angle;
   }
   if(strcmp(tg->wd[tg->now], "LT") == 0){
     NEXT_STRING;
     angle = check_varnum(tg);
     new_angle = reverse_angle(angle);
     return new_angle;
   }
   return 0;
}

/*This function computes the length found following the FD instruction. because
of the use of sets/ nested loops, the function searches previous words in some
instances to establish where the command has been generated from. This ensures
clairty and that the correct VAR is pulled through to the function thus enabling
appropriate computation.*/
int find_length(turtle_t *tg)
{
   double length;

   if(strcmp(tg->wd[tg->now-1], ";") == 0){
    NEXT_STRING;
    SET_VAR(tg);
    tg->set_var = tg->var_cnt;
    length = tg->t->num[tg->set_var];
    NEXT_STRING;
    return length;
   }
    else{
     NEXT_STRING;
     length = check_varnum(tg);
     return length;
    }
   return 0;
}

/*I have implemneted two stacks throughout the parser and interpreter. obviously
one stack cannot be used for mutliple tasks, given the nature of popping from
the last addition - if the last addition bears no relationship to the next. Both
stacks have been allocated using the course notes for simplicity and to retain
accuracy*/
void initialise_stack(Stack_t *s)
{
   s->tp = (Elem_t *)calloc(INITIAL, sizeof(Elem_t));
   if(s->tp == NULL){
    fprintf(stderr, "MEMORY ALLOCATION ISSUE\n");
    exit(2);
   }
}

void push(Stack_t *s, double n)
{
   Elem_t *e;

   e = (Elem_t *)calloc(INITIAL, sizeof(Elem_t));
   if(e == NULL){
    fprintf(stderr, "MEMORY ALLOCATION ISSUE\n");
    exit(2);
   }
   e->prev = s->tp;
   s->tp->i = n;
   s->tp = e;
}

double pop(Stack_t *s)
{
   s->tp = s->tp->prev;
   assert(s->tp != NULL);
   return s->tp->i;
}

/*A loop stack was also created so that information could be popped on and off
as and when needed by the do_loop grammar. Its inclusion is vital to the
operational efficiency of the program*/
void initialise_loop_stack(Loop_t *s)
{
    s->tp = (DoStack_t *)calloc(INITIAL, sizeof(DoStack_t));

    if(s->tp == NULL){
     fprintf(stderr, "MEMORY ALLOCATION ISSUE\n");
     exit(2);
    }
}

void stack_push_var(Loop_t *s, double n)
{
   DoStack_t *e;

   e = (DoStack_t *)calloc(INITIAL, sizeof(DoStack_t));
   if(e == NULL){
    fprintf(stderr, "MEMORY ALLOCATION ISSUE\n");
    exit(2);
   }
   e->prev = s->tp;
   s->tp->state = n;
   s->tp = e;
}

double stack_pop(Loop_t *s)
{
   s->tp = s->tp->prev;
   assert(s->tp != NULL);
   return s->tp->state;
}

/*Function initiates the first co-ordinated of the SDL picture, which in this
instance is directly in the middle of the SDL display*/
turtle_t start(turtle_t *tg)
{
   tg->x = 400.00;
   tg->y = 300.00;
   return *tg;
}
