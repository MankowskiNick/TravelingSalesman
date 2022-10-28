#ifndef CITY_H
#define CITY_H

#include <iostream>
#include <assert.h>

class City {
    public:

        City(int initial_id, std::pair<double, double>& initial_coords) {
            id = initial_id;
            coords = std::make_pair<double, double> (initial_coords.first, initial_coords.second);
        }
        
        City() {
            std::cout << "ERROR: Object 'City' declared without an id and/or coordinates.\n";
            assert(false);
        }
        
        int Id() {
            return id;
        }

        std::pair<double, double> Coords() {
            return std::make_pair(coords.first, coords.second);
        }

    private:
        int id;
        std::pair<double, double> coords;
};

#endif