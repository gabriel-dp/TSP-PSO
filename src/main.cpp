#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>

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
        newCity.position = pair(x, y);

        cities.push_back(newCity);
    }

    return cities;
}
    

void saveResults(const string &filename, const vector<tuple<int, int, double, double, double, double>>& results) {
    ofstream outFile(filename);
    outFile << "PopulationSize,Iterations,c1,c2,BestResult,TimeElapsed\n";
    for (const auto& result : results)
        outFile << get<0>(result) << "," 
                << get<1>(result) << "," 
                << get<2>(result) << "," 
                << get<3>(result) << "," 
                << get<4>(result) << ","
                << get<5>(result) << "\n";
    outFile.close();
}

int main() {
    vector<City> cities = getAllCities();
    vector<tuple<int, int, double, double, double, double>> results;

    //gridSearch parameters
    vector<int> populationSizes = {10,20,40,60,80};
    vector<int> iterationsList = {10,20,40,60,80};
    vector<double> c1List = {0.5,0.8,1.5};
    vector<double> c2List = {0.5,0.8,1.5};
    int i = 0;
    for (int populationSize : populationSizes) {
        cout << "iteration: " << i << endl;
        i++;
        for (int iterations : iterationsList) {
            for (double c1 : c1List) {
                for (double c2 : c2List) {

                    auto start = chrono::high_resolution_clock::now();

                    PSO tsp = PSO(generateCompleteGraph(cities), populationSize, iterations, c1, c2);
                    double result = tsp.getBestGlobalParticle()->personalBestPath.getCost();

                    auto end = chrono::high_resolution_clock::now();
                    chrono::duration<double> elapsed = end - start;
                    results.emplace_back(populationSize, iterations, c1, c2, result, elapsed.count());
                }
            }
        }
    }
    saveResults("grid_search_results.csv", results);

    return 0;
}