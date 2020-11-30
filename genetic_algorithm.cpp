//
// Created by kimer on 2020-11-24.
//

#include "genetic_algorithm.hpp"

/*
 * genetic_algorithm constructor
 * runs the genetic algorithm to minimize the distance travelled for the travelling
 * salesman problem.
 * @returns genetic_algorithm : newly created genetic_algorithm
 */
genetic_algorithm::genetic_algorithm() {
    tour tour_list;
    vector<tour> population{POPULATION_SIZE};
    vector<tour> cross{POPULATION_SIZE - NUMBER_OF_ELITES};
    double base_distance;
    double best_distance = 0.0;
    int num_of_iterations = 0;

    srand(static_cast<unsigned int>(time(nullptr)));

    //Create group of cities for each city in the tour, from char A, x btwn 0 and 1000, y btwn 0 and 1000
    for (unsigned int i = 0; i < CITIES_IN_TOUR; ++i) {
        city temp{(char) (i + 65),
                //x value
                  rand() % (MAP_BOUNDARY + 1),
                //y value
                  rand() % (MAP_BOUNDARY + 1)
        };
        tour_list.cities[i] = temp;
    }

    //Creating population of tours
    for (unsigned int i = 0; i < POPULATION_SIZE; ++i) {
        for (unsigned int j = 0; j < CITIES_IN_TOUR; ++j)
            population[i].cities[j] = tour_list.cities[j];
        shuffle_cities(population[i]);
        population[i].fitness = 0.0;
    }

    //Determine and record fitness of each tour
    int best_tour_index = selection(population, static_cast<int>(POPULATION_SIZE));

    //Shortest, fittest, tour, starting disttance or base_distance
    base_distance = KM_SCALE / population[best_tour_index].fitness;

    cout << "Base Distance: " << fixed << setprecision(3) << base_distance << endl;
    cout << "==========================================" << endl;

    //iterate while predetermined improvement, base / best > improvement_factor
    int iterations = 0;
    while (iterations++ < ITERATIONS && base_distance/ best_distance >  IMPROVEMENT_FACTOR) {
        if (best_tour_index != 0) {
            const tour temp_tour = population[0];
            population[0] = population[best_tour_index];
            population[best_tour_index] = temp_tour;
        }

        for (unsigned int i = 0; i < (POPULATION_SIZE - NUMBER_OF_ELITES); ++i) {
            vector<tour> parents = select_parents(population);
            cross[i] = crossover(parents);
        }

        for (unsigned int i = NUMBER_OF_ELITES; i < POPULATION_SIZE; ++i) {
            population[i] = cross[i - NUMBER_OF_ELITES];
            population[i].fitness = 0.0;
        }

        mutate(population);

        best_tour_index = selection(population, static_cast<int>(POPULATION_SIZE));
        best_distance = get_tour_distance(population[best_tour_index]);

        if (best_distance < base_distance) {
            base_distance = best_distance;
            report(population[best_tour_index].cities);
            num_of_iterations++;

            cout << ", Distance of tour: " << fixed << setprecision(3) << best_distance;
            cout << endl;
        }
    }
    cout << "==========================================" << endl;
    cout << "Number of displayed tour iterations: " << num_of_iterations << endl;
    cout << "Best Distance: " << KM_SCALE / population[best_tour_index].fitness;
}



/*
 * Swaps cities in a tour
 * @params int first : index of the first city
 *         int second : index of the second city
 *         vector<city> &cities : vector list of cities
 * @returns void
 */
void genetic_algorithm::swap_cities(int first, int second, vector<city> &cities) {
    city temp = cities[first];
    cities[first] = cities[second];
    cities[second] = temp;
}

/*
 * Shuffles the cities in a tour, random amount of times
 * @params tour &t : tour with cities to be shuffled
 * @returns void
 */
void genetic_algorithm::shuffle_cities(tour &t) {
    shuffle(begin(t.cities), end(t.cities), std::mt19937(std::random_device()()));
}

/*
 * Fitness of each tour in a population, Moves the fittest to the front
 * @params vector<tour> &p
 *         int num_of_tours_pop
 * @returns index of shortest tour
 */
int genetic_algorithm::selection(vector<tour> &p, int num_of_tours_pop) {
    int index_of_shortest_tour = 0;
    auto shortest_tour_in_population = (double) RAND_MAX;
    double candidate_distance = 0.0;

    for (int i = 0; i < num_of_tours_pop; ++i) {
        candidate_distance = get_tour_distance(p[i]);
        p[i].fitness = KM_SCALE / candidate_distance;
        if (candidate_distance <= shortest_tour_in_population) {
            index_of_shortest_tour = i;
            shortest_tour_in_population = candidate_distance;
        }
    }
    return index_of_shortest_tour;
}

