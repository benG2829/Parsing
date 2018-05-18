#include "interpreter.h"

/*I chose not to use the neillsdl2.c wrapper so that i could better understand
the implementation of SDL. However, the same functions have been essentially created*/
void start_SDL(SDL_t *start)
{
  static const int WIDTH = 800;
  static const int HEIGHT = 600;

  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    fprintf(stderr, "\nUnable to initialise SDL\n");
    SDL_Quit();
    exit(1);
  }

  start->finished = 0;

  start->window = SDL_CreateWindow("SDL Window",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          WIDTH, HEIGHT,
                          SDL_WINDOW_SHOWN);

  if(start->window == NULL){
      fprintf(stderr, "\nUnable to initialize SDL Window:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   start->renderer = SDL_CreateRenderer(start->window, -1, 0);
   if(start->renderer == NULL){
      fprintf(stderr, "\nUnable to initialize SDL Renderer:  %s\n", SDL_GetError());
      SDL_Quit();
      exit(1);
   }

   SDL_SetRenderDrawColor(start->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
   SDL_RenderClear(start->renderer);
   SDL_RenderPresent(start->renderer);
}

void destroy_SDL(SDL_t *start)
{
  SDL_DestroyRenderer(start->renderer);
  SDL_DestroyWindow(start->window);
}

void turtle_graphic_event(SDL_t *start)
{
   SDL_Event event;
   while(SDL_PollEvent(&event))
   {
       switch (event.type){
          case SDL_QUIT:
          case SDL_MOUSEBUTTONDOWN:
          case SDL_KEYDOWN:
             start->finished = 1;
       }
    }
}

/*The turtle go function is the initial function following on from when strings
are read into the file. The function intialises an SDL window first and then
the parser begins. If the parser fails at any point, the window will be exited
mid way through the drawing of any lines. The SDL window is kept open for
5 seconds to show the user what has been drawn.*/
void turtle_go(turtle_t *tg, SDL_t *start)
{
   turtle_draw(tg);
   start_SDL(start);

   initialise_parser(tg, start);
   SDL_Delay(5000);

   destroy_SDL(start);
   atexit(SDL_Quit);
}

/*Initiates variables such as drawing location on the SDL window, angle is
set to 0*/
void turtle_draw(turtle_t *tg)
{
   tg->angle = 0;
   *tg = start(tg);
}

/*The turtle print function takes in the former angle, copies it and updates
the coordinates to the next drawing location. A delay is set to a few
milliseconds to ensure that the link is printed slowly. The SDL screen having
been rendered black is then drawn on with a white line*/
turtle_t turtle_print(turtle_t *tg, SDL_t *s)
{
    *tg = copy(tg);
    *tg = co_ordinate(tg, tg->angle);

    do{
    SDL_Delay(MILLISECONDDELAY);

    SDL_SetRenderDrawColor(s->renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(s->renderer, tg->x, tg->y, tg->x1, tg->y1);

    SDL_RenderPresent(s->renderer);
    SDL_UpdateWindowSurface(s->window);
    turtle_graphic_event(s);

   }while(!s->finished);

   tg->x = tg->x1;
   tg->y = tg->y1;

    return *tg;
}
