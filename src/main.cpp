#include <iostream>
#include <vector>

#include "../include/plot.hpp"
#include "../include/pso.hpp"
#include "../include/tsp.hpp"

using namespace std;

vector<City> getAllCities() {
    vector<City> cities;
    int n;
    Coordinate x, y;

    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> x >> y;

        City newCity;
        newCity.vertex = i;
        newCity.position = std::pair(x, y);

        cities.push_back(newCity);
    }

    return cities;
}

int main() {
    vector<City> cities = getAllCities();
    PSO tsp = PSO(generateCompleteGraph(cities), 100, 15);
    tsp.getBestGlobalParticle()->personalBestPath.print();
    plotGraph(cities, tsp.getBestGlobalParticle()->personalBestPath);

    return 0;
}