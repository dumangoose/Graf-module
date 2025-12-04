// input.txt első sorában 1, ha irányított, 0 ha irányítatlan, 1 ha súlyozott, 0 ha nem

#ifndef GRAFMODUL_H
#define GRAFMODUL_H

#include <iostream>
#include <vector>
#include <cstring>
#include <queue>
#include <stack>
#include <fstream>
#include <algorithm>
#include <set>
#include <unordered_map>

using namespace std;

class HibasTipus : public exception {
public:
    const char *what() const noexcept override {
        return "Hibas tipus kivetelt dobok neked most, kapd el\n";
    }
};

class NemOsszefuggo : public exception {
public:
    const char *what() const noexcept override {
        return "Nem osszefuggo kivetelt dobok neked most, kapd el\n";
    }
};

class NemKormentes : public exception {
public:
    const char *what() const noexcept override {
        return "Nem kormentes kivetelt dobok neked most, kapd el\n";
    }
};

class NegativEl : public exception {
public:
    const char *what() const noexcept override {
        return "Negativ el kivetelt dobok neked most, kapd el\n";
    }
};

class NemFa : public exception {
public:
    const char *what() const noexcept override {
        return "NemFa kivetelt dobok neked most, kapd el\n";
    }
};

class FajlMegnyitas : public exception {
public:
    const char *what() const noexcept override {
        return "Nem sikerult megnyitni a bemeneti fajlt!\n";
    }
};

struct node {
    int value;
};

struct edge {
    int start;
    int end;
    int weight = 0;
};

struct huffmanTreeNode {
    char c;
    int freq;
    huffmanTreeNode* left;
    huffmanTreeNode* right;

    huffmanTreeNode(char c, int freq) : c(c), freq(freq), left(nullptr), right(nullptr) {}

    huffmanTreeNode(char c, int freq, huffmanTreeNode* left, huffmanTreeNode* right) : c(c), freq(freq), left(left), right(right) {}
};

class Graf {
private:
    int n, m;                       // csomopontok szama, elek szama
    vector<vector<pair<int, int>>> adjacencyList;
    vector<vector<int>> incidenceMatrix;
    vector<vector<int>> adjacencyMatrix;
    vector<int> parentArray;
    vector<edge> listOfEdges;
    bool directed; // 1, ha directed, 0, ha iranyitatlan
    bool weighted; // 1, ha weighted, 0, ha nem
    bool tree; // 1, ha tree, 0, ha nem
public:
    Graf reading(const char *s) {
        ifstream file(s);

        if (!file.is_open()) {
            throw FajlMegnyitas();
        }

        file >> n >> m;
        file >> directed;
        file >> weighted;
        listOfEdges.resize(m);

        for (int i = 0; i < m; i++)
        {
            file >> listOfEdges[i].start >> listOfEdges[i].end;
            if (listOfEdges[i].start > listOfEdges[i].end) {
                swap(listOfEdges[i].start, listOfEdges[i].end);
            }
            if (weighted == true) {
                file >> listOfEdges[i].weight;
            }
        }

        sort(listOfEdges.begin(), listOfEdges.end(), [](const edge& a, const edge& b) {
            if (a.start != b.start) {
                return a.start < b.start;
            }
            return a.end < b.end;
        });

        file.close();

        fillAdjacencyList();
        fillIncidenceMatrix();
        fillAdjacencyMatrix();

        return *this;
    }

    void writeListOfEdges() {
        cout << "\nEllista:\n\n";
        for (int i = 0; i < listOfEdges.size(); i++) {
            cout << "(" << listOfEdges[i].start << ", " << listOfEdges[i].end; // olvashatoan formazzuk
            if (weighted == true) {
                cout << ", suly = " << listOfEdges[i].weight;
            }
            cout << ")\n";
        }
    }

    void fillAdjacencyList() {
        adjacencyList.resize(n + 1); // annyi listat tartalmaz ahany Node van

        for (int i = 0; i < m; i++) {
            // haladok vegig a szomszedsagi listan
            // es beallitom a adjacencyList tombben mindket csomopontra, hogy a masikkal van kozos eluk
            adjacencyList[listOfEdges[i].start].push_back({listOfEdges[i].end, listOfEdges[i].weight});
            if (directed == 0) {
                adjacencyList[listOfEdges[i].end].push_back({listOfEdges[i].start, listOfEdges[i].weight});
            }
        }
    }

