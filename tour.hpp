//
// Created by kimer on 2020-11-24.
//

#ifndef ASSIGNMENT2_TOUR_HPP
#define ASSIGNMENT2_TOUR_HPP

#include <vector>
#include "city.hpp"

using namespace std;

class tour {
public:
    const int CITIES_IN_TOUR = 20;

    vector<city> cities;
    double fitness;

    tour();

    tour &operator=(tour t);
};



#endif //ASSIGNMENT2_TOUR_HPP
