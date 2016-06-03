#include <SDL2/SDL.h>
#include "random.h"
#include <cstdio>
#include <ctime>
#include <unordered_set>

struct World
{
    int width, height;
    std::unordered_set<int> cells;

    World(int width, int height)
    {
        this->width = width;
        this->height = height;
    }

    inline static int hash_cell(int row, int col)
    {
        int hash = 23;
        hash = hash * 31 + row;
        hash = hash * 31 + col;
        return hash;
    }

    bool has(int row, int col)
    {
        int hash = hash_cell(row, col);
        return cells.find(hash) != cells.end();
    }

    void add(int row, int col)
    {
        cells.insert(hash_cell(row, col));
    }

    void remove(int row, int col)
    {
        cells.erase(hash_cell(row, col));
    }

    int neighbor_count(int row, int col)
    {
        int count = 0;
        if (has(row - 1, col)) count++;
        if (has(row + 1, col)) count++;
        if (has(row, col - 1)) count++;
        if (has(row, col + 1)) count++;
        if (has(row - 1, col - 1)) count++;
        if (has(row + 1, col - 1)) count++;
        if (has(row - 1, col + 1)) count++;
        if (has(row + 1, col + 1)) count++;
        return count;
    }

    void step()
    {
        World old = *this;
        for (int row = 0; row < height; ++row) {
            for (int col = 0; col < width; ++col) {
                bool dead = old.has(row, col);
                int ncount = old.neighbor_count(row, col);
                if (dead) {
                    if (ncount == 3) {
                        add(row, col);
                    }
                }
                else {
                    if (ncount < 2 || ncount > 3) {
                        remove(row, col);
                    }
                }
            }
        }
    }
};

struct RenderConfig
{
    int cellWidth;
    int cellHeight;
};

void render_world(World* world, SDL_Renderer* renderer, SDL_Point* offset, RenderConfig* config);

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;
    const int CELL_WIDTH = 16;
    const int CELL_HEIGHT = 16;
    const int CELL_COUNT = 100;

    SDL_Window* window = SDL_CreateWindow("life", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int worldWidth = SCREEN_WIDTH / CELL_WIDTH, worldHeight = SCREEN_HEIGHT / CELL_HEIGHT;
    World world(worldWidth, worldHeight);

    Random64 random(time(NULL));

    for (int i = 0; i < CELL_COUNT; ++i) {
        int64_t index = random.get(worldWidth * worldHeight);

        int r = (int)index / worldWidth;
        int c = (int)index % worldHeight;

        world.add(r, c);
    }

    RenderConfig config;
    config.cellWidth = CELL_WIDTH;
    config.cellHeight = CELL_HEIGHT;

    SDL_Point cameraOffset = { 0, 0 };

    bool isRunning = true;
    bool shouldStep = false;

    int inputAxisX = 0;
    int inputAxisY = 0;

    float moveSpeed = 10.f;

    while (isRunning) {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    isRunning = false;
                }
                else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                    shouldStep = !shouldStep;
                }

                if (event.key.repeat == false) {
                    if (event.key.keysym.scancode == SDL_SCANCODE_UP) inputAxisY--;
                    else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) inputAxisY++;
                    else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) inputAxisX--;
                    else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) inputAxisX++;
                }
                break;

            case SDL_KEYUP:
                if (event.key.keysym.scancode == SDL_SCANCODE_UP) inputAxisY++;
                else if (event.key.keysym.scancode == SDL_SCANCODE_DOWN) inputAxisY--;
                else if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) inputAxisX++;
                else if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) inputAxisX--;
            }
        }

        if (shouldStep) {
            printf("stepping ");
            world.step();
        }

        cameraOffset.x += inputAxisX * moveSpeed;
        cameraOffset.y += inputAxisY * moveSpeed;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        render_world(&world, renderer, &cameraOffset, &config);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}

void render_world(World* world, SDL_Renderer* renderer, SDL_Point* offset, RenderConfig* config)
{
    int cellWidth = 32;
    int cellHeight = 32;

    if (config != NULL) {
        cellWidth = config->cellWidth;
        cellHeight = config->cellHeight;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect bgRect = { -offset->x, -offset->y, world->width * cellWidth, world->height * cellHeight };
    SDL_RenderFillRect(renderer, &bgRect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_Rect cellRect = { 0, 0, cellWidth, cellHeight };

    for (int row = 0; row < world->height; ++row) {
        for (int col = 0; col < world->width; ++col) {
            if (world->has(row, col)) {
                cellRect.x = col * cellWidth - offset->x;
                cellRect.y = row * cellHeight - offset->y;
                SDL_RenderFillRect(renderer, &cellRect);
            }
        }
    }
}