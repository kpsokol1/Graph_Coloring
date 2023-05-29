// Author: Kyle Sokol
// Class: CS 495
// NP Hard Project

#include "algorithms.h"
#include <algorithm>
#include <map>

int maxColors;              //the max number of colors we are allowed to use on this iteration
int numNodes;               //number of nodes in the graph

unordered_map<int,set<int>> adjList;        //adjacency list of the graph
vector<int> globalColors;                   //keeps track of each node and its color (can be uncolored 0)


//print the results
void printGraph(vector<int> colors, bool addOffset, int chromaticNumber){

    //print out the node numbers
    cout << "Nodes:     ";
    for(int i = 1; i <= adjList.size(); i++){
        cout << i << " ";
    }
    cout << endl;

    //list the corresponding color for each node
    cout << "Colors:    ";
    if(addOffset){
        for(int i = 0; i < colors.size(); i++){         //handle off by one error
            cout << colors.at(i) + 1 << " ";
        }
    }
    else{
        for(int i = 0; i < colors.size(); i++){
            cout << colors.at(i)<< " ";
        }
    }

    cout << endl;

    //display the min color
    cout << "Chromatic Number: " << chromaticNumber << endl;

    //output the maximum independent set
    cout << "Maximum Independent Set:" << endl;

    vector<int> tempColors = colors;
    sort(tempColors.begin(),tempColors.end());      //sort the colors
    int runningCount = 0;                           //running count of number of nodes with the current color
    int maxCount = 0;                               //maximum nodes with the same color
    int maxColor = 0;                               //color with the max count
    int currentColor = 1;                           //current color we are looking out

    //iterate through the colors
    for(int i = 0; i < tempColors.size(); i++){
        if(tempColors.at(i) != currentColor){       //we have reached a new color
            if(runningCount >= maxCount){           //check if we have a new max color
                maxCount = runningCount;
                maxColor = currentColor;
            }
            currentColor = tempColors.at(i);        //set the new color
            runningCount = 0;                       //reset the count
        }
        runningCount++;                             //increment the count
    }
    if(runningCount >= maxCount){                   //check if last color was he max
        maxCount = runningCount;
        maxColor = currentColor;
    }
    cout << "Size: " << maxCount << endl;           //output the max independent set size

    cout << "Nodes: ";                              //list the nodes in the independent set
    for(int i = 0; i < colors.size(); i++){
        if(colors.at(i) == maxColor){
            cout << i + 1 << " ";
        }
    }
    cout << endl;
    cout << endl;
}

//checks to see if graph has a valid coloring
bool isValid(vector <int> colors){
    unordered_map<int,set<int>>:: iterator it;
    int node1Color;
    int node2Color;
    int node1Number;
    int node2Number;
    set<int> edges;
    for(it = adjList.begin(); it!= adjList.end(); it++){        //iterate over the whole graph/adjacency list
        //get node 1 info
        node1Number = it->first;
        edges = it->second;
        node1Color = colors.at(node1Number-1);

        //check all of node1's edges to make sure the same color isn't adjacent
        for(auto itr : edges){
            node2Number = itr;
            node2Color = colors.at(node2Number-1);
            if(node1Color == node2Color){       //same color found
                return false;
            }
        }
    }
    return true;        //all nodes check out (different colors)
}

//recursively tries all combinations of colors to see if one works
bool bruteForceRecursive(vector<int> colors, int index){
    //see if we are at the last node to be colored
    if(index == numNodes-1){
        //check if this is a valid coloring
        if(isValid(colors)){
            globalColors = colors;
            return true;
        }
        return false;
    }

    for(int i = 0; i < maxColors; i++){             //try all color combinations recursively
        colors.at(index) = i;                       //give node a color
        if(bruteForceRecursive(colors, index+1)) {  //recursively try all colors on all other nodes past that
            return true;        //end recursion
        }
    }
    return false;       //no coloring found
}

