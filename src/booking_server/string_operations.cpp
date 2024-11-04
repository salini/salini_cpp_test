#include "string_operations.h"

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

string join_vector_strings(const vector<string>& strings) {
    string concatenate;
    for (size_t idx=0; idx<strings.size(); ++idx) {
        concatenate += strings[idx];
        if (idx < strings.size() -1) {
            concatenate += " ";
        }
    }
    return concatenate;
}
