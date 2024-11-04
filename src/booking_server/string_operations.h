#ifndef __STRING_OPERATIONS_H__
#define __STRING_OPERATIONS_H__


#include <vector>
#include <string>

using namespace std;

/**
 * convert a string sentence composed of words spearated by space characters into a list of words
 * 
 * for instance "then sentence to split" -> ["the", "sentence", "to", "split"]
 * @param str the string sentence to split
 * @return the list of words
 */
vector<string> split_string(const string& str);

/**
 * convert a list of words into a string sentence composed of words separated by a space character
 * 
 * for instance ["the", "sentence", "to", "split"] -> "then sentence to split"
 * @param words the list of words to join
 * @return the string sentence of these joined words
 */
string join_vector_strings(const vector<string>& words);


#endif
