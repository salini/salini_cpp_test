#include "string_operations.h"

#include <sstream>

using namespace std;


vector<string> split_string(const string& str) {
    stringstream ss(str);
    
    vector<string> splitted;
    while(!ss.eof()) {
        string tmp;
        ss >> tmp;
        if (tmp.length()) {
            splitted.push_back(tmp);
        }
    }
    return splitted;
}

string join_vector_strings(const vector<string>& words) {
    string concatenate;
    for (size_t idx=0; idx<words.size(); ++idx) {
        concatenate += words[idx];
        if (idx < words.size() -1) {
            concatenate += " ";
        }
    }
    return concatenate;
}
