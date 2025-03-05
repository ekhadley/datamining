// Ethan Hadley. 3/4/2025
// No known bugs. ChatGPT used to find bugs but nothing copypasted.

// The performance is difficult to measure. The tree formation and
// datastructures are highly unoptimized (see node class comments),
// but the algorithm completes practically instantly for all datasets
// becuase they are so small.

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <math.h>
#include "arff.cpp"
using namespace std;

// calculates the entropy of the given attribute ONLY on the datapoints specified by 'indices'.
float entropyOfDataset(const Dataset& d, int attr_id, const vector<int>& indices) {
    vector<float> freqs(d.attrs[attr_id].vals.size(), 0);
    float inc = 1.0 / indices.size();
    for (int i : indices) {
        uint8_t val = d.data.at(attr_id).at(i); // value of the datapoints for the given attribute
        freqs.at(val) += inc;
    }
    float h = 0;
    for (float p : freqs) {
        if (p != 0) {
            h -= p * log2(p);
        }
    }
    return h;
}

// calculates the expected info gain on the class attribute
float infoGain(const Dataset& d, int attr_id, const vector<int>& indices) {
    int class_id = d.attrs.size() - 1; // we assume the class attribute is the last attribute
    int n_classes = d.attrs.at(class_id).vals.size(); // number of possible class attribute values

    int n_vals = d.attrs.at(attr_id).vals.size(); // number of possible values for the class in question
    
    vector<vector<int>> counts; // create a vector which will hold the counts of each class value, for each attribute value
    vector<int> total_counts; // counts occurrences of each possible attribute value
    for (int i = 0; i < n_vals; i ++) {
        counts.push_back(vector<int>()); // a list for each attribute value
        total_counts.push_back(0);
        for (int j = 0; j < n_classes; j ++) {
            counts[i].push_back(0); // a frequency count for each possible class value
        }
    }
    int n_points = indices.size();
    for (int i : indices) { // go through all datapoints, adding up frequences for each class value for each attribute value
        uint8_t class_val = d.data.at(class_id).at(i); // attribute value of this datapoint
        uint8_t attr_val = d.data.at(attr_id).at(i); // class value of the datapoint``
        //printf("class_val: %d, attr_val: %d\n", class_val, attr_val);
        counts.at(attr_val).at(class_val) += 1; // log occurrence of class value under specific attr value
        total_counts.at(attr_val) += 1; // log occurrence of attr value
    }
    float h = 0;
    for (int i = 0; i < n_vals; i += 1) {// for each attr val
        int tcount = total_counts.at(i);
        if (tcount > 0) {
            float val_h = 0.0;
            for (int j = 0; j < n_classes; j += 1) { // for each class value
                float freq = (float)counts.at(i).at(j) / tcount;
                if (counts.at(i).at(j) > 0) {
                    val_h -= freq * log2(freq);
                }
            }
            h += float(tcount) / n_points * val_h;
        }
    }
    return entropyOfDataset(d, class_id, indices) - h;
}

// The output of the ID3 algorithm is represented as a tree of nodes, where each node is a branching point for all the possible values of a single attribute.
// It stores a different child node for each possible value of their attribute in a vector.
// The attribute-value pair is implicit in how the parent stores the child. If parent has attribute A, then its n'th child corresponds to the n'th possible value of attribute A.
struct ID3Node { 
    bool is_leaf = false;
    uint8_t predicted_class_id; // for leaf nodes. stores the resulting prediction of the tree.
    uint8_t attr = 255; // the index of the attribute the rule is about.
    vector<ID3Node> children; // vector of child nodes. each attribute value id maps to a different child node.

    // It is quite wasteful to store these 3 things in the node class, as they are only necessary
    // for creating the tree, not using it. I included them here for implementation simplicity.
    vector<int> indices; // indices of dataset elements which would reach this branch
    vector<int> remaining_attrs; // indices of attribtues which have not yet been branched on. The children may only branch off these attribtues.
    Dataset& d; // we store a reference to the dataset the tree is for.

    ID3Node(Dataset& dataset) : d(dataset) {}

