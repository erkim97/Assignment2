//
// Created by kimer on 2020-11-24.
//

#ifndef ASSIGNMENT2_GENETIC_ALGORITHM_HPP
#define ASSIGNMENT2_GENETIC_ALGORITHM_HPP
#include <random>
#include <iostream>
#include <iomanip>

#include <algorithm>
#include "city.hpp"
#include "tour.hpp"

#include <random>
#include <iostream>
#include <iomanip>

#include <algorithm>
#include "city.hpp"
#include "tour.hpp"

using namespace std;

class genetic_algorithm {

    unsigned int CITIES_IN_TOUR = 20;

    unsigned int POPULATION_SIZE = 32;

    int MAP_BOUNDARY = 1000;

    int ITERATIONS = 1000;

    unsigned int PARENT_POOL_SIZE = 5;

    double MUTATION_RATE = 0.20;

    unsigned int NUMBER_OF_PARENTS = 2;

    int NUMBER_OF_ELITES = 1;

    double KM_SCALE = 10000.0;

    double IMPROVEMENT_FACTOR = 0.1;

    void shuffle_cities(tour &t);

    double get_city_distance(city first, city second);

    int selection(vector<tour> &p, int num_of_tours_pop);

    double get_tour_distance(tour &t);

    vector<tour> select_parents(vector<tour> &p);

    tour crossover(vector<tour> &parents);

    bool contains_city(tour &candidate_tour, int tour_length, city &candidate_city);

    void mutate(vector<tour> &p);

    void swap_cities(int first, int second, vector<city> &cities);

    void report(vector<city> &cities);

public:
    genetic_algorithm();
};

#endif //ASSIGNMENT2_GENETIC_ALGORITHM_HPP
