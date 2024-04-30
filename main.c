#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>

int gridwidth=10;
int gridheight=20;
int grid[10][20];
int fallingBlock[10][20];
int blockPos = 1;
int blockPosX = 2;

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

#define CHK(value) grid[shape.pos[value][0]+x][shape.pos[value][1]+y] == 1
#define CHKCHK CHK(0) || CHK(1) || CHK(2) || CHK(3)

bool collisionCheck(int grid[gridwidth][gridheight], int x, int y, struct shape shape){
  if(CHKCHK || x > gridwidth || x < 0 || y > gridheight || y < 0){
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
  /* if(!collisionCheck(grid, x, y, shape) && !(a==10)){ */
  /*   /\* grid[x][y]=1; *\/ */
  /*   /\* grid[x][y+1]=1; *\/ */
  /*   /\* DRW(1, 0, 1) *\/ */
  /*   a++; */
  /*   goto recurse; */
  /* } else { */
    /* grid[x][y]=1; */
    /* grid[x][y-1]=1; */
    DRW(1, 0, 0-a)
  /* } */
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

  struct shape shapes[7];

  #define SHPCRTR(shape, x1, y1, x2, y2, x3, y3, x4, y4) {shapes[shape].pos[0][0] = x1; \
      shapes[shape].pos[0][1] = y1; \
      shapes[shape].pos[1][0] = x2; \
      shapes[shape].pos[1][1] = y2; \
      shapes[shape].pos[2][0] = x3; \
      shapes[shape].pos[2][1] = y3; \
      shapes[shape].pos[3][0] = x4; \
      shapes[shape].pos[3][1] = y4;}

  SHPCRTR(0, 0, 0, 1, 0, 1, 1, 2, 0); // t shape
  SHPCRTR(1, 0, 0, 1, 0, 1, 1, 0, 1); // o shape
  SHPCRTR(2, 0, 0, 0, 1, 0, 2, 0, 3); // i shape
  SHPCRTR(3, -1, 0, 0, 0, 0, 1, 1, 1); // z shape
  SHPCRTR(6, 0, 0, 1, 0, 0, 1, -1, 1); // s shape
  SHPCRTR(4, 0, 0, 0, 1, 0, 2, 1, 2); // L shape
  SHPCRTR(5, 0, 0, 0, 1, 0, 2, -1, 2); // J shape

  for (int i=0;i<12;i++){
    for (int j=0;j<6;j++){
      grid[j][i]=0;
      fallingBlock[j][i]=0;
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
                  if (event.key.keysym.sym == SDLK_UP) {
                    // Up Arrow
                  } else if (event.key.keysym.sym == SDLK_DOWN && fps - lastMove > fastfall && !collisionCheck(grid, blockPosX, blockPos+1, shapes[currentShape])) {
                    // Down Arrow
                    blockPos++;
                    lastMove = fps;
                    goto fastupdate;
                  } else if (event.key.keysym.sym == SDLK_LEFT && fps - lastMove > das && !collisionCheck(grid, blockPosX-1, blockPos, shapes[currentShape])) {
                    // Left Arrow
                    blockPosX-=1;
                    lastMove = fps;
                    goto fastupdate;
                  } else if (event.key.keysym.sym == SDLK_RIGHT && fps - lastMove > das && !collisionCheck(grid,blockPosX+1,blockPos, shapes[currentShape])) {
                    // Right Arrow
                    blockPosX+=1;
                    lastMove = fps;
                    goto fastupdate;
                  }
                  break;
            }
        }
    /* gaming = false; */
    fps++;

    if(grid[2][0]==1){gaming=false;}

    SDL_Delay(50);

    if(fps % 7 == 0){
      fastupdate:
      // spawn block in 3rd column
      for (int i=0;i<gridheight;i++){
        for (int j=0;j<gridwidth;j++){
          fallingBlock[j][i]=0;
          printf("%d ", grid[j][i]);
        }
        printf("\n");
      }
      drawBlock(blockPosX, blockPos, shapes[currentShape]);

      /* fallingBlock[blockPosX][0+blockPos+1] = 1; */
      /* fallingBlock[blockPosX][1+blockPos+1] = 1; */

      if(fps % 7 == 0){
        blockPos++;
        printf("%d", blockPos);
      }

      if(blockPos > gridheight-2 || collisionCheck(grid, blockPosX, blockPos, shapes[currentShape])) {
        addBackgroundBlock(blockPosX, blockPos-1, shapes[currentShape]);
        blockPos=2;
        blockPosX=2;
        if(currentShape==6){currentShape=-1;}
        currentShape++;
      }

      /* printf("this is the block pos: %i\n", blockPos); */

      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 250);
      SDL_RenderClear(renderer);

      for (int i=0;i<gridwidth;i++){
        for (int j=0;j<gridheight;j++){

        SDL_Rect rect;
        rect.x = 0+i*32;
        rect.y = 0+j*32;
        rect.w = 32;
        rect.h = 32;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        if(fallingBlock[i][j] == 0 && grid[i][j] == 0){


          SDL_RenderDrawRect(renderer, &rect);
        } else {
          if(fallingBlock[i][j] == 1){
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
          }
          SDL_RenderFillRect(renderer, &rect);
        }
        }
      }

      // Render text
      /* SDL_Rect textRect = {50, 50+blockPos, textSurface->w, textSurface->h}; // rectangle where the text is drawn */
      /* SDL_RenderCopy(renderer, textTexture, NULL, &textRect); */

      SDL_RenderPresent(renderer);

    /* printf("gaming"); */
    }


  }

  printf("ur bad");


  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  return EXIT_SUCCESS;
  return 0;
}
