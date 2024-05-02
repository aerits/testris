#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>

int gridwidth=12;
int gridheight=24;
int grid[12][24];
int fallingBlock[12][24];
int blockPos = 5;
int blockPosX = 4;

struct shape {
  int pos[4][2];
};

void displayGrid(int grid[gridwidth][gridheight]){
  for (int i=0;i<gridheight;i++){
    for (int j=0;j<gridwidth;j++){
      printf("%i ", grid[j][i]);
    }
    printf("\n");
  }
}

#define CHK(value, value2) grid[shape.pos[value][0]+x][shape.pos[value][1]+y] == value2
#define CHKCHK(a) CHK(0, a) || CHK(1, a) || CHK(2, a) || CHK(3, a)

bool collisionCheck(int grid[gridwidth][gridheight], int x, int y, struct shape shape){
  if(CHKCHK(1) || CHKCHK(3) || x > gridwidth || x < 0 || y > gridheight || y < 0){
    return true;
  }
  return false;
}

#undef CHK
#undef CHKCHK

#define DRW(value, modx, mody) {\
for (int i=0;i<4;i++){\
  fallingBlock[shape.pos[i][0]+x+modx][shape.pos[i][1]+y+mody] = value;\
}}

void drawBlock(int x, int y, struct shape shape) {
  if(!collisionCheck(grid, x, y, shape)) {
    /* fallingBlock[x][y]=1; */
    /* fallingBlock[x][y+1]=1; */

    DRW(1, 0, 0)
  }
}
void clearBlock(int x, int y, struct shape shape) {
  if(collisionCheck(fallingBlock, x, y, shape)){
    /* fallingBlock[x][y]=0; */
    /* fallingBlock[x][y+1]=0; */

    DRW(0, 0, 0)
  }
}
#undef DRW
#define DRW(value, modx, mody) {\
for (int i=0;i<4;i++){\
  grid[shape.pos[i][0]+x+modx][shape.pos[i][1]+y+mody] = value;\
}}

void addBackgroundBlock(int x, int y, struct shape shape) {
  int a = 0;
  /* recurse: */
  if(!collisionCheck(grid, x, y+1, shape) && !collisionCheck(grid, x, y-1, shape) && !(a==10)){
    /* grid[x][y]=1; */
    /* grid[x][y+1]=1; */
    DRW(1, 0, -1)
    /* a++; */
    /* goto recurse; */
  } else {
    /* grid[x][y]=1; */
    /* grid[x][y-1]=1; */
    DRW(1, 0, 0)
  }
}

