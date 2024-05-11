#ifndef PLOT_H
#define PLOT_H

#define PLOT_SIZE 500.0f  // 1:1 Aspect Ratio
#define PLOT_OFFSET 5.0f

#include <SDL2/SDL.h>

#include "../include/tsp.hpp"

int adjust(Coordinate coord, int graphSize);
void drawPoint(Coordinates coords, SDL_Renderer* renderer, int graphSize);
void drawLine(Coordinates p, Coordinates q, SDL_Renderer* renderer, int graphSize);
void plotGraph(vector<City>& cities, Path& path);

#endif