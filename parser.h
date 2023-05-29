// Author: Kyle Sokol
// Class: CS 495
// NP Hard Project

#ifndef GRAPH_COLORING_PARSER_H
#define GRAPH_COLORING_PARSER_H
#pragma once
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <set>
#include <iostream>
#include <vector>
using namespace std;
static unordered_map<int,set<int>> nodeList;     //fixme does this need to be extern instead of static

void addToAdjacencyList(string line);

unordered_map<int,set<int>> parseGraphFile(string fileName);

#endif //GRAPH_COLORING_PARSER_H
