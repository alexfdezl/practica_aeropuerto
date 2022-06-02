#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <fstream>

using namespace std;

vector<int> splitInt (const string &s, char delim) {
    vector<int> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)){
        result.push_back (std::stoi(item));
    }

    return result;
}
vector<string> split (const string &s, char delim) {
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim)){
        result.push_back (item);
    }

    return result;
}
