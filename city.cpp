//
// Created by kimer on 2020-11-24.
//
#include "city.hpp"

/*
 * city constructor
 * @params char name : name of the city
 *         int x : x coordinate of the city
 *         int y : y coordinate of the city
 * @returns city : newly created city
 */
city::city(char name, int x, int y)
        : name(name),
          x(x),
          y(y) {}
