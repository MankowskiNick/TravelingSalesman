#ifndef CITY_H
#define CITY_H

#include <iostream>
#include <assert.h>

class City {
    public:
        City(int initial_id, std::pair<double, double> initial_coords) {
            id = initial_id;
            x = initial_coords.first;
            y = initial_coords.second;
        }
        
        City() { }
    
        ~City() { }

        int Id() {
            return id;
        }

        std::pair<double, double> Coords() {
            return std::make_pair(x, y);
        }

        bool operator==(City city2) {
            if (Id() == city2.Id()) return true;
            return false;
        }
        
    private:
        int id;
        double x, y;
};

#endif
