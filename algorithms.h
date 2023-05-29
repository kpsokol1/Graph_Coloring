// Author: Kyle Sokol
// Class: CS 495
// NP Hard Project

#pragma once
#ifndef GRAPH_COLORING_ALGORITHMS_H
#define GRAPH_COLORING_ALGORITHMS_H
#include <vector>
#include <set>
#include <unordered_map>
#include <iostream>
using namespace std;

bool isValid(int colors[]);

bool bruteForceRecursive(vector<int> colors, int index);
void bruteForce(unordered_map<int,set<int>> adjacenyList);

bool isSameColorAdjacent(vector<int> colors, int index, int color);

bool backTrackingRecursive(vector<int> colors, int index);

void backTracking(unordered_map<int,set<int>> adjacenyList);

void heuristic1 (unordered_map<int,set<int>> adjacenyList);

#endif //GRAPH_COLORING_ALGORITHMS_H