    void writeAdjacencyList() {
        cout << "\n\nSzomszedsagi lista:\n\n";
        for (int i = 1; i <= n; i++) { // olvashatosag kedveert formazzuk
            cout << i << ": ";
            for (int j = 0; j < adjacencyList[i].size(); j++) {
                cout << adjacencyList[i][j].first;
                if (weighted == true) {
                    cout << "(" << adjacencyList[i][j].second << ")";
                }
                if (j != adjacencyList[i].size() - 1) {
                    cout << ", ";
                }
            }
            cout << "\n";
        }
    }

    void fillIncidenceMatrix() {
        incidenceMatrix.resize(n + 1, vector<int>(m, 0));

        for (int i = 0; i < m; i++) {
            incidenceMatrix[listOfEdges[i].start][i] = 1; 
            incidenceMatrix[listOfEdges[i].end][i] = directed ? -1 : 1;
            if (weighted == true) {
                incidenceMatrix[0][i] = listOfEdges[i].weight;
            }
        }
    }

    void writeIncidenceMatrix() {
        cout << "\n\nIncidencia matrix:\n\n   ";
        for (int i = 1; i <= m; i++) {
            cout << i << "  ";
        }
        cout << "\n   ";
        if (weighted == true) {
            for (int i = 0; i < m; i++) {
                cout << incidenceMatrix[0][i] << "  ";
            }
            cout << '\n';
        }
        
        for (int i = 1; i <= n; i++) {
            cout << i << ": ";
            for (int j = 0; j < m; j++) {
                cout << incidenceMatrix[i][j];
                if (j != m - 1) {
                    cout << ", ";
                }
            }
            cout << "\n";
        }
    }

    void fillAdjacencyMatrix() {
        adjacencyMatrix.resize(n + 1, vector<int>(n + 1, INT_MAX));

        for (int i = 0; i < m; i++) {
            adjacencyMatrix[listOfEdges[i].start][listOfEdges[i].end] = weighted ? listOfEdges[i].weight : 1;
            if (directed == 0) {
                adjacencyMatrix[listOfEdges[i].end][listOfEdges[i].start] = weighted ? listOfEdges[i].weight : 1;
            }
        }
    }

    void writeAdjacencyMatrix() {
        cout << "\n\nSzomszedsagi matrix:\n\n  ";
        for (int i = 1; i <= n; i++) {
            cout << i << " ";
        }
        cout << "\n";
        for (int i = 1; i <= n; i++) {
            cout << i << " ";
            for (int j = 1; j <= n; j++) {
                cout << adjacencyMatrix[i][j] << " ";
            }
            cout << "\n";
        }
    }

    void writeArray(vector<int> &array) {
        for (int i = 0; i < array.size(); i++) {
            cout << array[i] << " ";
        }
        cout << "\n";
    }

    vector<int> isolatedNodes() {
        vector<int> nodes;

        for (int i = 1; i <= n; i++) {
            bool onlyZeros = true;
            for (int j = 1; j <= n; j++) {
                if (adjacencyMatrix[i][j] != INT_MAX) {
                    onlyZeros = false;
                    break;
                }
            }
            if (onlyZeros == true) {
                nodes.push_back(i);
            }
        }

        writeArray(nodes);

        return nodes;
    }

    vector<int> endpoints() {
        vector<int> nodes;
        for (int i = 1; i <= n; i++) {
            int count = 0;
            for (int j = 0; j < m; j++) {
                if (incidenceMatrix[i][j] == 1) {
                    count++;
                }
            }
            if (count == 1) {
                nodes.push_back(i);
            }
        }

        writeArray(nodes);

        return nodes;
    }

    int degree(node Node) {
        if (directed == 1) {
            throw HibasTipus();
        }
        int count = 0;
        for (int i = 0; i < m; i++) {
            if (Node.value == listOfEdges[i].start || Node.value == listOfEdges[i].end) {
                count++;
            }
        }

        return count;
    }

