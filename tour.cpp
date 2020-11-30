//
// Created by kimer on 2020-11-24.
//
#include "tour.hpp"

using namespace std;

/*
 * tour constructor, name and set of coordinates
 * @returns tour
 */
tour::tour() : fitness(0.0) {
    city temp{'a', 0, 0};
    for (int i = 0; i < CITIES_IN_TOUR; ++i) {
        cities.push_back(temp);
    }
}

/*
 * assignment operator
 * @params tour t
 * @returns returns tour& : *this (tour)
 */
tour &tour::operator=(tour t) {
    swap(cities, t.cities);
    swap(fitness, t.fitness);
    return *this;
}