/*
 * Total distance travelled in a tour
 * @params tour &t
 * @returns total distance travelled in a tour
 */
double genetic_algorithm::get_tour_distance(tour &t) {
    double distance = 0.0;
    for (unsigned int i = 0; i < CITIES_IN_TOUR; ++i)
        distance += get_city_distance(t.cities[i], t.cities[(i + 1) % CITIES_IN_TOUR]);
    return distance;
}

/*
 * Distance between two cities
 * @params city first : first city (current city)
 *         city second : second city (next city to travel to)
 * @returns the distance between the two cities
 */
double genetic_algorithm::get_city_distance(city first, city second) {
    return sqrt(pow((double) (first.x - second.y), 2.0)+ pow((double) (first.y - second.y), 2.0)
    );
}

/*
 * Selects parent tours which are the fittest of the population p
 * @params vector<tour> &p
 * @returns returns fittest of the population p
 */
vector<tour> genetic_algorithm::select_parents(vector<tour> &p) {
    int index_of_parent = 0;
    vector<tour> parents{NUMBER_OF_PARENTS};
    vector<tour> parent_pool{PARENT_POOL_SIZE};

    for (unsigned int i = 0; i < NUMBER_OF_PARENTS; ++i) {
        for (unsigned int j = 0; j < PARENT_POOL_SIZE; ++j) {
            int k = static_cast<int>(rand() % POPULATION_SIZE);
            parent_pool[j] = p[k];
        }
        index_of_parent = selection(parent_pool, static_cast<int>(PARENT_POOL_SIZE));
        parents[i] = parent_pool[index_of_parent];
    }
    return parents;
}

/*
 * Crosses two parents, selects random index and uses the cities from one parent to populate
 * the mixed tour up to and including that index, and then the cities from the second parent to
 * top up the tour, making sure we don't add cities already in the mixed tour. If city in a following
 * parent has already been added to the tour, simply moves to the next city in the tour
 * @params char name : name of the city
 *         int x : x coordinate of the city
 *         int y : y coordinate of the city
 * @returns mixed child tour
 */
tour genetic_algorithm::crossover(vector<tour> &parents) {
    tour child_tour;

    unsigned int index_of_boundary = rand() % CITIES_IN_TOUR;

    for (unsigned int i = 0; i < index_of_boundary; ++i) {
        child_tour.cities[i] = parents[0].cities[i];
    }

    while (index_of_boundary < CITIES_IN_TOUR) {
        for (unsigned int i = 0; i < CITIES_IN_TOUR; ++i) {
            if (!contains_city(child_tour, index_of_boundary, parents[1].cities[i])) {
                child_tour.cities[index_of_boundary] = parents[1].cities[i];
                index_of_boundary++;
            }
        }
    }
    return child_tour;
}

/*
 * Mutates random tours. Creates a random mutation value for each city.
 * If this value is is less than the MUTATION_RATE, the city is swapped with a random
 * adjacent city from the same tour.
 * @params vector<tour> &p
 * @returns void
 */
void genetic_algorithm::mutate(vector<tour> &p) {
    for (unsigned int i = 0 + NUMBER_OF_ELITES; i < POPULATION_SIZE; ++i) {
        for (unsigned int j = 0; j < CITIES_IN_TOUR; ++j) {
            if ((double) rand() / (double) RAND_MAX < MUTATION_RATE) {
                swap_cities(j, rand() % CITIES_IN_TOUR, p[i].cities);
            }
        }
    }
}

/*
 * Checks if the candidate city is in the candidate tour
 * @params tour &candidate_tour
 *         int tour_length
 *         city &candidate_city
 * @returns true if the candidate_city is in the candidate tour, else false.
 */
bool genetic_algorithm::contains_city(tour &candidate_tour, int tour_length, city &candidate_city) {
    for (int i = 0; i < tour_length; ++i) {
        if (candidate_tour.cities[i].name == candidate_city.name &&
            candidate_tour.cities[i].x == candidate_city.x &&
            candidate_tour.cities[i].y == candidate_city.y) {
            return true;
        }
    }
    return false;
}

/*
 * Report for the tour of cities
 * @params vector<city> &cities
 * @returns void
 */
void genetic_algorithm::report(vector<city> &cities) {
    for (auto c : cities) cout << c.name << " ";
}