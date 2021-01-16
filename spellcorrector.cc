#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <numeric>
using namespace std;

unordered_map<string, int> WORDS;
int sum = 0;

double P (string word) {
    return (WORDS[word] / (double) sum); // Probability of 'word'.
}

vector<string> known(vector<string> words) {
	// The subset of 'words' that appear in the dictionary of WORDS.
    vector<string> known_words;
    for (int i = 0; i < words.size(); i++) {
        if (WORDS.find(words[i]) != WORDS.end()) {
            known_words.push_back(words[i]);
        }
    }
    return known_words;
}

vector<string> edits1(string word) {
    vector<string> edits; // All edits that are one edit away from `word`
    vector<tuple<string, string>> splits;
    for (int i = 0; i < word.length() + 1; i++) {
        splits.push_back(make_tuple(word.substr(0, i), word.substr(i)));
    }
    for (int i = 0; i < splits.size(); i++) {
        string L = get<0>(splits[i]);
        string R = get<1>(splits[i]);
        if (R.length() > 0) {
            edits.push_back(L + R.substr(1));           // deletes
            for (char c = 'a'; c <= 'z'; c++) {
                edits.push_back(L + c + R.substr(1));   // replaces
            }
        }
        if (R.length() > 1) {                           // transposes
            edits.push_back(L + R[1] + R[0] + R.substr(2));
        }
        for (char c = 'a'; c <= 'z'; c++) {             // inserts
            edits.push_back(L + c + R);
        }
    }
    return edits;
}

vector<string> edits2(string word) {
    vector<string> edits;   // All edits that are two edits away from 'word'.
    vector<string> v1 = edits1(word);
    for (int i = 0; i < v1.size(); i++) {
        vector<string> v2 = edits1(v1[i]);
        for (int j = 0; j < v2.size(); j++) {
            edits.push_back(v2[j]);
        }
    }
    return edits;
}

vector<string> candidates(string word) {
	// Generate possible spelling corrections for word.
	vector<string> known1 = known(vector<string>{word});
	vector<string> known2 = known(edits1(word));
	vector<string> known3 = known(edits2(word));

	return (!known1.empty()) ? known1 : (!known2.empty()) ? known2 
		: (!known3.empty()) ? known3 : vector<string>{word};
}

string correction(string word) {
	// Generate the most probable correct word.
    double max = 0.0;
    string most_probable_word;
    vector<string> word_candidates = candidates(word);
    for (int i = 0; i < word_candidates.size(); i++) {
        string candidate = word_candidates[i];
        double probability = P(candidate);
        if (probability >= max) {
            max = probability;
            most_probable_word = candidate;
        }
    }
    return most_probable_word;
}

int main() {
	// Load words into map
    ifstream fs {"english_dictionary.txt"};
    string word;
    while (fs >> word) {
        WORDS[word]++;
        sum++;
    }
    cout << correction("somthing") << endl;
    cout << correction("korrectud") << endl;
    cout << correction("bycycle") << endl;
    cout << correction("inconvient") << endl;
    cout << correction("arrainged") << endl;
    cout << correction("peotry") << endl;
    cout << correction("peotryy") << endl;
    cout << correction("word") << endl;
    cout << correction("quintessential") << endl;
}
