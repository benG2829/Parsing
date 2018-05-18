#include "parser.h"

int main(int argc, const char **argv)
{
   file_test(argc);
   read_file(argv);
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
  int x = 0;

  tg = (turtle_t*)malloc(sizeof(turtle_t));
  tg->wd = (char**)calloc(MAX_INPUT, sizeof(char*));
  while(x < MAX_INPUT){
   tg->wd[x] = (char*)calloc(MAX_LENGTH, sizeof(char));
   x++;
   }
  tg->array = MAX_INPUT;
  tg->now = 0;

  return *tg;
}

/*This function serves to swap over arrays if the input file is more than
the initial allocated memory. In this instance when the array gets to 80%
capacity this fucntion will swap over the contents to a temporary array*/
turtle_t enlarge_array(turtle_t *tg, int increase)
{
  int x = 0;
  int j = 0;
  int old_array;
  turtle_t *temp;

  old_array = tg->array;

  temp = (turtle_t*)malloc(sizeof(turtle_t));
  temp->wd = (char**)calloc(increase, sizeof(char*));
  while(x < increase){
     temp->wd[x] = (char*)calloc(MAX_LENGTH, sizeof(char));
     x++;
   }
   temp->array = increase;

  while(j < old_array){
   strcpy(temp->wd[j], tg->wd[j]);
   j++;
  }
  free(tg->wd);
  return *temp;
}

turtle_t initialise_struct(turtle_t *tg)
{
   tg->pg = (polish_t*)calloc(MAX_LENGTH, sizeof(polish_t));
   return *tg;
}

/*Turtle prog is the first part of the parser. It scans in the tgs from the
turtle file one by one and passes it to the next programme*/
void turtle_prog(FILE *turtle)
{
   turtle_t tg;
   int i = 0;
   int cnt = 0;
   int new_arr;

   tg = allocate_space(&tg);
   tg = initialise_struct(&tg);
   while(fscanf(turtle, "%s", tg.wd[i++]) == 1){
     cnt++;
     tg.now++;
     if(cnt > tg.array * MAX_CAPACITY){
       new_arr = new_array(&tg);
       tg = enlarge_array(&tg, new_arr);
     }
   }
   tg.now = 0;
   initialise_parser(&tg);
}

/*This function doubles the size of the array when further capacity is
required.*/
int new_array(turtle_t *tg)
{
  int new_array = tg->array * DOUBLE;
  return new_array;
}

/*Function checks that the first wd is a valid start to the parser. In
this instance an opening brace indicates this. If a do loop is requested
by the user, the program also initiates a stack for it*/
void initialise_parser(turtle_t *tg)
{
  if(strcmp(tg->wd[tg->now], "{") != 0){
    fprintf(stderr, "No opening brace [%s] PIC [%d]?\n",
    tg->wd[tg->now], tg->now);
    exit(2);
  }
  NEXT_STRING;
  code(tg);
}

/*Function checks that whether the text file has been fully read, if not
it continues to loop, else it finishes the parsing by finding a closing brace.
This will also mean that the SDL functionality will cease*/
void code(turtle_t *tg)
{
  if(strcmp(tg->wd[tg->now], "}") == 0){
    return;
  }
  instruction_statement(tg);
  code(tg);
}

/*This is the main body of the parser. Here the program sorts between instructions
and directs the next operation to the required function. A stack is also initialised
for polish grammar functionality*/
void instruction_statement(turtle_t *tg)
{
  if(strcmp(tg->wd[tg->now], "FD") == 0){
   find_length(tg);
  }
  else if(strcmp(tg->wd[tg->now], "DO") == 0){
   do_grammar(tg);
  }
  else if(strcmp(tg->wd[tg->now], "SET") == 0){
   check_set(tg);
  }
  else if(strcmp(tg->wd[tg->now], "RT") == 0){
   find_angle(tg);
  }
  else if(strcmp(tg->wd[tg->now], "LT") == 0){
   find_angle(tg);
  }
  instruct_list_check(tg);
}

