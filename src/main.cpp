#include <iostream>
#include <vector>

#include "../include/pso.hpp"
#include "../include/tsp.hpp"

std::vector<City> getAllCities() {
    std::vector<City> cities;
    int n;
    Coordinate x, y;

    std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::cin >> x >> y;

        City newCity;
        newCity.vertex = i + 1;
        newCity.position = std::pair(x, y);

        cities.push_back(newCity);
    }

    return cities;
}

int main() {
    std::vector<City> cities = getAllCities();
    PSO tsp = PSO(generateCompleteGraph(cities), 100, 10);
    tsp.printAllParticles();
    tsp.getBestGlobalParticle()->getPersonalBestPath().print();

    return 0;
}