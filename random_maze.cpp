#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL.h>
#include <thread>
using namespace std;

#define ROWS 10
#define COLS 10
#define SIZE 30

struct CELL {
    unsigned int rows;
    unsigned int colmns;
    unsigned int visited;
    unsigned int rightdir;
    unsigned int downdir;
};

unsigned int get_random_int(const int max) {
    return rand() % max;
}

class Stack {
public:
    CELL* arrdata;
    unsigned int top;
    unsigned int size;

    Stack(unsigned int size) {
        this->size = size;
        arrdata = new CELL[size];
        top = -1;
    }

    void push(CELL* data) {
        if (top + 1 == size) {
            cout << "Stack is full" << endl;
        }
        else {
            top++;
            arrdata[top] = *data;
        }
    }

    CELL* getTop() {
        if (top == -1) {
            cout << "Stack is empty" << endl;
            exit(1);
        }
        return &arrdata[top];
    }

    void pop() {
        if (top == -1) {
            cout << "Stack underflow" << endl;
        }
        else {
            top--;
        }
    }

    bool isEmpty() {
        return top == -1;
    }
};

void Stack_insert(Stack* stacks, CELL* cells) {
    if (stacks->top + 1 == stacks->size) {
        cout << "Stack is full" << endl;
        exit(1);
    }
    else {
        stacks->top++;
        stacks->arrdata[stacks->top] = *cells;
    }
}

CELL* Check_stack(Stack* stacks, const unsigned int rows, const unsigned int colmns) {
    if (stacks->isEmpty()) {
        cout << "Stack is empty" << endl;
        exit(1);
    }
    for (int i = 0; i < stacks->top; i++) {
        if (stacks->arrdata[i].rows == rows && stacks->arrdata[i].colmns == colmns) {
            return &(stacks->arrdata[i]);
        }
    }
    return nullptr;
}

unsigned int CheckStack(Stack* stacks, unsigned int rows, unsigned int colmns) {
    if (stacks->isEmpty()) {
        cout << "Stack is empty" << endl;
        exit(1);
    }
    for (int i = 0; i < stacks->top; i++) {
        if (stacks->arrdata[i].rows == rows && stacks->arrdata[i].colmns == colmns) {
            return 1;
        }
    }
    return 0;
}

int IsFreeNeighbours(Stack* stacks, CELL* cells) {
    unsigned int count = 0;
    if (CheckStack(stacks, cells->rows + 1, cells->colmns) && !Check_stack(stacks, cells->rows + 1, cells->colmns)->visited) {
        count++;
    }
    if (CheckStack(stacks, cells->rows - 1, cells->colmns) && !Check_stack(stacks, cells->rows - 1, cells->colmns)->visited) {
        count++;
    }
    if (CheckStack(stacks, cells->rows, cells->colmns + 1) && !Check_stack(stacks, cells->rows, cells->colmns + 1)->visited) {
        count++;
    }
    if (CheckStack(stacks, cells->rows, cells->colmns - 1) && !Check_stack(stacks, cells->rows, cells->colmns - 1)->visited) {
        count++;
    }
    return count;
}

CELL* choose_CELL_neighbours(Stack* stacks, CELL* cells) {
    unsigned int nighbours = 0;
    CELL* neighbourcount[4];
    if (CheckStack(stacks, cells->rows + 1, cells->colmns) && !Check_stack(stacks, cells->rows + 1, cells->colmns)->visited) {
        neighbourcount[nighbours++] = Check_stack(stacks, cells->rows + 1, cells->colmns);
    }
    if (CheckStack(stacks, cells->rows - 1, cells->colmns) && !Check_stack(stacks, cells->rows - 1, cells->colmns)->visited) {
        neighbourcount[nighbours++] = Check_stack(stacks, cells->rows - 1, cells->colmns);
    }
    if (CheckStack(stacks, cells->rows, cells->colmns + 1) && !Check_stack(stacks, cells->rows, cells->colmns + 1)->visited) {
        neighbourcount[nighbours++] = Check_stack(stacks, cells->rows, cells->colmns + 1);
    }
    if (CheckStack(stacks, cells->rows, cells->colmns - 1) && !Check_stack(stacks, cells->rows, cells->colmns - 1)->visited) {
        neighbourcount[nighbours++] = Check_stack(stacks, cells->rows, cells->colmns - 1);
    }
    if (nighbours > 0) {
        unsigned int ran = get_random_int(nighbours);

        // Only set the direction flags based on relative positions
        if (cells->rows > neighbourcount[ran]->rows) {
            neighbourcount[ran]->downdir = 0;
        }
        if (cells->rows < neighbourcount[ran]->rows) {
            cells->downdir = 0;
        }
        if (cells->colmns > neighbourcount[ran]->colmns) {
            neighbourcount[ran]->rightdir = 0;
        }
        if (cells->colmns < neighbourcount[ran]->colmns) {
            cells->rightdir = 0;
        }

        return neighbourcount[ran];
    }
    return nullptr;
}

void cell_creation(Stack* stacks) {
    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            CELL cells;
            cells.rows = y;
            cells.colmns = x;
            cells.rightdir = 1;
            cells.downdir = 1;
            cells.visited = 0;
            Stack_insert(stacks, &cells);
        }
    }
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    srand(time(NULL));
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return 1;
    }

    // Create an SDL window
    SDL_Window* window = SDL_CreateWindow("Cell Structure Grid",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        500,
        500,
        SDL_WINDOW_SHOWN);
    if (window == NULL) {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        return 1;
    }

    // Create an SDL renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        return 1;
    }

    // Clear the screen
    Stack stacks(ROWS * COLS);
    Stack cells(ROWS * COLS);
    cell_creation(&cells);
    CELL* cell = Check_stack(&cells, 0, 0);
    Stack_insert(&stacks, cell);
    cell->visited = 1;
    int visited_blocks = 1;

    // Set the cell color
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    // Create a thread for maze generation
    while (visited_blocks < (ROWS * COLS)) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        if (IsFreeNeighbours(&stacks, cell) > 0) {
            cell = choose_CELL_neighbours(&stacks, cell);
            if (cell) {
                cell->visited = 1;
                Stack_insert(&stacks, cell);
                visited_blocks++;
            }
        }
        else {
            if (!stacks.isEmpty()) {
                stacks.pop();
                cell = stacks.getTop();
            }
            else {
                break;
            }
        }

        SDL_Event event;

        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }
        }
    }

    for (int y = 0; y < ROWS; y++) {
        for (int x = 0; x < COLS; x++) {
            SDL_Rect rect;
            rect.x = x * (SIZE + 10);
            rect.y = y * (SIZE + 10);
            rect.h = SIZE;
            rect.w = SIZE;

            CELL* cell = Check_stack(&cells, x, y);
            if (cell->downdir == 0) {
                rect.h += 5;
            }
            if (cell->rightdir == 0) {
                rect.w += 5;
            }

            if (cell->visited) {
                SDL_SetRenderDrawColor(renderer, 222, 35, 70, SDL_ALPHA_OPAQUE);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
            }

            SDL_RenderFillRect(renderer, &rect);
        }
    }

    // Continue with SDL rendering loop
    bool quit = false;
    while (!quit) {
        // Handle SDL events (e.g., quit event)
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        // Render your maze here based on the current state

        // Present the renderer to display the grid
        SDL_RenderPresent(renderer);
    }

    SDL_Delay(5000);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    delete[] cells.arrdata;
    delete[] stacks.arrdata;
    return 0;
}