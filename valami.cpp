#include "grafmodul.h"

using namespace std;

struct ActivityInput {
    string name;
    int duration;
    vector<string> predecessors;
};


vector<ActivityInput> readCPM(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            throw FajlMegnyitas();
        }

        int n;
        file >> n;

        vector<ActivityInput> input(n);

        for (int i = 0; i < n; i++) {
            file >> input[i].name;
            file >> input[i].duration;

            string pred;
            file >> pred;

            if (pred != "-") {
                size_t pos;
                while ((pos = pred.find(',')) != string::npos) {
                    input[i].predecessors.push_back(pred.substr(0, pos));
                    pred.erase(0, pos + 1);
                }
                input[i].predecessors.push_back(pred);
            }
        }
        return input;
    }

void buildCPMGraph (Graf& g, vector<node>& nodes, vector<ActivityInput>& input) {
    unordered_map<string, int> id;
    int idx = 1;

    for (auto& a : input) {
        id[a.name] = idx++;
    }

    int n = input.size();
    g.setNumberOfNodes(n);
    g.setDirection(true);
    g.setWeighted(false);

    nodes.resize(n + 1);

    for (auto& a : input) {
        int u = id[a.name];
        nodes[u].value = u;
        nodes[u].expectedTime = a.duration;

        for (auto& p : a.predecessors) {
            g.listOfEdges.push_back({id[p], u});
        }
    }

    g.setNumberOfEdges(g.listOfEdges.size());
    g.fillAdjacencyList();
}

int main() {
    try {
        Graf g;
        vector<node> nodes;

        auto input = readCPM("input.txt");
        buildCPMGraph(g, nodes, input);

        g.CPM(nodes);
    }
    catch (exception& e) {
        cout << e.what();
    }

    return 0;
}
