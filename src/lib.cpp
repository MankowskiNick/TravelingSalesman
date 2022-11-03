
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>

#include "City.h"

#define TIME_MAX 60
#define START_TEMP_SCALAR 1000
#define COOLING_RATE 0.999
#define TEMP_MIN 1
#define PERCENT_CHANGES 0.2
#define MAX_NUM_CHANGES 200

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

template<typename T>
int RemoveFromVector(std::vector<T>& vect, T remove_item) {
    int item_index = -1;
    int list_size = vect.size();
    for (int i = 0; i < list_size; i++) {
        if (vect[i] == remove_item && item_index == -1) {
            item_index = i;
            list_size--;
            for (int j = i; j < vect.size(); j++) {
                vect[j] = vect[j + 1];
            }
            vect.resize(list_size);
        }
    }
    return item_index;
}

template<typename T>
int GetMinIndex(std::vector<T>& vect) {
    int index = 0;
    T min_result = vect[0];

    for (int i = 0; i < vect.size(); i++) {
        if (vect[i] < min_result) {
            index = i;
            min_result = vect[i];
        }
    }
    return index;
}

template<typename T>
bool InsertAtPosition(std::vector<T>& vect, T& add_item, int index) {
    vect.resize(vect.size() + 1);
    if (index >= vect.size() || index < 0) throw std::out_of_range ("Out of Range Exception: Insert index is out of range of vector.");
    for (int i = vect.size() - 2; i >= index; i--) {
        vect[i + 1] = vect[i];
    }
    vect[index] = add_item;
    return true;
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

void SpliceRandomCity(std::vector<City>& city_list) {
    City rand_city;
    int remove_index = rand() % (city_list.size() - 2);
    rand_city = city_list[remove_index];
    RemoveFromVector<City>(city_list, rand_city);
    int constraint = city_list.size() - remove_index + 1;
    int random = rand() % constraint;

    int new_index = random + remove_index;
    InsertAtPosition(city_list, rand_city, new_index);
    return;
}

double GetDistBetween(std::pair<double, double> p1, std::pair<double, double> p2) {
    double x0, y0, x1, y1;
    
    x0 = p1.first;
    y0 = p1.second;
    
    x1 = p2.first;
    y1 = p2.second;
    return sqrt( pow(x1 - x0, 2) + pow(y1 - y0, 2) );
}

double GetGreedySolution(std::vector<City>& city_list, int start_node = 0) {
    std::vector<City> ref_list(city_list);
    city_list = std::vector<City>();
    city_list.push_back(ref_list[start_node]);
    RemoveFromVector<City>(ref_list, ref_list[0]);

    double result = 0.00f;

    //int start_city = 0;
    //int cur_city = start_city;
    int num_iterations = ref_list.size();
    for (int i = 0; i < num_iterations; i++) {
        std::vector<double> possible_dist;
        std::vector<int> possible_index;
        // Get the cheapest move
        for (int j = 0; j < ref_list.size(); j++) {
            if (city_list[city_list.size() - 1].Id() != ref_list[j].Id()) {
                possible_dist.push_back(GetDistBetween(city_list[city_list.size() - 1].Coords(), ref_list[j].Coords()));
                possible_index.push_back(j);
            }
        }
        int min_index = GetMinIndex<double>(possible_dist);
        int city_index = possible_index[min_index];
        double cur_result = possible_dist[min_index];

        city_list.push_back(ref_list[city_index]);
        RemoveFromVector<City>(ref_list, ref_list[city_index]);

        result += cur_result;
    }
    result += GetDistBetween(city_list[city_list.size() - 1].Coords(), city_list[0].Coords());

    return result;

}

double SumPoints(std::vector<City>& city_list) {
    double result = 0.0f;
    for (int i = 0; i < city_list.size(); i++) {
       result += GetDistBetween(city_list[i].Coords(), city_list[i + 1].Coords());
    }
    result += GetDistBetween(city_list[city_list.size() - 1].Coords(), city_list[0].Coords());
    
    return result;
}

double Anneal(std::vector<City>& city_list, int cur_num_changes) {

    //if (city_list.size() == 8) std::random_shuffle(city_list.begin(), city_list.end());  // maybe remove soon
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
        // Relocate the specified number of cities in the order(in the vector, obviously)
        for (int i = 0; i < cur_num_changes; i++) {
            SpliceRandomCity(annealing_list);
        }

        double check = SumPoints(annealing_list);

        double take_probability = exp(-1 * abs(check - current_best) / (temp + 1));
        double random = double(rand()) / double(RAND_MAX);
        if (check < current_best || (random < take_probability && abs(check - current_best) > 0.01 )) {
            current_best = check;
            city_list = std::vector<City>(annealing_list);
        }
        temp = initial_temp / double(iteration);
        //temp *= COOLING_RATE;
        iteration++;

    }
    return current_best;
}

double GetTour(std::vector<City>& city_list) {

    std::vector<double> result_costs;
    std::vector< std::vector<City> >  result_tours;
    if (city_list.size() == 8) { // annoying hard coding in n=8 because the heuristic sucks for it
        for (int i = 0; i < city_list.size(); i++) {     
            double cur_result;
            std::vector<City> cur_tour(city_list);
            cur_result = Anneal(cur_tour, i);

            result_tours.push_back(cur_tour);
            result_costs.push_back(cur_result);
        }

        int result_index = GetMinIndex<double>(result_costs);
        city_list = result_tours[result_index];
        return result_costs[result_index];
    }

    // Get a greedy solution
    double initial_result = GetGreedySolution(city_list);

    result_costs.push_back(initial_result);
    result_tours.push_back(city_list);

    // Get pre-annealing time
    time_t pre_time = time(NULL);

    int max_num_changes = int(double(city_list.size()) * PERCENT_CHANGES);
    if (max_num_changes < 1) max_num_changes = 1;
    for (int i = 1; i <= max_num_changes && time(NULL) - pre_time < TIME_MAX; i++) {

        double cur_result;
        std::vector<City> cur_tour(city_list);
        cur_result = Anneal(cur_tour, i);

        result_tours.push_back(cur_tour);
        result_costs.push_back(cur_result);
    }
    

    int result_index = GetMinIndex<double>(result_costs);
    city_list = result_tours[result_index];
    return result_costs[result_index];
}
