#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <math.h>
#include "arff.cpp"
using namespace std;

float infoGain(const Dataset& d, string attribute_name) {
    int class_id = d.attrs.size() - 1; // we assume the last attribute is the target class
    int n_classes = d.attrs[class_id].vals.size(); // number of possible class attribute values

    int attr_id = -1;
    for (int i = 0; i < d.attrs.size(); i++) {
        if (d.attrs[i].name == attribute_name) {
            attr_id = i;
            break;
        }
    }
    int n_vals = d.attrs[attr_id].vals.size(); // number of possible values for the class in question
    vector<vector<int>> counts; // create a vector which will hold the counts of each class value, for each attribute value
    vector<int> total_counts; // counts occurrences of each possible attribute value
    for (int i = 0; i < n_vals; i ++) {
        counts.push_back(vector<int>()); // a list for each attribute value
        for (int j = 0; j < n_classes; j ++) {
            counts[i].push_back(0); // a frequency count for each possible class value
        }
    }

    for (int i = 0; i < d.data.at(attr_id).size(); i ++) { // adding up frequences for each class value for each attribute value
        uint8_t class_val = d.data.at(class_id).at(i); // attribute value of this datapoint
        uint8_t attr_val = d.data.at(attr_id).at(i); // class value of the datapoint``
        counts.at(attr_val).at(class_val) += 1; // log occurrence of class value under specific attr value
        total_counts.at(attr_val) += 1; // log occurrence of attr value
    }

    float h = 0;
    for (int i = 0; i < n_vals; i += 1) {// for each attr val
        for (int j = 0; j < n_classes; j += 1) { // for each class value
            float freq = counts.at(i).at(j) / total_counts.at(i);
            h -= total_counts.at(i) * log2(freq); // not proper information gain but has same order
        }
    }

    for (int i = 0; i < n_vals; i += 1) {// for each attr val
        printf("counts for attr %d: [", i)
        for (int j = 0; j < n_classes; j += 1) { // for each class value
            float freq = counts.at(i).at(j) / total_counts.at(i);
            printf(" %d ", counts.at(i).at(j));
        }
        printf("]\n");
    }
    return h;
}

// The output of the ID3 algorithm is represented as a tree of nodes, where each node is about a single attribute.
// It stores a different child node for each possible value of that attribute in an unordered_map.
struct ID3Node { 
    bool is_leaf = false;
    uint8_t classID; // for leaf nodes. stores the resulting prediction of the tree.
    const Attribute* att; // the attribute the rule is about.
    vector<int> indices; // indices from the dataset of data points who lie under this rule. children's indices are disjoint subsets whcih make up this whole list.
    unordered_map<uint8_t, ID3Node> children; // vector of child nodes. each attribte value id maps to a different child node.
};

int main() {
    string fname;
    //cout << "Enter the name of the file to parse. (e.g 'restaurant.arff'):  ";
    //cin >> fname;
    //while (!filesystem::exists(fname)) {
    //    cout << "'" << fname << "' was not found in the local directory. Please enter a valid file name:  ";
    //    cin >> fname;
    //}
    fname = "restaurant.arff";
    ifstream arff(fname);
    Dataset d = readARFF(arff);
    d.print();
    
    float h = entropyOfAttribute(d, "Bar");
    printf("entropy of attr is %f", h);

    return 0;
}
