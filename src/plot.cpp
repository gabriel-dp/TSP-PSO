#include "../include/plot.hpp"

#include <SDL2/SDL.h>

#include <algorithm>
#include <iostream>

#include "../include/tsp.hpp"

using namespace std;

// Adjust an coordinate position due to plot and graph sizes difference
int adjust(Coordinate coord, int graphSize) {
    const double adjust = PLOT_SIZE / graphSize;
    return (int)(coord * adjust);
}

// Draw a point with some adjusts
void drawPoint(Coordinates coords, SDL_Renderer* renderer, int graphSize) {
    SDL_RenderDrawPoint(renderer, adjust(coords.first + PLOT_OFFSET / 2, graphSize), adjust((graphSize - PLOT_OFFSET / 2 - coords.second), graphSize));
}

// Draw a line with some adjusts
void drawLine(Coordinates p, Coordinates q, SDL_Renderer* renderer, int graphSize) {
    SDL_RenderDrawLine(renderer, adjust(p.first + PLOT_OFFSET / 2, graphSize), adjust((graphSize - PLOT_OFFSET / 2 - p.second), graphSize), adjust(q.first + PLOT_OFFSET / 2, graphSize), adjust((graphSize - PLOT_OFFSET / 2 - q.second), graphSize));
}

// Function to compare cities by their max coordinate
bool compareCityCoodinates(City& a, City& b) {
    return max(a.position.first, a.position.second) < max(b.position.first, b.position.second);
}

// Function to plot the graph of all cities and a path on it
void plotGraph(vector<City>& cities, Path& path) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Best Solution Found", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, PLOT_SIZE + PLOT_OFFSET, PLOT_SIZE + PLOT_OFFSET, SDL_WINDOW_SHOWN);

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    City furtherCity = *max_element(cities.begin(), cities.end(), compareCityCoodinates);
    int graphSize = max(furtherCity.position.first, furtherCity.position.second) + PLOT_OFFSET;

    // Draw lines between cities in the path
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red
    vector<Vertex> vertexes = path.getVertexes();
    for (auto i = vertexes.begin(); next(i) != vertexes.end(); i++) {
        drawLine(cities[*i].position, cities[*next(i)].position, renderer, graphSize);
    }
    drawLine(cities[*vertexes.begin()].position, cities[*vertexes.rbegin()].position, renderer, graphSize);

    // Draw all points
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);  // Light gray
    for (City& city : cities) {
        drawPoint(city.position, renderer, graphSize);
    }

    // Render to screen
    SDL_RenderPresent(renderer);
    cout << "Graph Plotted!\n";

    // Wait until user closes window
    SDL_Event event;
    int quit = 0;
    while (!quit) {
        SDL_Delay(250);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}