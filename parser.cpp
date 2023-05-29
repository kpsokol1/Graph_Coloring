// Author: Kyle Sokol
// Class: CS 495
// NP Hard Project

#include "parser.h"

//adds the current edge/line to the adjaceny list
void addToAdjacencyList(string line){
    //get node number
    int node1Number;                    //node 1 number
    int node2Number;                    //node 2 number
    int weight;                         //edge weight (should always be 1 for undriected)

    istringstream iss(line);
    iss >> node1Number;                 //read in node 1 number
    iss >> node2Number;                 //read in node 2 number
    iss >> weight;                      //read in weight

    //check if node1 is already in the adjaceny list
    if(nodeList.count(node1Number) == 0){
        set<int> edges;
        nodeList.insert({node1Number,edges});            //create empty set if doesn't exist
    }
    nodeList.at(node1Number).insert(node2Number);        //insert into existing set
}

//convert the adjaceny-list
void convertToSAT(int numNodes){
    cout << "3 SAT Representation:" << endl;
    set<int> edges;
    set<int>::iterator it;
    vector<pair<int,int>>edges_seen;            //list of edge pairs that have been seen
    bool seenEdgeBefore = false;                //if seen edge before don't need to add another term

    //iterate through the adjaceny list
    for(int i = 1; i <= numNodes; i++){
        edges = nodeList.at(i);                                 //get the egdes for the node i
        for(it = edges.begin(); it != edges.end(); it++){       //iterate through the edges
            seenEdgeBefore = false;
            for(int k = 0; k < edges_seen.size(); k++){         //don't include existing edges
                if(edges_seen.at(k) == make_pair(i,*it) || edges_seen.at(k) == make_pair(*it,i)){   //check if edge has been seen before
                    seenEdgeBefore = true;
                    break;
                }
            }
            if(!seenEdgeBefore){ //new edge
                //add new edge
                edges_seen.push_back(make_pair(i,(*it)));       //add new edge to the seen list
                for(int j = 0; j < 3; j++){                     //produce output so that the node has a different color than its neighbors
                    cout << -(i*3-2+j) << " " << -((*it)*3-2+j) << " " << -((*it)*3-2+j) << endl;
                }
            }
        }
        cout << i*3-2 << " " << i*3-2+1 << " " << i*3-2+2 << endl;      //produce output so that nodes have to be red, green, or blue
    }
    cout << endl;
}

//parses the graph file and turns it into a adjaceny list
unordered_map<int,set<int>> parseGraphFile(string fileName){
    //open file
    ifstream file(fileName);
    if(!file.good()){  //check if file exists
        cout << "Error opening file or file doesn't exist" << endl;
        exit(-1);
    }

    //parse file line by line
    int currentLine = 1;     //keep track of the line we are on
    string line;             //temp variable to hold the current line
    int numberOfNodes = 0;   //number of nodes in the graph we are reading in

    while(getline(file,line) && line != "$"){      //read line by line until eof of $
        if(currentLine == 1){                              //ignore the first line since it is just the name of the problem (e.g. graph coloring)
            currentLine++;
            continue;
        }
        else if(currentLine == 2){
            numberOfNodes = stoi(line);                   //capture the number of nodes there are in the graph
        }
        else{
            addToAdjacencyList(line);                     //add the current line/edge to the adjaceny list
        }
    currentLine++;                                        //increment the current line we are examining
    }
    convertToSAT(numberOfNodes);                          //convert the graph to a 3-SAT representation
    return nodeList;                                      //return the adjaceny list
}