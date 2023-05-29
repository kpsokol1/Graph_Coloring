// Author: Kyle Sokol
// Class: CS 495
// NP Hard Project

#include <iostream>
#include <vector>
#include <unordered_map>
#include "parser.h"
#include "algorithms.h"

using namespace std;

int main(int argc, char* argv[]) {
    if(argc < 2){                                                                           //check to make sure the user provided the filename as an argument
        cout << "Please provide a file name" << endl;
        return 1;
    }

    string fileName = argv[1];                                                              //name or path of the graph file
    unordered_map<int,set<int>> adjacenyList = parseGraphFile(fileName);                    //read the graph file into a map (key: node, value: list of edges)

    cout << "Running Heuristic..." << endl;
    heuristic1(adjacenyList);                                                               //run heuristic
    cout << "Running Brute-Force... " << endl;
    backTracking(adjacenyList);                                                             //run brute-force
    return 0;
}