#undef DRW


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int main(int argc, char *argv[]) {
  // sdl boilerplate
  if (SDL_Init(SDL_INIT_VIDEO) < 0){
    printf("Couldn't initialize SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }
  // Initialize SDL_ttf
  if (TTF_Init() < 0) {
      printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
      SDL_Quit();
      return EXIT_FAILURE;
  }

  SDL_Window *window = SDL_CreateWindow("testris", SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  if (!window){
    printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
    return EXIT_FAILURE;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  if (!renderer){
    printf("Failed to create renderer: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  TTF_Font *font = TTF_OpenFont("ttf/Roboto-Regular.ttf", 64);

  if (!font){
    printf("Failed to load font: %s\n", TTF_GetError());
    return EXIT_FAILURE;
  }

  // Create surface with rendered text
  SDL_Color textColor = {0, 0, 0, 255}; // black color
  SDL_Surface *textSurface = TTF_RenderText_Solid(font, "Hello World!", textColor);

  if (!textSurface) {
    printf("Failed to create text surface: %s\n", TTF_GetError());
    return EXIT_FAILURE;
  }

  // Create texture from surface
  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

  if (!textTexture){
    printf("Failed to create text texture: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }



  // initialize variables

  srand(time(NULL));

  struct shape shapes[28];

  #define SHPCRTR(shape, x1, y1, x2, y2, x3, y3, x4, y4) {shapes[shape].pos[0][0] = x1; \
      shapes[shape].pos[0][1] = y1; \
      shapes[shape].pos[1][0] = x2; \
      shapes[shape].pos[1][1] = y2; \
      shapes[shape].pos[2][0] = x3; \
      shapes[shape].pos[2][1] = y3; \
      shapes[shape].pos[3][0] = x4; \
      shapes[shape].pos[3][1] = y4;}

  SHPCRTR(0, 0, 0, 1, 0, 0, -1, -1, 0); // t shape
  SHPCRTR(1, 0, 0, 1, 0, 1, 1, 0, 1); // o shape
  SHPCRTR(2, -1, 0, 0, 0, 1, 0, 2, 0); // i shape
  SHPCRTR(3, -1, 0, 0, 0, 0, 1, 1, 1); // s shape
  SHPCRTR(6, 0, 0, 1, 0, 0, 1, -1, 1); // z shape
  SHPCRTR(4, 0, 0, -1, 0, 1, 0, 1, -1); // L shape
  SHPCRTR(5, 0, 0, -1, 0, 1, 0, -1, -1); // J shape

  SHPCRTR(7, 0, 0, 1, 0, 0, 1, 0, -1); // t shape
  SHPCRTR(8, 0, 0, 1, 0, 1, 1, 0, 1); // o shape
  SHPCRTR(9, 0, 0, 0, 1, 0, -1, 0, -2); // i shape
  /* SHPCRTR(10, -1, 0, 0, 0, 0, 1, 1, 1); // s shape */
  /* SHPCRTR(11, 0, 0, 1, 0, 0, 1, -1, 1); // z shape */
  /* SHPCRTR(12, 0, 0, 0, -1, 0, 1, 1, 1); // L shape */
  /* SHPCRTR(13, 0, 0, 0, -1, 0, 1, -1, 1); // J shape */

  SHPCRTR(14, 0, 0, 1, 0, 0, 1, -1, 0); // t shape
  SHPCRTR(15, 0, 0, 1, 0, 1, 1, 0, 1); // o shape
  SHPCRTR(16, -1, 0, 0, 0, 1, 0, 2, 0); // i shape
  /* SHPCRTR(17, -1, 0, 0, 0, 0, 1, 1, 1); // s shape */
  /* SHPCRTR(18, 0, 0, 1, 0, 0, 1, -1, 1); // z shape */
  /* SHPCRTR(19, 0, 0, 0, -1, 0, 1, 1, 1); // L shape */
  /* SHPCRTR(20, 0, 0, 0, -1, 0, 1, -1, 1); // J shape */

  SHPCRTR(21, 0, 0, -1, 0, 0, -1, 0, 1); // t shape
  SHPCRTR(22, 0, 0, 1, 0, 1, 1, 0, 1); // o shape
  SHPCRTR(23, 0, 0, 0, 1, 0, -1, 0, -2); // i shape
  /* SHPCRTR(24, -1, 0, 0, 0, 0, 1, 1, 1); // s shape */
  /* SHPCRTR(25, 0, 0, 1, 0, 0, 1, -1, 1); // z shape */
  /* SHPCRTR(26, 0, 0, 0, -1, 0, 1, 1, 1); // L shape */
  /* SHPCRTR(27, 0, 0, 0, -1, 0, 1, -1, 1); // J shape */

  for (int i=0;i<gridheight;i++){
    for (int j=0;j<gridwidth;j++){
      grid[j][i]=0;
      fallingBlock[j][i]=0;
      if(j==0 || i==gridheight-1 || j==gridwidth-1){
        grid[j][i]=3;
      }
    }
  }

  bool gaming = true;

  /* displayGrid(grid); */

  SDL_Event event;

  int fps = 0;
  int lastMove = 0;
  int das = 1;
  int fastfall = 0;
  int currentShape = 0;
  int slidetime = 10;
  int lastslide = 0;
  int onFloor = false;

  while(gaming) {

    while (SDL_PollEvent(&event) > 0)
        {
            switch (event.type)
            {
                case SDL_QUIT:
                  gaming = false;
                  break;
                case SDL_KEYDOWN:
                  /* printf("keys are down"); */
                  if(event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT){
                    lastslide -= 1;
                  }
                  if (event.key.keysym.sym == SDLK_SPACE) {
                    // Up Arrow
                    lastslide = fps;
                    continueGoingDown:
                    if(!collisionCheck(grid, blockPosX, blockPos, shapes[currentShape])){
                      blockPos++;
                      goto continueGoingDown;
                    }
                  } else if (event.key.keysym.sym == SDLK_DOWN && fps - lastMove > fastfall && !collisionCheck(grid, blockPosX, blockPos, shapes[currentShape]) && !collisionCheck(grid, blockPosX, blockPos+1, shapes[currentShape])) {
                    // Down Arrow
                    blockPos++;
                    lastslide = fps;
                    lastMove = fps;
                    goto fastupdate;
                  } else if (event.key.keysym.sym == SDLK_LEFT && fps - lastMove > das && !collisionCheck(grid, blockPosX-1, blockPos -1, shapes[currentShape])) {
                    // Left Arrow
                    blockPosX-=1;
                    lastslide -= 1;
                    lastMove = fps;
                    goto fastupdate;
                  } else if (event.key.keysym.sym == SDLK_RIGHT && fps - lastMove > das && !collisionCheck(grid,blockPosX+1,blockPos -1, shapes[currentShape])) {
                    // Right Arrow
                    blockPosX+=1;
                    lastslide -= 1;
                    lastMove = fps;
                    goto fastupdate;
                  } else if (event.key.keysym.sym == SDLK_x){
                    if(currentShape > 20){
                      currentShape = currentShape+7-28;
                    } else {
                      currentShape+=7;
                    }
                    goto fastupdate;
                  } else if (event.key.keysym.sym == SDLK_z){
                    if(currentShape<8){
                      currentShape +=21;
                    } else {
                      currentShape-=7;
                    }
                  }
                  break;
            }
        }
    /* gaming = false; */
    fps++;

    if(grid[2][0]==1){gaming=false;}

    SDL_Delay(50);
    drawBlock(blockPosX, blockPos-1, shapes[currentShape]);
      for (int i=0;i<gridheight;i++){
        int a = 0;
        for (int j=0;j<gridwidth;j++){
          fallingBlock[j][i]=0;
          if (grid[j][i]==1){
            a++;
          }
          /* printf("%d ", grid[j][i]); */
        }
        /* printf("\n"); */
        if(a==10){
          // clear the line
          for(int k=0;i-k>0;k++){
            for (int j=0;j<gridwidth;j++){
              grid[j][i-k] = grid[j][i-k-1];
            }
          }
        }
      }
    drawBlock(blockPosX, blockPos-1, shapes[currentShape]);

    if(fps % 7 == 0){
      fastupdate:
      // spawn block in 3rd column
      /* drawBlock(blockPosX, blockPos, shapes[currentShape]); */

      /* fallingBlock[blockPosX][0+blockPos+1] = 1; */
      /* fallingBlock[blockPosX][1+blockPos+1] = 1; */

      if(fps % 7 == 0 && !onFloor){
        blockPos++;
        printf("%d", blockPos);
      }
    }
    if(collisionCheck(grid, blockPosX, blockPos, shapes[currentShape]) && !onFloor) {
      onFloor = true;
      lastslide = fps;
    }
    if(!collisionCheck(grid, blockPosX, blockPos, shapes[currentShape]) && onFloor) {
      onFloor = false;
      lastslide = fps;
    }

    if(collisionCheck(grid, blockPosX, blockPos, shapes[currentShape]) && fps - lastslide > slidetime) {
      onFloor = false;
      addBackgroundBlock(blockPosX, blockPos-1, shapes[currentShape]);
      blockPos=5;
      blockPosX=4;
      currentShape = rand() % 7;
      if(collisionCheck(grid, blockPosX, blockPos+1, shapes[currentShape])){
        gaming=false;
      }
    }

    /* printf("this is the block pos: %i\n", blockPos); */

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 250);
    SDL_RenderClear(renderer);

    for (int i=0;i<gridwidth;i++){
      for (int j=0;j<gridheight;j++){

      SDL_Rect rect;
      rect.x = 0+i*32;
      rect.y = 0+j*32 - (32*4);
      rect.w = 32;
      rect.h = 32;

      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      if(fallingBlock[i][j] == 0 && grid[i][j] == 0){


        SDL_RenderDrawRect(renderer, &rect);
      }
      else if (grid[i][j] == 3){
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
      }
      else {
        if(fallingBlock[i][j] == 1){
          SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }
        SDL_RenderFillRect(renderer, &rect);
      }
      }
    }

    /* SDL_Color textColor = {0, 0, 0, 255}; // black color */
    /* SDL_Surface *textSurface = TTF_RenderText_Solid(font, "gaming", textColor); */
    /* SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface); */


    /* // Render text */
    /* SDL_Rect textRect = {50, 50+blockPos, textSurface->w, textSurface->h}; // rectangle where the text is drawn */
    /* SDL_RenderCopy(renderer, textTexture, NULL, &textRect); */

    SDL_RenderPresent(renderer);

    /* printf("gaming"); */


  }

  printf("ur bad");


  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  /* return EXIT_SUCCESS; */
  return 0;
}
