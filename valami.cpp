// g++ valami.cpp -static-libstdc++ -static-libgcc -o p
#include "grafmodul.h"
#include <sstream>

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
            stringstream ss(pred);
            string token;

            while (getline(ss, token, ',')) {
                input[i].predecessors.push_back(token);
            }
        }
    }

    return input;
}

void buildCPMGraph (Graf& g, vector<node>& nodes, vector<ActivityInput>& input) {
    unordered_map<string, int> id;
    int idx = 1;

    for (int i = 0; i < input.size(); i++) {
        id[input[i].name] = idx++;
    }

    int n = input.size();
    g.setNumberOfNodes(n);
    g.setDirection(true);
    g.setWeighted(false);

    nodes.resize(n + 1);

    for (int i = 0; i < input.size(); i++) {
        int u = id[input[i].name];
        nodes[u].value = u;
        nodes[u].expectedTime = input[i].duration;

        for (int j = 0; j < input[i].predecessors.size(); j++) {
            g.listOfEdges.push_back({id[input[i].predecessors[j]], u});
        }
    }

    g.setNumberOfEdges(g.listOfEdges.size());
    g.fillAdjacencyList();
}

void readFlowIntoGraph(const string& filename, Graf& g, int& source, int& sink) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw FajlMegnyitas();
    }

    int n, m;
    file >> n >> m;
    file >> source >> sink;

    g.setNumberOfNodes(n);
    g.setNumberOfEdges(m);
    g.setDirection(true);
    g.setWeighted(true);

    g.listOfEdges.clear();

    for (int i = 0; i < m; i++) {
        edge e;
        file >> e.start >> e.end >> e.weight;
        g.listOfEdges.push_back(e);
    }

    g.fillAdjacencyMatrix(); // EZ FONTOS
}

void readBipartiteIntoGraf(const string& filename, Graf& g, int& nLeft, int& nRight) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw FajlMegnyitas();
    }

    int m;
    file >> nLeft >> nRight >> m;

    g.setNumberOfNodes(nLeft + nRight);
    g.setDirection(false);
    g.setWeighted(false);
    g.listOfEdges.clear();

    for (int i = 0; i < m; i++) {
        int u, v;
        file >> u >> v;
        g.listOfEdges.push_back({u, nLeft + v});
    }

    g.setNumberOfEdges(g.listOfEdges.size());
    g.fillAdjacencyList();
}

int main() {
    try {
        Graf g;
        vector<node> nodes;

        vector<ActivityInput> input = readCPM("input.txt");
        buildCPMGraph(g, nodes, input);

        g.CPM(nodes);

        Graf flowGraph;
        int source, sink;

        readFlowIntoGraph("flow.txt", flowGraph, source, sink);

        int maxFlow = flowGraph.edmondsKarp(source, sink);
        cout << "Maximalis folyam: " << maxFlow << '\n';

        Graf bipartit;
        int nLeft, nRight;

        readBipartiteIntoGraf("bipartit.txt", bipartit, nLeft, nRight);

        int maxMatching = bipartit.hopcroftKarp(nLeft, nRight);
        cout << "Maximalis parositas merete: " << maxMatching << '\n';

    }
    catch (exception& e) {
        cout << e.what();
    }

    return 0;
}
