#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
using namespace std;

struct Attribute {
    vector<string> vals;
    string name;

    uint8_t find(string val) {
        for (int i = 0; i < vals.size(); i++) {
            if (vals.at(i) == val) return i;
        }
        return vals.size();
    }

    string idToVal(uint8_t id) {
        if (id >= vals.size()) return "<UNKNOWN>";
        return vals.at(id);
    }

    void print() {
        cout << "Attribute '" << name << "' with values: {";
        for (int i = 0; i < vals.size() - 1; i++ ) {
            cout << "'" << vals.at(i) << "', ";
        }
        cout << "'" << vals.at(vals.size() - 1) << "'}\n";
    }
};


struct Dataset {
    vector<vector<uint8_t>> data; // array of arrays for all datapoint's values.
    vector<Attribute> attrs; // list of attributes which each each datapoint has.
    string relation; // the name of the relation

    void print() {
        cout << "Relation: '" << relation << "' with " << numDataPoints() << " data points and " << attrs.size() << " attributes:" << endl;
        for (auto attr : attrs) {
            cout << "  ";
            attr.print();
        }
        cout << "Datapoints:" << endl;
        for (int i = 0; i < numDataPoints(); i++) {
            printDataElem(i);
        }
    }
    int numDataPoints() {
        return data.at(0).size();
    }
    void printDataElem(int idx) {
        cout << "  " << idx << ":  ";
        for (int attr_idx = 0; attr_idx < attrs.size(); attr_idx++) {
            Attribute& attr = attrs.at(attr_idx);
            if (idx < data.at(attr_idx).size()) {
                int attr_id = data.at(attr_idx).at(idx);
                cout << attrs.at(attr_idx).name << ": " << attr.idToVal(attr_id) << "  ";
            } else {
                cout << attrs.at(attr_idx).name << ": " << "<UNSET>" << "  ";
            }
        }
        cout << endl;
    }
};

Dataset readARFF(ifstream& arff) {
    Dataset d;

    string line;
    int vstart, vend, len, attr_idx;
    while (getline(arff, line)) { // seek to @relation label and record value
        if (line[0] == '%' || line[0] == '\n' || line[0] == '\r') continue; // ignore commented/empty lines. (doesnt handle comments at other positions than 0)
        int len = line.length();
        if (line.find("@relation ") != -1) {
            if (line[len - 1] == '\r') {
                d.relation = line.substr(10, len - 11); // grabs substring from position 10 to right before the last char
            } else {
                d.relation = line.substr(10, len - 10); // grabs substring from position 10 to right before the last char
            }
            break;
        }
    }
    while (getline(arff, line)) { // record all attributes
        if (line[0] == '%' || line[0] == '\n' || line[0] == '\r') continue;
        len = line.length();
        if (line.find("@attribute") != -1) {

            Attribute a; // attribute found

            int endname = line.find_first_of(" \n\r\t", 11); // find position of second space on the line.
            a.name = line.substr(11, endname - 11); // store the name in the new attribute
            
            int start_val_list = line.find('{');
            vstart = start_val_list + 1; // marks the start of the value name in the string
            vend = start_val_list + 2; // marks the end of attribute value
            while (vend < len) { // while there are still attribute values to read
                if (line[vend] == ',' || line[vend] == '}'){ // vend is at end of new word
                    a.vals.push_back(line.substr(vstart, vend - vstart)); // append the substring as the value name
                    vend += 2; // move vstart and vend to start of next word
                    vstart = vend;
                } else { // vend is in middle of word. move it 1 right.
                    vend += 1;
                }
            }
            d.attrs.push_back(a); // add the completed attribute;
            d.data.push_back(vector<uint8_t>()); // vector for new column in the data

        } if (line.find("@data") != -1) {
            break;
        }
    }
    uint8_t attr_val_id;
    string attr_val;
    while (getline(arff, line)) { // record all attributes
        if (line[0] == '%' || line[0] == '\n' || line[0] == '\r') continue;
        len = line.length();
        attr_idx = 0; vstart = 0; vend = 0;
        while (vend < len) {
            if (line[vend] == ',' || vend == len - 1) { // if we've reached the comma between values or the last character
                if (vend == len - 1 && line[vend] != '\r') {
                    attr_val = line.substr(vstart, vend - vstart + 1); // get attribute value from list
                } else {
                    attr_val = line.substr(vstart, vend - vstart); // get attribute value from list
                }
                attr_val_id = d.attrs.at(attr_idx).find(attr_val); // find attribute value's index
                d.data.at(attr_idx).push_back(attr_val_id); // record the specific value ID for the proper attribute
                vstart = vend + 1;
                attr_idx += 1; // move to next attribute
            }
            vend += 1;
        }
    }
    return d;
}