    int outDegree(node Node)
    {
        if (directed == 0) {
            throw HibasTipus();
        }
        int count = 0;
        for (int i = 0; i < m; i++) {
            if (Node.value == listOfEdges[i].start) {
                count++;
            }
        }
        return count;
    }

    int inDegree(node Node) {
        if (directed == 0) {
            throw HibasTipus();
        }
        int count = 0;
        for (int i = 0; i < m; i++) {
            if (Node.value == listOfEdges[i].end) {
                count++;
            }
        }
        return count;
    }

    void BFS(node startingNode) {
        queue<int> q;
        vector<bool> visited(n + 1, 0);
        visited[startingNode.value] = true;
        q.push(startingNode.value);

        while (q.empty() == false) {
            int current = q.front();
            q.pop();
            cout << current << " ";

            for (int i = 0; i < adjacencyList[current].size(); i++) {
                if (visited[adjacencyList[current][i].first] == false) {
                    q.push(adjacencyList[current][i].first);
                    visited[adjacencyList[current][i].first] = true;
                }
            }        
        }

        cout << '\n';
    }

    void DFS(node startingNode) {
        stack<int> st;
        vector<bool> visited(n + 1, 0);
        st.push(startingNode.value);

        while (st.empty() == false) {
            int current = st.top();
            st.pop();

            if (visited[current]) {
                continue; 
            } 
            visited[current] = true;
            cout << current << " ";

            for (int i = adjacencyList[current].size() - 1; i >= 0; i--) {
                int neighbour = adjacencyList[current][i].first;
                if (!visited[neighbour]) {
                    st.push(neighbour);
                }
            } 
            }

        cout << "\n\n";
    }

    vector<int> kthDegreeAcquaintance(node startingNode, int k)
    {
        vector<int> result;
        vector<int> d(n + 1, -1);
        d[startingNode.value] = 0;
        queue<int> q;
        q.push(startingNode.value);

        while (!q.empty()) {
            int current = q.front();
            q.pop();
            for (int i = 0; i < m; i++) {
                int neighbour = -1;
                if (directed == 1) {
                    if (listOfEdges[i].start == current) {
                        neighbour = listOfEdges[i].end;
                    }
                } else {
                    if (listOfEdges[i].start == current) {
                        neighbour = listOfEdges[i].end;
                    } else if (listOfEdges[i].end == current) {
                        neighbour = listOfEdges[i].start;
                    }
                }

                if (neighbour != -1 && d[neighbour] == -1) {
                    d[neighbour] = d[current] + 1;
                    q.push(neighbour);
                }
            }
        }
        for (int i = 0; i < n + 1; i++) {
            if (d[i] == k) {
                result.push_back(i);
            }
        }
        return result;
    }

    bool isConnected() {
        int startingNode = 1;
        stack<int> st;
        vector<bool> visited(n + 1, 0);
        visited[startingNode] = true;
        st.push(startingNode);

        while (st.empty() == false) {
            int current = st.top();
            st.pop();
            // cout << current << " ";

            for (int i = 0; i < listOfEdges.size(); i++) {
                int u = listOfEdges[i].start;
                int v = listOfEdges[i].end;

                if (current == u && visited[v] == false) {
                    st.push(v);
                    visited[v] = true;
                }
                else if (current == v && visited[u] == false) {
                    st.push(u);
                    visited[u] = true;
                }
            }
        }

        for (int i = 1; i < n + 1; i++) {
            if (visited[i] == false) {
                return false;
            }
        }
        return true;
    }

    bool DFScircle(int Node, vector<int> &state) {
        state[Node] = 1;
        for (int i = 0; i < m; i++) {
            if (listOfEdges[i].start != Node) {
                continue;
            }

            if (state[listOfEdges[i].end] == 0) {
                if (DFScircle(listOfEdges[i].end, state) == true) {
                    return true;
                }
            } else if (state[listOfEdges[i].end] == 1) {
                return true;
            }
        }

        state[Node] = 2;
        return false;
    }

