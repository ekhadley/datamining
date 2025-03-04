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


// given a dataset and the name of an an attribute, find the entropy of that attribute
float _entropyOfAttribute(const Dataset& d, string attribute_name) {

    int attr = -1;
    for (int i = 0; i < d.attrs.size(); i++) {
        if (d.attrs[i].name == attribute_name) {
            attr = i;
            break;
        }
    }
    vector<float> freqs(d.attrs[attr].vals.size(), 0);

    float inc = 1.0 / d.data.at(0).size();
    for (uint8_t val : d.data[attr]) {
        freqs.at(val) += inc;
    }

    float h = 0;
    for (float p : freqs) {
            h -= p * log2(p);
    }

    printf("[");
    for (float p : freqs) {
        printf(" %.3f ", p);
    }
    printf("]\n");
        
    return h;
}

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
    vector<vector<float>> freqs; // create a vector which will hold the frequencies of each class value, for each attribute value.
    vector<int> counts; // counts occurrences of each possible attribute value
    for (int i = 0; i < n_vals; i ++) {
        freqs.push_back(vector<float>()); // a list for each attribute value
        for (int j = 0; j < n_classes; j ++) {
            freqs[i].push_back(0); // a frequency count for each possible class value
        }
    }

    for (int i = 0; i < d.data.size(); i ++) {
        uint8_t class_val = d.data.at(class_id).at(i);
        uint8_t attr_val = d.data.at(attr_id).at(i);
        freqs.at(attr_val).at(class_val) += 1;
    }

    float h = 0;
    for (float f : freqs) {
            h -= f * log2(p);
    }

    printf("[");
    for (float f : freqs) {
        printf(" %.3f ", f);
    }
    printf("]\n");
        
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