    // This function finds the appropriate attribute to split on (setting self.attr), creates the necessary children for the values
    // of the attribute selected, and recursively repeats for all the children.
    void addChildren() {
        if (is_leaf) return; // we know when making the child wether it is a leaf or not. If so it has no children or attribute.

        int class_id = d.attrs.size() - 1; // the class attribute is assumed to be the last one listed in the dataset
        int n_class_vals = d.attrs.at(class_id).vals.size(); // the number of possible values for the class attribute.

        float max_ig = -1e9;
        // loop over remaining attributes to find out which attribute this node should cover, choosing the attribute with the highest information gain
        for (int attr_id : remaining_attrs) { // iterate over all non-class attributes,
            float ig = infoGain(d, attr_id, indices); // finding the entropy of the class value under all possible values of the attribute,
            if (ig > max_ig) { // storing the one with highest expected info gain.
                max_ig = ig;
                attr = attr_id;
            }
            //printf("current best attr: %d: ig of attr_id %d is %.3f\n", attr, attr_id, ig);
        }
        if (attr == 255) printf("ERROR ERROR attr was never set !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");

        vector<int> child_remaining_attrs; // holds the remaining attrs of the parent minus its own new attribute
        for (int i : remaining_attrs) {
            if (i != attr) {
                child_remaining_attrs.push_back(i); 
            }
        }
        for (int i = 0; i < d.attrs.at(attr).vals.size(); i ++) { // create and initialize all the children.
            ID3Node c(d);
            c.d = d;
            c.remaining_attrs = child_remaining_attrs;

            vector<int> class_val_counts(n_class_vals, 0); // list so we can count the frequency of class attribute values of the child
            for (int j : indices) {
                if (d.data.at(attr).at(j) == i) { // children's indices only include datapoints which reach the parent, and also have the corresponding value for that attribute.
                    c.indices.push_back(j);
                    class_val_counts.at(d.data.at(class_id).at(j)) += 1; // record all class attributes values of the child
                }
            }
            if (c.indices.size() > 0) { // we create children even if no data points would reach that value. This avoids doing extra stuff for empty branches.
                int nonzero = 0; // stores number of class values that occur at least once
                int max_class = 0; // stores number of occurrences of most common class value
                int max_class_id = 0; // stores the id of the most common class value
                for (int j = 0; j < n_class_vals; j++) {
                    int count = class_val_counts.at(j);
                    if (count > 0) nonzero += 1;
                    if (count > max_class) {
                        max_class = count;
                        max_class_id = j;
                    }
                }
                // If we only had 1 nonzero class value, we have a leaf node and predict that value upon reaching this child.
                // If we have branched on all possible attributes, but not narrowed down the class to a single value,
                // we choose to predict whichever class value was most common.
                // In either case, the child is terminal, and the class id we wish to predict is the one which occurred the most.
                if (nonzero == 1 || child_remaining_attrs.size() == 0) { 
                    c.is_leaf = true;
                    c.predicted_class_id = max_class_id;
                } else {
                    c.is_leaf = false;
                }
            } else {
                c.is_leaf = true;
            }
                
            children.push_back(c);
            c.addChildren();
        }
        //printf("current attribute was set as %d. created %d children\n\n", attr, children.size());
        for (auto &c : children) {
            c.addChildren();
        }
    }
    void print(const string &indent = "") const {
        for (int i = 0; i < children.size(); i++) {
            string attr_name = d.attrs.at(attr).name;
            const ID3Node& c = children.at(i);
            string val_name = d.attrs.at(attr).idToVal(i);
            cout << endl << indent << attr_name << " = " << val_name << ": ";
            if (c.is_leaf) {
                string class_val_name = d.attrs.at(d.attrs.size() - 1).idToVal(c.predicted_class_id);
                cout << class_val_name;
            } else {
                c.print(indent + "\t");
            }
        }
    }
};

ID3Node createID3Tree(Dataset& d) {
    ID3Node root(d);
    vector<int> indices;
    for (int i = 0; i < d.data.at(0).size(); i++) indices.push_back(i); // all datapoints reach the root node
    vector<int> remaining_attrs;
    for (int i = 0; i < d.attrs.size() - 1; i++) remaining_attrs.push_back(i); // all attributes (except the last, assumed to be the class) are available to be branched on
    root.indices = indices;
    root.remaining_attrs = remaining_attrs;
    root.addChildren(); // branch on the appropriate attribute, making more nodes recursively until we run out of attributes or acheive 100% information on the class attribute
    return root;
}
    

int main() {
    string fname;
    cout << "Enter the name of the file to parse. (e.g 'restaurant.arff'):  ";
    cin >> fname;
    while (!filesystem::exists(fname)) {
        cout << "'" << fname << "' was not found in the local directory. Please enter a valid file name:  ";
        cin >> fname;
    }
    ifstream arff(fname);
    Dataset d = readARFF(arff);
    //d.print();

    ID3Node id3 = createID3Tree(d);
    id3.print();

    return 0;
}