    bool circleCheck() {
        if (isConnected() == 0) {
            throw NemOsszefuggo();
        }
        if (directed == 0) {
            throw HibasTipus();
        }

        vector<int> state(n + 1, 0);
        for (int i = 1; i < n + 1; i++) {
            if (state[i] == 0) {
                if (DFScircle(i, state) == true) {
                    return true;
                }
            }
        }
        return false;
    }

    void DFStopologicalOrder(int Node, vector<bool> &visited, stack<int> &order) {
        visited[Node] = true;
        for (int i = 0; i < m; i++) {
            if (Node == listOfEdges[i].start) {
                if (visited[listOfEdges[i].end] == false) {
                    DFStopologicalOrder(listOfEdges[i].end, visited, order);
                }
            }
        }
        order.push(Node);
    }

    vector<int> topologicalOrder() {
        if (circleCheck() == 1) {
            throw NemKormentes();
        }
        if (directed == 0) {
            throw HibasTipus();
        }
        vector<bool> visited(n + 1, 0);
        stack<int> order;
        for (int i = 1; i < n + 1; i++) {
            if (visited[i] == false) {
                DFStopologicalOrder(i, visited, order);
            }
        }
        vector<int> result;
        while (!order.empty()) {
            result.push_back(order.top());
            order.pop();
        }
        if (result.size() != n) {
            throw NemKormentes();
        }
        return result;
    }

    void DFSstrongConnect(int Node, int &indexCount, vector<int> &index, vector<int> &lowlink, vector<bool> &inStack, stack<int> &st, vector<vector<int>> &components) {
        index[Node] = indexCount;
        lowlink[Node] = indexCount;
        indexCount++;
        st.push(Node);
        inStack[Node] = true;
        for (pair<int, int> neighbour : adjacencyList[Node]) {
            if (index[neighbour.first] == -1) {
                DFSstrongConnect(neighbour.first, indexCount, index, lowlink, inStack, st, components);
                lowlink[Node] = min(lowlink[Node], lowlink[neighbour.first]);
            } else if (inStack[neighbour.first]) {
                lowlink[Node] = min(lowlink[Node], index[neighbour.first]);
            }
        }
        if (lowlink[Node] == index[Node]) {
            vector<int> component;
            int helper;
            do {
                helper = st.top();
                st.pop();
                inStack[helper] = false;
                component.push_back(helper);
            } while (helper != Node);
            components.push_back(component);
        }
    }

    vector<vector<int>> strongConnect() {
        if (directed == false) {
            throw HibasTipus();
        }

        vector<vector<int>> components;
        vector<int> index(n + 1, -1);
        vector<int> lowlink(n + 1, -1);
        vector<bool> inStack(n + 1, 0);
        stack<int> s;
        int indexCount = 0;
        for (int i = 1; i < n + 1; i++) {
            if (index[i] == -1) {
                DFSstrongConnect(i, indexCount, index, lowlink, inStack, s, components);
            }
        }
        cout << "\nErosen osszefuggo komponensek:\n";
        for (int i = 0; i < components.size(); i++) {
            cout << i + 1 << ": ";
            for (int cs : components[i])
                cout << cs << " ";
            cout << "\n";
        }
        return components;
    }

    bool getDirection() {
        return directed;
    }

    int getNumberOfNodes() {
        return n;
    }

    bool getWeighted() {
        return weighted;
    }

    int find(int x, vector<int>& disjointSet) {
        while (disjointSet[x] != x) {
            x = disjointSet[x];
        }

        return x;
    }

    Graf kruskal() {
        if (!isConnected() || directed || !weighted) {
            throw HibasTipus();
        }
        
        sort(listOfEdges.begin(), listOfEdges.end(), [](const edge& a, const edge& b) {
            return a.weight < b.weight;
        });

        vector<int> disjointSet(n + 1);
        for (int i = 1; i <= n; i++) {
            disjointSet[i] = i;
        }

        Graf minimumCostSpanningTree;

        int i = 0;
        int j = 0;

        while (j < m && i < (n - 1)) {
            int u = listOfEdges[j].start;
            int v = listOfEdges[j].end;

            int rootU = find(u, disjointSet);
            int rootV = find(v, disjointSet);

            if (rootU != rootV) {
                i++;
                minimumCostSpanningTree.listOfEdges.push_back({u, v, 0});
                disjointSet[rootV] = rootU;
            }
            j++;
        }

        minimumCostSpanningTree.m = minimumCostSpanningTree.listOfEdges.size();

        return minimumCostSpanningTree;
    }

