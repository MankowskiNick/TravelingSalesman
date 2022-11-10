#ifndef LIB_H
#define LIB_H

#include <vector>
#include <fstream>
#include "City.h"

int InputMapper(std::ifstream& fin, std::vector<City>& city_list);

double GetTour(std::vector<City>&  city_list);

#endif
