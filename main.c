#include <stdio.h>

#include <SDL2/SDL.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define ROWS 20
#define COLS 20
#define CELL_SIZE 20

// render a grid
void render_grid(SDL_Renderer * renderer, int x, int y) {
    SDL_SetRenderDrawColor(renderer, 0xCC, 0xCC, 0xCC, 255);

    SDL_Rect cell;
    cell.w = CELL_SIZE;
    cell.h = CELL_SIZE;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cell.x = x + (i * CELL_SIZE);
            cell.y = y + (j * CELL_SIZE);
            SDL_RenderDrawRect(renderer, &cell);
        }
    }
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Could not initialize sdl2: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow("Saanp",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH,
                                          WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN
                                          );

    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED |
                                                SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    int grid_x = (WINDOW_WIDTH - (COLS * CELL_SIZE)) / 2;
    int grid_y = (WINDOW_HEIGHT - (ROWS * CELL_SIZE)) / 2;

    SDL_Event event;
    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                default: {}
            }
        }
        SDL_RenderClear(renderer);
        // RENDER LOOP START

        render_grid(renderer, grid_x, grid_y);

        // RENDER LOOP END
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