    Graf prim() {
        if (directed || !weighted) {
            throw HibasTipus();
        }

        vector<bool> been(n + 1, false);
        vector<int> smallestWeight(n + 1, INT_MAX);
        vector<int> parent(n + 1, -1);

        int start = 1;
        smallestWeight[start] = 0;

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
        pq.push({0, start});

        Graf minimumCostSpanningTree;

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            if (been[u]) {
                continue;
            }
            been[u] = true;

            if (parent[u] != -1) {
                minimumCostSpanningTree.listOfEdges.push_back({parent[u], u, adjacencyMatrix[parent[u]][u]});
            }

            for (int v = 1; v <= n; v++) {
                int weight = adjacencyMatrix[u][v];
                if (weight != 0 && weight < INT_MAX && !been[v] && weight < smallestWeight[v]) {
                    smallestWeight[v] = weight;
                    parent[v] = u;
                    pq.push({weight, v});
                }
            }
        }

        if ((int)minimumCostSpanningTree.listOfEdges.size() < n - 1) {
            throw HibasTipus();
        }

        minimumCostSpanningTree.m = minimumCostSpanningTree.listOfEdges.size();

        return minimumCostSpanningTree;
    }

    vector<vector<int>> dijkstra(node Node) {
        // a legrovidebb utvonalakat adja vissza
        if (!weighted) {
            throw HibasTipus();
        }

        for (int i = 0; i < m; i++) {
            if (listOfEdges[i].weight < 0) {
                throw NegativEl();
            }
        }

        vector<int> d(n + 1, INT_MAX);
        vector<int> p(n + 1, -1);
        int s = Node.value;
        vector<bool> visited(n + 1, false);

        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

        d[s] = 0;
        pq.push({0, s});

        while (!pq.empty()) {
            pair<int, int> current = pq.top();
            pq.pop();
            int u = current.second;

            if (visited[u]) {
                continue;
            }
            visited[u] = true;
            
            if (d[u] == INT_MAX) {
                continue;
            }

            for (int i = 1; i <= n; i++) {
                int j = adjacencyMatrix[u][i];
                if (j == INT_MAX) {
                    continue;
                }
                if (d[i] > d[u] + j) {
                    d[i] = d[u] + j;
                    p[i] = u;
                    pq.push({d[i], i});
                }
            }
        }

        vector<vector<int>> roads(n + 1);

        for (int i = 1; i <= n; i++) {
            if (d[i] == INT_MAX) {
                continue;
            }
            vector<int> road;
            for (int v = i; v != -1; v = p[v]) {
                road.push_back(v);
            }
            reverse(road.begin(), road.end());
            roads[i] = road;
        }

        return roads;
    }

    vector<int> bellmanFord(node Node) {
        // a legrovidebb utvonalak tavolsagat adja vissza

        if (!directed) {
            throw HibasTipus();
        }

        vector<int> d(n + 2, INT_MAX);
        int s = Node.value;
        d[s] = 0;

        for (int i = 1; i <= n - 1; i++) {
            bool changed = false;
            for (int j = 0; j < (int)listOfEdges.size(); j++) {
                int u = listOfEdges[j].start;
                int v = listOfEdges[j].end;
                int w = listOfEdges[j].weight;

                if (d[u] != INT_MAX && d[u] + w < d[v]) {
                    d[v] = d[u] + w;
                    changed = true;
                }
            }
            if (!changed) {
                break;
            }
        }

        return d;
    }

    vector<vector<vector<int>>> johnson() {
        // elso dimenzio: start
        // masodik dimenzio: end
        // harmadik dimenzio: koztes csomopontok
        // osszes csomopontbol osszes csomopontba legrovidebb utakat tartalmazza

        if (!directed) {
            throw HibasTipus();
        }

        node q = {n + 1};

        int newN = n + 1;
        int newM = m + n;

        for (int i = 1; i <= n; i++) {
            listOfEdges.push_back({q.value, i, 0});
        }

        vector<int> h = bellmanFord(q);

        for (int i = 0; i < m; i++) {
            listOfEdges[i].weight += (h[listOfEdges[i].start] - h[listOfEdges[i].end]);
        }

        listOfEdges.resize(m);

        vector<vector<vector<int>>> result(n + 1);

        for (int i = 1; i <= n; i++) {
            vector<vector<int>> roads = dijkstra({i});
            result[i] = roads;
        }

        return result;
    }

    vector<vector<int>> royFloydWarshall() {
        // tavolsagi matrix
        vector<vector<int>> d(n + 1, vector<int>(n + 1, INT_MAX / 2)); // tulcsordulas

        for (int i = 1; i <= n; i++) {
            d[i][i] = 0;
        }

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (adjacencyMatrix[i][j] != 0 && adjacencyMatrix[i][j] != INT_MAX) {
                    d[i][j] = adjacencyMatrix[i][j];
                }
            }
        }

        for (int k = 1; k <= n; k++) {
            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= n; j++) {
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
                }
            }
        }

        return d;
    }

    int pathReconstruction(node Node, vector<int> path) {
        int sum = 0;

        for (int i = 0; i < path.size() - 1; i++) {
            int u = path[i];
            int v = path[i + 1];

            int weight = adjacencyMatrix[u][v];
            if (weight == INT_MAX) {
                cout << "Nincs el";
                return 0;
            }
            sum += weight;
        }

        return sum;
    }

    void fillParentArray() {
        if (!isTree()) {
            throw NemFa();
        }

        parentArray.resize(n + 1, -1);

        if (directed) {
            for (int i = 0; i < m; i++) {
                int u = listOfEdges[i].start;
                int v = listOfEdges[i].end;
                parentArray[v] = u;
            }
        } else {
            int root = 1;
            vector<bool> visited(n + 1, false);
            stack<int> st;
            st.push(root);
            visited[root] = true;

            vector<vector<int>> al(n + 1);
            for (int i = 0; i < m; i++) {
                int u = listOfEdges[i].start;
                int v = listOfEdges[i].end;
                al[u].push_back(v);
                al[v].push_back(u);
            }

            while (!st.empty()) {
                int u = st.top();
                st.pop();

                for (int v : al[u]) {
                    if (!visited[v]) {
                        visited[v] = true;
                        parentArray[v] = u;
                        st.push(v);
                    }
                }
            }
        }
    }
    
    bool isTree() {
        if (m != n - 1) {
            return false;
        }

        if (!isConnected()) {
            return false;
        }

        if (directed) {
            if (circleCheck()) {
                return false;
            }
        }

        tree = true;
        return true;
        
    }
    
    pair<int,int> DFSfurthestNode(node Node) {
        vector<vector<pair<int, int>>> al(n + 1);

        for (int i = 0; i < m; i++) {
            al[listOfEdges[i].start].push_back({listOfEdges[i].end, weighted ? listOfEdges[i].weight : 1});
            al[listOfEdges[i].end].push_back({listOfEdges[i].start, weighted ? listOfEdges[i].weight : 1});
        }

        vector<int> visited(n + 1, 0);
        vector<int> d(n + 1, 0);

        stack<int> st;
        int start = Node.value;
        st.push(start);

        visited[start] = 1;
        d[start] = 0;

        int furthestNode = start;
        int maxDistance = 0;

        while (!st.empty()) {
            int u = st.top();
            st.pop();

            if (d[u] > maxDistance) {
                maxDistance = d[u];
                furthestNode = u;
            }

            for (int i = 0; i < al[u].size(); i++){
                if (!visited[al[u][i].first]) {
                    visited[al[u][i].first] = 1;
                    d[al[u][i].first] = d[u] + al[u][i].second;
                    st.push(al[u][i].first);
                }
            }
        }

        return {furthestNode, d[furthestNode]};
    }

    int diameter() {
        if (isTree()) {
            if (!weighted && !directed) {
                int a = DFSfurthestNode({1}).first;
                pair<int, int> b = DFSfurthestNode({a});
                return b.second;
            }
        }
        else {
            throw NemFa();
        }

        return 0;
    }

    vector<node> center() {
        if (!isTree()) {
            throw NemFa();
        }

        pair<int, int> firstDFS = DFSfurthestNode({1});
        int furthestNode = firstDFS.first;
        pair<int, int> secondDFS = DFSfurthestNode({furthestNode});
        int otherEnd = secondDFS.first;

        vector<int> path;
        vector<int> visited(n + 1, false);
        vector<int> parent(n + 1, -1);

        stack<int> st;
        st.push(furthestNode);
        visited[furthestNode] = true;

        vector<vector<pair<int, int>>> al(n + 1);
        for (int i = 0; i < m; i++) {
            int u = listOfEdges[i].start;
            int v = listOfEdges[i].end;
            int w = weighted ? listOfEdges[i].weight : 1;
            al[u].push_back({v, w});
            al[v].push_back({u, w});
        }

        while (!st.empty()) {
            int u = st.top();
            st.pop();

            if (u == otherEnd) {
                break;
            }

            for (auto [v, w] : al[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    parent[v] = u;
                    st.push(v);
                }
            }
        }

        int current = otherEnd;
        while (current != -1) {
            path.push_back(current);
            current = parent[current];
        }
        reverse(path.begin(), path.end());

        vector<node> centers;
        int len = path.size();
        if (len % 2 == 1) {
            centers.push_back({path[len / 2]});
        } else {
            centers.push_back({path[len / 2 - 1]});
            centers.push_back({path[len / 2]});
        }

        return centers;
    }

    void unionSet(vector<int>& parent, vector<int>& rank, int x, int y) {
        int xroot = find(x, parent);
        int yroot = find(y, parent);

        if (rank[xroot] < rank[yroot]) {
            parent[xroot] = yroot;
        }
        else if (rank[xroot] > rank[yroot]) {
            parent[yroot] = xroot;
        }
        else {
            parent[yroot] = xroot;
            rank[xroot]++;
        }
    }

    Graf boruvka() {
        if (!weighted) {
            throw HibasTipus();
        }

        int V = n;
        Graf MST;
        MST.n = V;

        vector<int> parent(V + 1);
        vector<int> rank(V + 1, 0);

        for (int i = 1; i <= V; i++) {
            parent[i] = i;
        }

        int numTrees = V;

        while (numTrees > 0) {
            vector<vector<int>> cheapest(V + 1, vector<int>(3, -1));

            for (auto& e : listOfEdges) {
                int u = e.start;
                int v = e.end;
                int w = e.weight;
                int set1 = find(u, parent);
                int set2 = find(v, parent);

                if (set1 != set2) {
                    if (cheapest[set1][2] == -1 || cheapest[set1][2] > w)
                        cheapest[set1] = {u, v, w};
                    if (cheapest[set2][2] == -1 || cheapest[set2][2] > w)
                        cheapest[set2] = {u, v, w};
                }
            }

            bool merged = false;
            for (int i = 1; i <= V; i++) {
                if (cheapest[i][2] != -1) {
                    int u = cheapest[i][0];
                    int v = cheapest[i][1];
                    int w = cheapest[i][2];
                    int set1 = find(u, parent), set2 = find(v, parent);

                    if (set1 != set2) {
                        MST.listOfEdges.push_back({u, v, w});
                        MST.m++;

                        unionSet(parent, rank, set1, set2);
                        numTrees--;
                        merged = true;
                    }
                }
            }

            if (!merged) {
                break;
            }
        }

        return MST;
    }

    vector<int> prufferEncode() {
        if (!isTree()) {
            throw NemFa();
        }

        vector<int> p;
        vector<int> degrees(n + 1);
        int minimumEdge = 0;

        for (int i = 1; i < n + 1; i++) {
            degrees[i] = degree({i});
        }

        set<int> leaves;
        for (int i = 1; i <= n; i++)
            if (degrees[i] == 1) {
                leaves.insert(i);
            }

        for (int i = 0; i < n - 2; i++) {
            int leaf = *leaves.begin();
            leaves.erase(leaves.begin());

            int neighbour = -1;
            for (int j = 0; j < listOfEdges.size(); j++) {
                if (listOfEdges[j].start == leaf && degrees[listOfEdges[j].end] > 0) { 
                    neighbour = listOfEdges[j].end; 
                    break; 
                }
                if (listOfEdges[j].end == leaf && degrees[listOfEdges[j].start] > 0) { 
                    neighbour = listOfEdges[j].start; 
                    break; 
                }
            }

            p.push_back(neighbour);

            degrees[leaf]--;
            degrees[neighbour]--;
            if (degrees[neighbour] == 1) {
                leaves.insert(neighbour);
            }
        }

        return p;
    }

    Graf prufferDecode(vector<int>& code) {
        vector<int> degree(n + 1, 1);
        for (int i = 0; i < code.size(); i++) {
            degree[code[i]]++;
        }

        set<int> leaves;
        for (int i = 1; i <= n; i++)
            if (degree[i] == 1) {
                leaves.insert(i);
            }

        Graf tree;

        for (int index = 0; index < code.size(); index ++) {
            int currentNode = code[index];

            int leafNode = *leaves.begin();
            leaves.erase(leaves.begin());

            tree.listOfEdges.push_back({leafNode, currentNode});

            degree[leafNode] = degree[leafNode] - 1;
            degree[currentNode] = degree[currentNode] - 1;

            if (degree[currentNode] == 1) {
                leaves.insert(currentNode);
            }
        }

        vector<int> remaining;
        for (int i = 1; i <= n; i++) {
            if (degree[i] == 1) {
                remaining.push_back(i);
            }
        }

        tree.listOfEdges.push_back({remaining[0], remaining[1]});
        tree.m = tree.listOfEdges.size();

        return tree;
    }

    unordered_map<char, int> frequencyString(string& text) {
        unordered_map<char, int> frequency;
        for (int i = 0; i < text.length(); i++) {
            frequency[text[i]]++;
        }
        return frequency;
    }

    struct compare {
        bool operator() (huffmanTreeNode* l, huffmanTreeNode* r) {
            return l->freq > r->freq;
        }
    };

    huffmanTreeNode* huffmanTree(unordered_map<char, int>& frequency) {
        priority_queue<huffmanTreeNode*, vector<huffmanTreeNode*>, compare> pq;
        
        for (auto& pair : frequency) {
            pq.push(new huffmanTreeNode(pair.first, pair.second));
        }

        while (pq.size() > 1) {
            huffmanTreeNode* left = pq.top();
            pq.pop();
            huffmanTreeNode* right = pq.top();
            pq.pop();

            int sum = left->freq + right->freq;
            pq.push(new huffmanTreeNode('\0', sum, left, right));
        }

        huffmanTreeNode* root = pq.top();
        pq.pop();
        return root;
    }

    void printCodes(huffmanTreeNode* root, string str, unordered_map<char, string>& out) {
        if (!root) {
            return;
        }

        if (!root->left && !root->right) {
            out[root->c] = str;
        }

        printCodes(root->left, str + "0", out);
        printCodes(root->right, str + "1", out);
    }

    string huffmanEncode(string& text, huffmanTreeNode* root) {
        unordered_map<char, int> frequency = frequencyString(text);

        root = huffmanTree(frequency);

        unordered_map<char, string> code;
        printCodes(root, "", code);

        string out = "";
        for (char c : text) {
            out += code[c];
        }

        return out;
    }

    string huffmanDecode(string& code, huffmanTreeNode* root) {
        string originalText = "";
        huffmanTreeNode* current = root;

        for (int i = 0; i < code.length(); i++) {
            if (code[i] == '0') {
                current = current->left;
            }
            else {
                current = current->right;
            }

            if (!current->left && !current->right) {
                originalText += current->c;
                current = root;
            }
        }

        return originalText;
    }
};
#endif