/*Issues were found in the parser that a correct check had to be made to
ensure that variable had not been mispelt - otherwise it would loop around and
the program would eventually segmentation fault. As a result this function
checks to ensure that the correct instruction set has been provided.*/
void instruct_list_check(turtle_t *tg)
{
  if(strcmp(tg->wd[tg->now], "FD") == 0 || strcmp(tg->wd[tg->now], "DO") == 0
  || strcmp(tg->wd[tg->now], "SET") == 0 || strcmp(tg->wd[tg->now], "RT") == 0
  || strcmp(tg->wd[tg->now], "LT") == 0 || strcmp(tg->wd[tg->now], "}") == 0){
    code(tg);
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
statement.*/
void do_grammar(turtle_t *tg)
{
   NEXT_STRING;
   check_VAR(tg);
   check_varnum(tg);
   check_brace(tg);
   code(tg);
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
    fprintf(stderr, "Are braces correct [%s] PIC [%d]?\n",
    tg->wd[tg->now], tg->now);
    exit(2);
   }
}

/*Check set undertakes the operations requried within "SET" including assigning
polish operators with VAR/ VARNUM. This is then returned as a value for a value
for SET which can be computed by the program*/
void check_set(turtle_t *tg)
{
   check_VAR(tg);
   SET_VAR(tg);
   NEXT_STRING;
   assign_var(tg);
   free(tg->pg);
   return;
}

/*Function assigns any variables input by the user are assigned to the SET
for this iteration. Function moves from the ";=" through to the closing semi colon.
Error can be caught either in the polish function or here*/
void assign_var(turtle_t *tg)
{
  if(strcmp(tg->wd[tg->now], ":=") == 0){
   NEXT_STRING;
  }
  else{
   fprintf(stderr, "Incorrect SET grammar [%s] PIC [%d]\n", tg->wd[tg->now],
   tg->now);
   exit(2);
  }
   while(strcmp(tg->wd[tg->now], ";") != 0){
     polish_grammar(tg);
     if(strcmp(tg->wd[tg->now], ";") == 0){
      NEXT_STRING;
      return;
     }
   }
}

/*Polish grammar function looks for an ascii code which corresponds to the
operator required [+*-/]. If this is not found, the function then searches
other functions (check_var and check_varnum) to ascertain whether they are
valid VAR inputs. If not it is then rejected.*/
void polish_grammar(turtle_t *tg)
{
   char *p = tg->wd[tg->now];
   int cnt = 42;
   int max = 47;

   /*If the value is found, the string is copied across to a polish grammar array,
   which is later used when converting the polish into a SET_VAR. If it */
   while(cnt <= max){
   if(strchr(p, 43) || strchr(p, 45) || strchr(p, 42)|| strchr(p, 47)){
    if(strlen(p) < 2){
    NEXT_STRING;
    return;
    }
    else{
      fprintf(stderr, "Incorrect POLISH grammar [%s] PIC [%d]\n",
      tg->wd[tg->now], tg->now);
      exit(2);
    }
   }
   else if(cnt == max){
     if(isdigit(tg->wd[tg->now][0])){
      check_varnum(tg);
      return;
     }
     if(isalpha(tg->wd[tg->now][0])){
      SET_VAR(tg);
      NEXT_STRING;
      return;
     }
     else{
      fprintf(stderr, "Incorrect POLISH grammar [%s] PIC [%d]\n",
      tg->wd[tg->now], tg->now);
      exit(2);
     }
   }
   cnt++;
 }
}

/*SET VAR finds the corresponding value of a VAR, it is simply to cyle from
A-Z. If a VAR is not found then an error statement is printed.*/
void SET_VAR(turtle_t *tg)
{
   int A = 65;
   int i = 65;
   int Z = 91;

   if(strcmp(tg->wd[tg->now], ":=") == 0){
    NEXT_STRING;
   }

   while(A < Z){
      if(strchr(tg->wd[tg->now], A)){
       tg->var_cnt = A - i;
       return;
      }
   A++;
   }
}

/*Due to the level of words included in the formal grammar which atcually do
not represent computational variables, i felt that this function was necessary to
simply skip through any unwanted/ unnecessary grammar. The programs design is
such that should these words be missing they will be caught by another function.*/
void skip_string(turtle_t *tg)
{
   if(strcmp(tg->wd[tg->now], "TO") == 0){
    NEXT_STRING;
    return;
   }
   else if(strcmp(tg->wd[tg->now], "FROM") == 0){
    NEXT_STRING;
    return;
   }
}

/*This function checks for a VARS corresponding value. VAR values are stored
in an array (see var_t) which cycles through the alphabet and stores the
corresponding value. If at any point a string is input taht contains a digit/ or
is not the expected format (i.e. not a capital letter), the function will reject
it on the basis of grammar deviation*/
void check_VAR(turtle_t *tg)
{
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
       fprintf(stderr, "Is VAR correct [%s] PIC [%d] ?\n",
       tg->wd[tg->now], tg->now);
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
   int Z = 91;
   char *p = tg->wd[tg->now];

   while(A < Z){
      if(strchr(p, A)){
       tg->var_cnt = A - i;
         if(tg->num[tg->var_cnt] < 1){
          NEXT_STRING;
          skip_string(tg);
          check_varnum(tg);
          return;
         }
         else{
          NEXT_STRING;
          skip_string(tg);
          check_varnum(tg);
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
void check_varnum(turtle_t *tg)
{
   int i;
   int check;
   char d_point = '.';
   check = strlen(tg->wd[tg->now]);

   for(i = 0; i < check; i++){
   if(!isdigit(tg->wd[tg->now][i])){
      if(isalpha(tg->wd[tg->now][0]) && strlen(tg->wd[tg->now]) < 2){
       SET_VAR(tg);
       NEXT_STRING;
       return;
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
   NEXT_STRING;
   return;
  }
}

/*Find angle fucntion computes whether or not a variable found is RT or LT. The
variable provided will be a RT or LT instrcution and the value as an integer is
found in this function. The value is returned and converted to a radian in
instructlist*/
void find_angle(turtle_t *tg)
{
   if(strcmp(tg->wd[tg->now], "RT") == 0){
    NEXT_STRING;
     if(isalpha(tg->wd[tg->now][0])){
      SET_VAR(tg);
     }
     else{
      check_varnum(tg);
     }
     return;
    }
    if(strcmp(tg->wd[tg->now], "LT") == 0){
     NEXT_STRING;
      if(isalpha(tg->wd[tg->now][0])){
       SET_VAR(tg);
      }
      else{
      check_varnum(tg);
    }
    return;
   }
}

/*This function computes the length found following the FD instruction. because
of the use of sets/ nested loops, the function searches previous words in some
instances to establish where the command has been generated from. This ensures
clairty and that the correct VAR is pulled through to the function thus enabling
appropriate computation.*/
void find_length(turtle_t *tg)
{
   if(strcmp(tg->wd[tg->now], "FD") == 0){
    NEXT_STRING;
    if(isalpha(tg->wd[tg->now][0])){
     SET_VAR(tg);
    }
    else{
    check_varnum(tg);
     }
    }
    return;
}
