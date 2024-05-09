#include <iostream>
#include <vector>

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
        newCity.vertex = i + 1;
        newCity.position = std::pair(x, y);

        cities.push_back(newCity);
    }

    return cities;
}

int main() {
    vector<City> cities = getAllCities();
    PSO tsp = PSO(generateCompleteGraph(cities), 12, 10);
    tsp.printAllParticles();
    tsp.getBestGlobalParticle()->personalBestPath.print();

    return 0;
}