//Brute Force 1: (Naive approach) (trys all possible combinations)
void bruteForce(unordered_map<int,set<int>> adjacenyList){
    //initialize variables
    numNodes = adjacenyList.size();
    adjList = adjacenyList;
    int chromaticNumber = numNodes+1;
    maxColors = adjacenyList.size();
    vector<int> minColors;

    for(int i = adjacenyList.size()-1; i >= 1; i--){        //try largest number of colors first then reduce the number of colors available
        vector<int> colors(numNodes);
        if (bruteForceRecursive(colors, 0)){          //see if there is a combination which works
            chromaticNumber--;                              //reduce number of colors available
            maxColors--;
            minColors = globalColors;
        }
        else{
            break;
        }
    }

    cout << "Brute Force: " << endl;
    printGraph(minColors, true, chromaticNumber);       //print results

}

//check that colors on the same edge don't have the same color
bool isSameColorAdjacent(vector<int> colors, int index, int color){
    //initialize variables
    int node1Number = index + 1;
    int node1Color = color;
    int node2Number;
    int node2Color;
    set<int> edges = adjList.at(node1Number);
    for(auto itr : edges){
        node2Number = itr;
        node2Color = colors.at(node2Number-1);
        if(node1Color == node2Color){
            return true;
        }
    }
    return false;
}



bool backTrackingRecursive(vector<int> colors, int index){

    //we have colored all the nodes because we have traversed beyond the last node (based on 0-based indexing)
    if(index == numNodes){
        globalColors = colors;
        return true;
    }

    for(int i = 1; i <= maxColors; i++){                            //try each color per vertex
        if(!isSameColorAdjacent(colors,index,i)){                   //check safety
            colors[index] = i;                                      //set the color

            if(backTrackingRecursive(colors,index+1)){        //we are done
                return true;
            }
            else{ //parent color was bad so backtrack
                colors[index] = 0;
            }
        }
    }
    return false;   //could not assign a color to the vertex so backtrack
}

//Brute Force 2: (Better approach, same worst case but much better average case)
void backTracking(unordered_map<int,set<int>> adjacenyList){
    //initialize variables
    numNodes = adjacenyList.size();
    adjList = adjacenyList;
    int chromaticNumber = numNodes+1;
    maxColors = adjacenyList.size();
    vector<int> minColors;

    //start with max colors and try to make smaller every time
    for(int i = adjacenyList.size(); i >= 1; i--){
        vector<int> colors(numNodes);
        if (backTrackingRecursive(colors, 0)){      //check if current combination of colors valid
            chromaticNumber--;                          //try on a smaller amount of colors
            maxColors--;
            minColors = globalColors;
        }
        else{
            break;  //need more colors
        }
    }
    cout << "Brute Force: " << endl;
    printGraph(minColors, false, chromaticNumber);      //print the results
}

