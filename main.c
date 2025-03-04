#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "./game.h"

typedef enum { UP, DOWN, LEFT, RIGHT } Direction;

typedef struct Saanp {
    int x;
    int y;
    Direction direction;
    struct Saanp *next;
} Saanp;

typedef struct {
    int x;
    int y;
} Food;

Saanp *head;
Saanp *tail;

Food food;

// initialize saanp
void init_saanp() {
    Saanp *saanp = malloc(sizeof(Saanp));

    saanp->x = (rand() % COLS) * CELL_SIZE;
    saanp->y = (rand() % ROWS) * CELL_SIZE;
    saanp->direction = RIGHT;

    saanp->next = NULL;

    head = saanp;
    tail = saanp;
}

// add segment
void add_segment() {
    Saanp *segment = malloc(sizeof(Saanp));
    segment->x = tail->x;
    segment->y = tail->y;
    segment->direction = tail->direction;
    segment->next = NULL;

    tail->next = segment;
    tail = segment;
}

// render saanp
void render_saanp(SDL_Renderer *renderer, int x, int y) {
    SDL_SetRenderDrawColor(renderer, 0x45, 0xFE, 0x02, 255);

    SDL_Rect segment;
    segment.w = CELL_SIZE;
    segment.h = CELL_SIZE;

    Saanp *current = head;

    while (current != NULL) {
        segment.x = x + current->x;
        segment.y = y + current->y;

        SDL_RenderFillRect(renderer, &segment);

        current = current->next;
    }
}

// move saanp
void move_saanp() {
    // Move the head
    int prev_x = head->x;
    int prev_y = head->y;

    switch (head->direction) {
        case UP:
            head->y -= CELL_SIZE;
            break;
        case DOWN:
            head->y += CELL_SIZE;
            break;
        case LEFT:
            head->x -= CELL_SIZE;
            break;
        case RIGHT:
            head->x += CELL_SIZE;
            break;
    }

    // Move the body segments
    Saanp *current = head->next;
    while (current != NULL) {
        int temp_x = current->x;
        int temp_y = current->y;
        current->x = prev_x;
        current->y = prev_y;
        prev_x = temp_x;
        prev_y = temp_y;
        current = current->next;
    }
}

// generate food
void generate_food() {
    bool inside_saanp;

    do {
        inside_saanp = false;
        food.x = (rand() % COLS);
        food.y = (rand() % ROWS);

        Saanp *current = head;

        // make sure food doesn't overlap with the body
        while (current != NULL) {
            if (current->x == food.x * CELL_SIZE && current->y == food.y * CELL_SIZE) {
                inside_saanp = true;
            }
            current = current->next;
        }
    } while (inside_saanp);
}

// render food
void render_food(SDL_Renderer *renderer, int x, int y) {
    SDL_SetRenderDrawColor(renderer, 0xFA, 0x4E, 0x58, 255);

    SDL_Rect foodRect;
    foodRect.w = CELL_SIZE;
    foodRect.h = CELL_SIZE;
    foodRect.x = x + food.x * CELL_SIZE;
    foodRect.y = y + food.y * CELL_SIZE;

    SDL_RenderFillRect(renderer, &foodRect);
}

// eat food
void eat_food() {
    if(head->x == food.x * CELL_SIZE && head->y == food.y * CELL_SIZE) {
        generate_food();
        add_segment();
    }
}

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

// detect collision
bool check_collision() {
    // check if the head hits the walls
    if (head->x < 0 || head->x >= COLS * CELL_SIZE ||
        head->y < 0 || head->y >= ROWS * CELL_SIZE) {
        return true;
    }

    // check if the head collides with its own body
    Saanp *current = head->next;
    while (current != NULL) {
        if (head->x == current->x && head->y == current->y) {
            return true;
        }
        current = current->next;
    }

    return false;
}


int main() {
    srand(time(0));

    // initialize saanp
    init_saanp();

    // generate food randomly
    generate_food();

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


   // Render initial state
    SDL_RenderClear(renderer);
    render_grid(renderer, grid_x, grid_y);
    render_saanp(renderer, grid_x, grid_y);
    render_food(renderer, grid_x, grid_y);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);


    SDL_Event event;
    int quit = 0;
   int game_started = 0;
    while (!quit) {
        while (!game_started) {
            while (SDL_PollEvent(&event)) {
                // click space to start the game
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_i) {
                    game_started = 1;
                }
            }
        }
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = 1;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            quit = 1;
                            break;
                        case SDLK_h:
                            if(head->direction != RIGHT)
                                head->direction = LEFT;
                            break;
                        case SDLK_j:
                            if(head->direction != UP)
                                head->direction = DOWN;
                            break;
                        case SDLK_k:
                            if(head->direction != DOWN)
                                head->direction = UP;
                            break;
                        case SDLK_l:
                            if(head->direction != LEFT)
                                head->direction = RIGHT;
                            break;

                    }
                default: {}
            }
        }
        SDL_RenderClear(renderer);

        move_saanp();

        if (check_collision()) {
            quit = 1;
        } else {
            eat_food();
        }

        render_grid(renderer, grid_x, grid_y);
        render_saanp(renderer, grid_x, grid_y);
        render_food(renderer, grid_x, grid_y);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderPresent(renderer);

        SDL_Delay(SPEED);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
