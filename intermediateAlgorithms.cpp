// g++ intermediateAlgorithms.cpp -static-libstdc++ -static-libgcc -o p

#include <iostream>
#include "grafmodul.h"

using namespace std;

void CPM_forwardPass(Graf& g, vector<node>& nodes, vector<int>& topo) {
    for (int u : topo) {
        nodes[u].EF = nodes[u].ES + nodes[u].expectedTime;

        for (auto [v, w] : g.adjacencyList[u]) {
            nodes[v].ES = max(nodes[v].ES, nodes[u].EF);
        }
    }
}

void CPM_backwardPass(Graf& g, vector<node>& nodes, vector<int>& topo, int projectDuration) {
    for (int i = 1; i < nodes.size(); i++) {
        nodes[i].LF = projectDuration;
    }

    for (int i = topo.size() - 1; i >= 0; i--) {
        int u = topo[i];
        nodes[u].LS = nodes[u].LF - nodes[u].expectedTime;

        for (auto [v, w] : g.adjacencyList[u]) {
            nodes[u].LF = min(nodes[u].LF, nodes[v].LS);
            nodes[u].LS = nodes[u].LF - nodes[u].expectedTime;
        }
    }
}

void CPM(Graf& g, vector<node>& nodes) {
    if (!g.getDirection()) {
        throw HibasTipus();
    }

    vector<int> topo = g.topologicalOrder();

    CPM_forwardPass(g, nodes, topo);

    int projectDuration = 0;
    for (int i = 1; i < nodes.size(); i++) {
        projectDuration = max(projectDuration, nodes[i].EF);
    }

    CPM_backwardPass(g, nodes, topo, projectDuration);

    cout << "\nCPM eredmeny:\n";
    cout << "Node  ES  EF  LS  LF  Slack\n";

    for (int i = 1; i < nodes.size(); i++) {
        int slack = nodes[i].LS - nodes[i].ES;
        cout << i << "     "
             << nodes[i].ES << "   "
             << nodes[i].EF << "   "
             << nodes[i].LS << "   "
             << nodes[i].LF << "   "
             << slack << "\n";
    }

    cout << "\nProjekt idotartam: " << projectDuration << "\n";
}




int main() {
    freopen("input.txt", "r", stdin);
    Graf g;
    return 0;
}