//returns the next node to color and what color it should be
int getNextNodeToColor(unordered_map<int,set<int>> adjacenyList){
    //find most saturated node
    unordered_map<int,set<int>>:: iterator it;
    int node1Number;
    int node2Number;
    int node2Color;
    set<int> edges;

    //node number: <saturation number, number of uncolored neighbors>
    map<int, pair<int,int>> colors_used;

    //calculate saturation
    for(it = adjacenyList.begin(); it!= adjacenyList.end(); it++){          //calculate saturation of nodes
        //initialize variables
        int runningUncoloredNeighbors = 0;
        set<int>neighborColors;
        node1Number = it->first;
        edges = it->second;
        if(globalColors.at(node1Number-1) == 0){                  //only find saturation for nodes we haven't colored yet
            for(auto itr : edges){                                      //iterate over all of the node's edges
                node2Number = itr;
                node2Color = globalColors.at(node2Number-1);
                if(node2Color == 0){                                    //node is uncolored
                    runningUncoloredNeighbors++;                        //increment count of uncolored neighbors
                }
                else{
                    neighborColors.insert(node2Color);                  //insert into list of neighbor colors
                }
            }
            colors_used.insert({node1Number,make_pair(neighborColors.size(),runningUncoloredNeighbors)});       //add to list of potential candidates for most saturated
        }
    }
    //find the max saturation of all candidates
    int maxSaturation = 0;
    int maxSaturationNodeNumber = 0;
    int numTiedNodes = 0;

    //determine the highest saturation and resolve any ties
    map<int, pair<int,int>> :: iterator color_iterator;
    for(color_iterator = colors_used.begin(); color_iterator!= colors_used.end(); color_iterator++){        //iterate through the candidates
        int nodeNumber = color_iterator->first;
        int saturation = (color_iterator->second).first;
        if(saturation == maxSaturation){            //tie for max saturation
            numTiedNodes++;
        }
        else if(saturation > maxSaturation){        //new max saturation
            maxSaturation = saturation;
            maxSaturationNodeNumber = nodeNumber;
            numTiedNodes = 0;                       //reset the number of ties
        }
    }

    //there was a tie for saturation (find highest saturation with uncolored node)
    if(numTiedNodes != 0){
        int maxUncoloredCount = 0;
        int maxUncoloredNodeNumber = 0;
        int nodeNumber = 0;
        map<int, pair<int,int>> :: iterator color_iterator_2;

        for(color_iterator_2 = colors_used.begin(); color_iterator_2!= colors_used.end(); color_iterator_2++){  //iterate over all candiates
            nodeNumber = color_iterator_2->first;
            int saturation = color_iterator_2->second.first;
            int uncoloredCount = color_iterator_2->second.second;
            if(saturation == maxSaturation){                //look at nodes that are tied
                if(uncoloredCount > maxUncoloredCount){     //new max uncolored neighbors count
                    maxUncoloredCount = uncoloredCount;
                    maxUncoloredNodeNumber = nodeNumber;
                }
            }
        }
        if(maxUncoloredNodeNumber == 0){
            return nodeNumber;  //just return something if both tied at 0
        }
        return maxUncoloredNodeNumber;
    }
    else{
        return maxSaturationNodeNumber;
    }
}
int getLowestColor(unordered_map<int,set<int>> adjacenyList, int nodeToColor){
    //initialize variables
    set<int> edges = adjacenyList.at(nodeToColor);
    set<int> nodeToColorNeighborColors;     //list of colors of all neighbors (sorted)

    //get the colors of all neighbors
    for(auto itr : edges) {
     nodeToColorNeighborColors.insert(globalColors.at(itr-1));
    }
    //try lowest color first and icnrease until we find a color that works
    int color = 1;
    while(true){
        if(nodeToColorNeighborColors.find(color) == nodeToColorNeighborColors.end()){           //check and see if the current color is not in the set
            return color;
        }
        color++;
    }
}
bool heuristic1Helper(unordered_map<int,set<int>> adjacenyList){
    //initialize variables
    vector<int> nodeColors(adjacenyList.size(),0);          //0 is default value and is not colored
    int coloredCount = 0;
    globalColors = nodeColors;
    int mostConstrainedNodeNumber;
    int lowestColor;

    while(coloredCount < adjacenyList.size()){         //continue until all nodes are covered
        mostConstrainedNodeNumber = getNextNodeToColor(adjacenyList);   //find the most constrained node that hasn't been colored yet
        lowestColor = getLowestColor(adjacenyList,mostConstrainedNodeNumber);   //give it the lowest color available
        if(lowestColor > maxColors){        //check if the coloring is possible
            return false;
        }
        //add color to the list
        globalColors.at(mostConstrainedNodeNumber-1) = lowestColor;
        coloredCount++;
    }

    //coloring found
    return true;
}

//Heuristic: greedy coloring on the most constrained nodes
void heuristic1 (unordered_map<int,set<int>> adjacenyList){
    //initialize variables
    maxColors = adjacenyList.size();
    adjList = adjacenyList;
    int chromaticNumber = adjacenyList.size() + 1;
    vector<int> minColors;


    for(int i = maxColors; i >= 1; i--){                    //optimize to find the minimum color (reduce available colors by 1 each time)
        if(heuristic1Helper(adjacenyList)){                 //heurisitc found a solution, try again with less colors
            chromaticNumber--;
            maxColors--;
            minColors = globalColors;
        }
        else{
            break;      //no solution found
        }
    }

    //print results (guranteed to have 1 since we can use a unqiue color for each node)
    cout << "Heuristic: " << endl;
    printGraph(minColors, false, chromaticNumber);
};


