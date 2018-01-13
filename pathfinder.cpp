#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <queue>
#include <cmath>

using namespace std;

//Node struct for use in the tree
struct Node{
	int first;
	int second;
	int elevation;
	int tentativeDist;
	double f;
	Node* previous;
	bool visited;
};

class PathFinder{	
	public:
		PathFinder(){};
		vector<vector <char> > map;
		vector<vector <double> > heuristicVal;
		Node start;
		Node end;
		pair<int, int> size;
		int pathCost;

//returns the neighbours of the givn node
		vector<Node*> findNodes(vector<Node* > nodes, int x, int y){
			vector<Node*> neighbours;
			for(int i = 0; i< nodes.size(); i++){
				if(nodes[i]->first == (x-1) && nodes[i]->second == y){
					//cout << "l--";
					neighbours.push_back(nodes[i]);
				}
				if(nodes[i]->first == (x+1) && nodes[i]->second == y){
					//cout << "r--";
					neighbours.push_back(nodes[i]);
				}
				if(nodes[i]->first == x && nodes[i]->second == (y+1)){
					//cout << "d--";
					neighbours.push_back(nodes[i]);
				}
				if(nodes[i]->first == x && nodes[i]->second == (y-1)){
					//cout << "u--";
					neighbours.push_back(nodes[i]);
				}
				
				
			}
			//cout << endl;
			return neighbours;
		}

//Finds the currently unvisited node with the smallest tentative distance
//For use in A* and Uniform Cost Search
		int findSmallest(vector<Node* > nodes, bool heuristic){
			int index = 0;
			if(!heuristic){
				for (int i = 0; i < nodes.size(); i++)
				{
					if(nodes[i]->tentativeDist < nodes[index]->tentativeDist){
						index = i;
					}
				}
				for (int i = 0; i < nodes.size(); i++)
				{
					if(nodes[i]->tentativeDist == nodes[index]->tentativeDist){
						//cout << nodes[i] -> first << " " << nodes[i]->second << " // ";
						if(nodes[i]->second > nodes[index]->second){
							index = i;
						}else if(nodes[i]->second == nodes[index]->second){
							if(nodes[i]->first < nodes[index]->first){
								index = i;
							}
						}
					}
				}
				//cout << "** " << nodes[index]->tentativeDist << endl;
			}
			else{
				for (int i = 0; i < nodes.size(); i++)
				{
					if(nodes[i]->f < nodes[index]->f){
						index = i;
					}
				}
				for (int i = 0; i < nodes.size(); i++)
				{
					if(nodes[i]->f == nodes[index]->f){
						//cout << nodes[i] -> first << " " << nodes[i]->second << " // ";
						if(nodes[i]->second > nodes[index]->second){
							index = i;
						}else if(nodes[i]->second == nodes[index]->second){
							if(nodes[i]->first < nodes[index]->first){
								index = i;
							}
						}
					}
				}
				//cout << "** " << nodes[index]->tentativeDist << endl;	
			}
			return index;
		}

//Reads the input file map, example maps exist in make directory
		void readMap(string filename){
			ifstream file;
			string line, first, second, temp;
			file.open(filename.c_str());

			//reading map size
			getline(file, line);
			size_t i = line.find(" ");
			first = line.substr(0, i);
			second = line.substr(i+1);
			size.first = atoi(first.c_str());
			size.second = atoi(second.c_str());

			//reading start position
			getline(file, line);
			i = line.find(" ");
			first = line.substr(0, i);
			second = line.substr(i+1);
			start.first = atoi(first.c_str()) - 1;
			start.second = atoi(second.c_str()) - 1;

			//reading end position
			getline(file, line);
			i = line.find(" ");
			first = line.substr(0, i);
			second = line.substr(i+1);
			end.first = atoi(first.c_str()) - 1;
			end.second = atoi(second.c_str()) - 1;

			int row = 0;
			while(getline(file, line)){
				vector<char> v;
				map.push_back(v);
				stringstream ss(line);
				while(getline(ss, temp, ' ')){
					map[row].push_back(temp[0]);
				}
				row++;
			}
		}

//calculated heuristic value table. Calculations are either euclidean or manhattan distance
//depending on parameter 'h'
		void calcHeuristic(string h){
			for(int i = 0; i < size.first; i++){
				vector<double> v;
				heuristicVal.push_back(v);
				for(int j = 0; j < size.second; j++){
					if(h == "manhattan"){
						double val1 = i - end.first;
						if(val1 < 0){
							val1 = val1 * (-1.0);
						}
						double val2 = j - end.second;
						if(val2 < 0){
							val2 = val2 * (-1.0);
						}
						heuristicVal[i].push_back(val1 + val2);
					}

					if(h == "euclidean"){
						double val1 = i - end.first;
						if(val1 < 0){
							val1 = val1 * (-1.0);
						}
						double val2 = j - end.second;
						if(val2 < 0){
							val2 = val2 * (-1.0);
						}

						double val = pow(val1, 2) + pow(val2, 2);
						heuristicVal[i].push_back(sqrt(val));
					}
				}
			}
		}

//Implements a Breadth-First Search algorithm
		bool BFS(){
			vector<vector<char> > copyMap = map;
			queue<Node > q;
			Node current = start;
			vector<vector<string> > path;
			for(int i = 0; i < size.first; i++){
				vector<string> v;
				path.push_back(v);
				for(int j = 0; j < size.second; j++){
					string x = "x";
					path[i].push_back(x);
				}
			}
			q.push(start);

			while(!q.empty()){
				if(current.first == end.first && current.second == end.second){
					copyMap = map;
					current = end;

					while(current.first != start.first || current.second != start.second){

						copyMap[current.first][current.second] = '*';

						if(path[current.first][current.second] == "u"){
							current.first += 1;
						}
						else if(path[current.first][current.second] == "d"){
							current.first -= 1;
						}
						else if(path[current.first][current.second] == "l"){
							current.second += 1;
						}
						else if(path[current.first][current.second] == "r"){
							current.second -= 1;
						}
					}
					copyMap[start.first][start.second] = '*';
					for(int i = 0; i < size.first; i++){
						for(int j = 0; j < size.second; j++){
							cout << copyMap[i][j] << " ";
						}
						cout << endl;
					}
					return true;
				}

				//up
				if(current.first > 0){
					Node up;
					up.first = current.first - 1;
					up.second = current.second;
					if(copyMap[up.first][up.second] != 'X'){
						q.push(up);
						copyMap[up.first][up.second] = 'X';
						path[up.first][up.second]= "u";
					}
				}
				//down
				if(current.first < (size.first-1)){
					Node down;
					down.first = current.first + 1;
					down.second = current.second;
					if(copyMap[down.first][down.second] != 'X'){
						q.push(down);
						copyMap[down.first][down.second] = 'X';
						path[down.first][down.second] = "d";
					}
				}
				//left
				if(current.second > 0){
					Node left;
					left.first = current.first;
					left.second = current.second - 1;
					if(copyMap[left.first][left.second] != 'X'){
						q.push(left);
						copyMap[left.first][left.second] = 'X';
						path[left.first][left.second] = "l";
					}
				}
				//right
				if(current.second < (size.second-1)){
					Node right;
					right.first = current.first;
					right.second = current.second + 1;
					if(copyMap[right.first][right.second] != 'X'){
						q.push(right);
						copyMap[right.first][right.second] = 'X';
						path[right.first][right.second] = "r";
					}
				}
				q.pop();
				current = q.front();
			}
			return false;
		};

//Implements a Uniform Cost Search pathfinding algorithm
		void uniformCost(){
			vector<Node*> visited;
			vector<Node*> unvisited;
			vector<vector<char> > copyMap = map;

			Node* current = new Node;
			current->first = start.second;
			current->second = start.first;
			current->tentativeDist = 0;
			current->elevation = map[current->first][current->second]-48;
			current->visited = true;
			current->previous = NULL;

			for(int i = 0; i < size.first; i++){
				for(int j = 0; j < size.second; j++){
					//cout << i << " " << j << endl;
					if((map[i][j] != 'X') && (j != current->first || i != current->second)){
						Node* n = new Node;
						n->first = j;
						n->second = i;
						n->elevation = map[i][j]-48;
						//cout << "***" << n->elevation << endl;
						n->tentativeDist = 100000;
						n->visited = false;
						unvisited.push_back(n);
						//cout<< n->elevation << " " << "(" << n->first << "," << n->second << ")" << endl;
					}
				}
			}
			cout << endl << endl;

			while(unvisited.size() > 0){
				vector<Node*> neighbours = findNodes(unvisited, current->first, current->second);
				for(int i = 0; i < neighbours.size(); i++){
					int diff = neighbours[i]->elevation - current->elevation;
					if(diff < 0){
						diff = 0;
					}

					if(neighbours[i]->tentativeDist > (current->tentativeDist + diff + 1)){
						neighbours[i]->tentativeDist = current->tentativeDist + diff + 1;
						neighbours[i]->previous = current;
					}
				}

				int index = findSmallest(unvisited, 0);
				//cout << "(" << unvisited[index]->first << "," << unvisited[index]->second << ") " << unvisited[index]->tentativeDist << endl;
				current = unvisited[index];
				unvisited.erase(unvisited.begin() + index);

				if(current->first == end.second && current->second == end.first){
					//cout << current ->tentativeDist << endl;
					break;
				}
			}

			while(current->previous != NULL){
				//cout << current-> first <<  " " << current->second << endl;
				copyMap[current->second][current->first] = '*';
				current = current->previous;
			}
			copyMap[current->second][current->first] = '*';

			for(int i = 0; i < size.first; i++){
				for(int j = 0; j < size.second; j++){
					cout << copyMap[i][j] << " ";
				}
				cout << endl;
			}			

		}

//Implements the A* pathfinding algorithm. Takes heuristic id in as a string parameter.
//Parameter must be either 'manhattan' or 'euclidean'
		void astar(string heuristic){
			calcHeuristic(heuristic);
			for(int i = 0; i < size.first; i++){
				for(int j = 0; j < size.second; j++){
					//cout << heuristicVal[i][j] << " ";
				}
				//cout << endl;
			}
			//cout << endl;

			vector<Node*> visited;
			vector<Node*> unvisited;
			vector<vector<char> > copyMap = map;

			Node* current = new Node;
			current->first = start.first;
			current->second = start.second;
			current->tentativeDist = 0;
			current->elevation = map[current->first][current->second]-48;
			current->visited = true;
			current->previous = NULL;

			for(int i = 0; i < size.first; i++){
				for(int j = 0; j < size.second; j++){
					if((map[i][j] != 'X') && (j != current->first || i != current->second)){
						Node* n = new Node;
						n->first = j;
						n->second = i;
						n->elevation = map[i][j]-48;
						n->tentativeDist = 100000;
						n->f = n->tentativeDist + heuristicVal[n->second][n->first];
						n->visited = false;
						unvisited.push_back(n);
					}
				}
			}

			while(unvisited.size() > 0){
				vector<Node*> neighbours = findNodes(unvisited, current->first, current->second);
				//cout << "--" << current->first << " " << current->second << endl;
				for(int i = 0; i < neighbours.size(); i++){
					int diff = neighbours[i]->elevation - current->elevation;
					if(diff < 0){
						diff = 0;
					}

					if(neighbours[i]->f > (current->tentativeDist + diff + 1 + heuristicVal[neighbours[i]->second][neighbours[i]->first])){
						neighbours[i]->tentativeDist = current->tentativeDist + diff + 1;
						neighbours[i]->f = neighbours[i]->tentativeDist + heuristicVal[neighbours[i]->second][neighbours[i]->first];
						neighbours[i]->previous = current;
						//cout << "**";
					}
					//cout << neighbours[i]->first << " " << neighbours[i]->second << " - " << neighbours[i]->f << endl;
				}

				int index = findSmallest(unvisited, 1);
				//cout << "(" << unvisited[index]->first << "," << unvisited[index]->second << ") " << unvisited[index]->tentativeDist << endl;
				current = unvisited[index];
				unvisited.erase(unvisited.begin() + index);

				if(current->first == end.second && current->second == end.first){
					//cout << current ->tentativeDist << endl;
					break;
				}
			}

			copyMap[start.first][start.second] = '*';
			while(current -> previous != NULL){
				copyMap[current->second][current->first] = '*';
				current = current -> previous;
			}

			for(int i = 0; i < size.first; i++){
				for(int j = 0; j < size.second; j++){
					cout << copyMap[i][j] << " ";
				}
				cout << endl;
			}
		}
};

int main(int argc, char* argv[]){

	if(argc < 2){
		cout << "At least 2 parameters required: \n 1 - map file \n 2 - algorithm to use (bfs, ucs, astar) \n 3 - astar algorithm heuristic" << endl;
		return 0;
	}

	PathFinder pathfinder = PathFinder();
	pathfinder.readMap(argv[1]);
	string algorithm = argv[2];
	string heuristic;
	if(argc >=4){
		heuristic = argv[3];
	}
	if(algorithm == "bfs"){
		pathfinder.BFS();
	}else if(algorithm == "ucs"){
		pathfinder.uniformCost();
	}else if(algorithm == "astar"){
		if(heuristic == "euclidean"){
			pathfinder.astar(heuristic);
		}else if(heuristic == "manhattan"){
			pathfinder.astar(heuristic);
		}else{
			cout << "please enter a heuristic as a third parameter (euclidean or manhattan)" << endl;
		}

	}else{
		cout << "invalid algorithm" << endl;
	}
	cout << endl;
	return 0;
}

