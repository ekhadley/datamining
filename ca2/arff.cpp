#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
using namespace std;

// An Attribute is a wrapper for a vector of strings which are the possibilities of a particular property.
// Datapoints themselves store ints which are indices into the vector of property values.
struct Attribute {
    vector<string> vals;
    string name;
};

// This holds an arbitrary list of datapoints, in a Struct of Arrays format.
// This is done to reduce memory footprint and prioritize accesses which only
// look at a single or a few properties of a datapoint, which I think would
// be the more way to use it (as opposed to frequently accessing all or most
// of the properties of a data element).
struct DataList {
    vector<string> props; // holds an array for each property of the datapoints. We assume the porperty value IDs can fit into chars to save memory.
};

struct Dataset {
    DataList data; // struct of arrays for all datapoint's values.
    vector<Attribute> attrs; // list of attributes whcih each each datapoint has.
    string relation; // the name of the relation
};

Dataset readARFF(string fname) {
    ifstream arff(fname);
    Dataset d;

    string line;
    while (getline(arff, line)) { // seek to @relation label and record value
        if (line[0] != '%') continue; // ignore commented lines. (doesnt handle comments at other positions than 0)
        int len = line.length();
        if (line.find("@relation ") != line.npos) {
            d.relation = line.substr(10, len - 11); // grabs substring from position 10 to right before the last char.
            break;
        }

    }
    while (getline(arff, line)) { // record all attributes
        if (line[0] != '%') continue;
        int len = line.length();
        if (line.find("@attribute") != line.npos) {
            Attribute a; // attribute found
            int i = 11; // starting point of the attribute name
            while (line[i] != ' ') { i ++; }  // seek to space at end of attr name
            a.name = line.substr(11, i - 11); // store the name in the new attribute
            int vstart = i + 2; // marks the start of the value in the string
            int vend = i + 2; // marks the end of attribute value
            while (vend < len) { // while there are still attribute values to read
                if (line[vend] == ','){
                    bleh
                }

        }

    }
    return d;
}


int main() {
    Dataset d = readARFF("./restaurant.arff");

    cout << "relation name: " << d.relation << endl;
    cout << "relation name len: " << d.relation.length() << endl;


    return 0;
}
