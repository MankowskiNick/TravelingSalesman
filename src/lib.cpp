
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>

#include "City.h"

#define TIME_MAX 600
#define START_TEMP_SCALAR 1000
#define COOLING_RATE 0.999
#define TEMP_MIN 1
#define PERCENT_CHANGES 0.1

int InputMapper(std::ifstream& fin, std::vector<City>& city_list) {
    
    // Get the number of cities
    int num_cities;
    fin >> num_cities;

    // Empty the list of cities(just in case)
    city_list = std::vector<City>();

    // Set up variables we need for looping through

    for (int i = num_cities - 1; i >= 0; i--) {
        double x, y;

        fin >> x;
        fin >> y;

        std::pair<double, double> coords = std::make_pair (x, y);
        city_list.push_back(City(i, coords));
    }
    return num_cities;
}

template<typename T>
void Swap(T& obj1, T& obj2) {
    T temp = obj1;
    obj1 = obj2;
    obj2 = temp;
}

// Swap two random elements in the list of nodes, our "random" change in simulated annealing
void SwapRandomCities(std::vector<City>& city_list) {
    int swap_node1 = 0;
    int swap_node2 = 0;
    while (swap_node1 == swap_node2) {
        swap_node1 = rand() % city_list.size();
        swap_node2 = rand() % city_list.size();
    }
    Swap<City>(city_list[swap_node1], city_list[swap_node2]);
}

double SumPoints(std::vector<City>& city_list) {
    double result = 0.0f;
    for (int i = 0; i < city_list.size() - 1; i++) {
        double x0, y0, x1, y1;
        std::pair<double, double> first_coords = city_list[i].Coords();
        x0 = first_coords.first;
        y0 = first_coords.second;
        
        std::pair<double, double> next_coords = city_list[i + 1].Coords();
        x1 = next_coords.first;
        y1 = next_coords.second;

        result += sqrt( pow(x1 - x0, 2) + pow(y1 - y0, 2) );
    }
    double x0, y0, x1, y1;
    std::pair<double, double> first_coords = city_list[0].Coords();
    x0 = first_coords.first;
    y0 = first_coords.second;
        
    std::pair<double, double> next_coords = city_list[city_list.size() - 1].Coords();
    x1 = next_coords.first;
    y1 = next_coords.second;

    result += sqrt( pow(x1 - x0, 2) + pow(y1 - y0, 2) );
    
    return result;
}

double GetTour(std::vector<City>&  city_list) {
    //std::random_shuffle(city_list.begin(), city_list.end());
    double current_best = SumPoints(city_list);

    // Simulated Annealing
    // Initialize temperature
    double initial_temp = (START_TEMP_SCALAR * START_TEMP_SCALAR) / double(city_list.size());
    double temp = double(initial_temp);


    // Start simulated annealing
    int iteration = 1;
    while (temp > TEMP_MIN) {
        
        std::vector<City> annealing_list = std::vector<City>(city_list);

        // Make a random change

        // Reshuffle all of the possible_colors - maybe we could make a different change
        for (int i = 0; i < annealing_list.size() * PERCENT_CHANGES; i++) {
            SwapRandomCities(annealing_list);
        }

        double check = SumPoints(annealing_list);

        double take_probability = exp(-1 * abs(check - current_best) / (temp + 1));
        double random = double(rand()) / double(RAND_MAX);
        if (check < current_best || (random < take_probability && abs(check - current_best) > 0.1 )) {
            current_best = check;
            city_list = std::vector<City>(annealing_list);
        }
        //temp = initial_temp / double(iteration);
        temp *= COOLING_RATE;
        iteration++;

    }

    return current_best;
}