// g++ teszt.cpp -static-libstdc++ -static-libgcc -o p
#include <iostream>
#include "grafmodul.h"

using namespace std;

void tesztAlapMuveletek(Graf& g) {
    cout << "<--- ALAP MUVELETEK TESZTELESE --->\n";

    g.writeListOfEdges();
    g.writeAdjacencyList();
    g.writeIncidenceMatrix();
    g.writeAdjacencyMatrix();

    cout << "\nIzolalt pontok: ";
    g.isolatedNodes();
    
    cout << "Vegpontok: ";
    g.endpoints();

    cout << "\nOsszefuggo: " << (g.isConnected() ? "igen" : "nem") << '\n';

    int k = 2;
    node csomopont = {1};
    cout << csomopont.value << " csomopont " << k << " rendu ismerosei: ";
    vector<int> seged = g.kthDegreeAcquaintance(csomopont, k);
    g.writeArray(seged);
}

void tesztIranyitatlanGraf(Graf& g) {
    cout << "\n<--- IRANYITATLAN GRAF TESZTELESE --->\n";
    
    try {
        node csomopont = {1};
        cout << "1-es csomopont fokszama: " << g.degree(csomopont) << '\n';
        
        csomopont = {6};
        cout << "3-as csomopont fokszama: " << g.degree(csomopont) << '\n';

        cout << "\nBFS bejaras " << csomopont.value << " csomopontbol: ";
        g.BFS(csomopont);
        cout << "\nDFS bejaras " << csomopont.value << " csomopontbol: ";
        g.DFS(csomopont);
        cout << '\n';

        int k = 1;
        vector<int> k_rendu = g.kthDegreeAcquaintance(csomopont, k);
        cout << csomopont.value << " csomopont " << k << "-rendu ismerosei: ";
        for (int x : k_rendu) {
            cout << x << " ";
        }
        cout << '\n';

        k = 2;
        k_rendu = g.kthDegreeAcquaintance(csomopont, k);
        cout << csomopont.value << " csomopont " << k << "-rendu ismerosei: ";
        for (int x : k_rendu) {
            cout << x << " ";
        }
        cout << '\n';
        
    } catch (const HibasTipus& e) {
        cout << "HibasTipus kivetel: " << e.what() << '\n';
    }
}

void tesztIranyitottGraf(Graf& g) {
    cout << "\n<--- IRANYITOTT GRAF TESZTELESE --->\n";
    
    try {
        node csomopont = {1};
        cout << csomopont.value << " csomopont kifokszama: " << g.outDegree(csomopont) << '\n';
        cout << csomopont.value << " csomopont befokszama: " << g.inDegree(csomopont) << '\n';
        
        csomopont = {3};
        cout << csomopont.value << " csomopont kifokszama: " << g.outDegree(csomopont) << '\n';
        cout << csomopont.value << " csomopont befokszama: " << g.inDegree(csomopont) << '\n';

        try {
            cout << "Van-e kor: " << (g.circleCheck() ? "igen" : "nem") << '\n';
        } catch (const NemOsszefuggo& e) {
            cout << "Nem lehet kor-t vizsgalni: " << e.what() << '\n';
        }

        if (g.getDirection() && !g.circleCheck()) {
            try {
                vector<int> topo = g.topologicalOrder();
                cout << "Topologiai sorrend: ";
                for (int x : topo) {
                    cout << x << " ";
                }
                cout << '\n';
            } catch (const NemKormentes& e) {
                cout << "Nem kormentes: " << e.what() << '\n';
            }
        }

        try {
            vector<vector<int>> komponensek = g.strongConnect();
            cout << "\nErosen osszefuggo komponensek:" << '\n';
            for (int i = 0; i < komponensek.size(); i++) {
                cout << "Komponens " << i + 1 << ": ";
                for (int x : komponensek[i]) {
                    cout << x << " ";
                }
                cout << '\n';
            }
        } catch (const HibasTipus& e) {
            cout << "Nem iranyitott graf: " << e.what() << '\n';
        }
        
    } catch (const HibasTipus& e) {
        cout << "HibasTipus kivetel: " << e.what() << '\n';
    }
}

void tesztSulyozottGraf(Graf& g) {
    cout << "\n<--- SULYOZOTT GRAF TESZTELESE --->\n";

    try {
        node csomopont = {6};
        if (!g.getDirection()) {
            Graf feszitofa = g.kruskal();
            cout << "\nKruskal-algoritmusa (feszitofa):\n";
            feszitofa.writeListOfEdges();

            feszitofa = g.prim();
            cout << "\nPrim-algoritmusa (feszitofa):\n";
            feszitofa.writeListOfEdges();
        }
        else
        {
            cout << "\nBellman-Ford algoritmus: Legrovidebb utak tavolsaga a " << csomopont.value << " csomopontbol:\n";
            vector<int> tavolsagok = g.bellmanFord(csomopont);
            for(int i = 1; i <= g.getNumberOfNodes(); i++) {
                cout << i << " csomopontba: " << tavolsagok[i] << '\n';
            }

            cout << "\nJohnson-algoritmusa: Osszes csomopontbol osszes csomopontba legrovidebb utak:\n";
            vector<vector<vector<int>>> utak = g.johnson();
            for (int i = 1; i <= g.getNumberOfNodes(); i++) {
                for (int j = 1; j <= g.getNumberOfNodes(); j++) {
                    cout << "Kezdopont: " << i << ", Vegpont: " << j << "\nUt: ";
                    for (int k = 0; k < utak[i][j].size(); k++) {
                        cout << utak[i][j][k];
                        cout << ", ";
                    }
                    cout << '\n';
                }
            }
        }

        cout << "\nDijkstra-algoritmusa: Utak a " << csomopont.value << " csomopontbol:\n";
        vector<vector<int>> dijkstra_utak = g.dijkstra(csomopont);
        for (int i = 1; i <= g.getNumberOfNodes(); i++) {
            cout << i << " csomopontba: ";
            for(int j = 0; j < dijkstra_utak[i].size(); j++) {
                cout << dijkstra_utak[i][j];
                cout << ", ";
            }
            cout << "(" << g.pathReconstruction(csomopont, dijkstra_utak[i]) << ")\n";
        }

        cout << "\nRoy-Floyd-Warshall algoritmus: Tavolsagi matrix:\n";
        vector<vector<int>> tavolsagi = g.royFloydWarshall();
        for (int i = 1; i <= g.getNumberOfNodes(); i++) {
            for (int j = 1; j <= g.getNumberOfNodes(); j++) {
                cout << tavolsagi[i][j] << " ";
            }
            cout << '\n';
        }
    }
    catch (const HibasTipus& e) {
        cout << "Nem iranyitott graf: " << e.what() << '\n';
    }
    catch (const NegativEl& e) {
        cout << "Negativ el: " << e.what() << '\n';
    }
}

void tesztFa(Graf& g) {
    cout << "\n<--- FA TESZTELESE --->\n";

    try {
        vector<int> code = g.prufferEncode();
        cout << "Pruffer kodolas: ";
        g.writeArray(code);
        Graf tree = g.prufferDecode(code);
        cout << "Dekodolas:\n";
        tree.writeListOfEdges();
        cout << '\n';
        cout << "Atmero: " << g.diameter() << "\n\n";
        cout << "Kozeppont: ";
        vector<node> center = g.center();
        for(int i = 0; i < center.size(); i++) {
            cout << center[i].value << " ";
        }
        cout << "\n\n";

        if (g.getWeighted() && !g.getDirection()) {
            cout << "\nBoruvka MST\n";
            Graf mst = g.boruvka();
            mst.writeListOfEdges();
        }
    } catch (const NemFa& e) {
        cout << e.what();
    }
}

int main() {
    cout << "GRAF MODUL TESZT PROGRAM" << '\n';
    cout << "------------------------" << '\n';

    try {
        Graf g;
        g = g.reading("input.txt");
        tesztAlapMuveletek(g);
        if (!g.getDirection()) {
            tesztIranyitatlanGraf(g);
        }
        else {
            tesztIranyitottGraf(g);
        }
        if (g.getWeighted()) {
            tesztSulyozottGraf(g);
        }
        if (g.isTree()) {
            tesztFa(g);
        }

        // huffman kodolas
        string text = "alma a fa alatt";
        cout << "\n\nHuffman kodolas: " << text << "\n";
        unordered_map<char, int> frequency = g.frequencyString(text);
        huffmanTreeNode* tree = g.huffmanTree(frequency);
        string coded = g.huffmanEncode(text, tree);
        string decoded = g.huffmanDecode(coded, tree);
        cout << "Kodolt: " << coded;
        cout << "\nDekodolt: " << decoded;
        cout << "\n\n";

    } catch (const FajlMegnyitas& e) {
        cout << e.what();
    } catch (const HibasTipus& e) {
        cout << e.what();
    } catch (const NemKormentes& e) {
        cout << e.what();
    } catch (const NemOsszefuggo& e) {
        cout << e.what();
    } catch (const NegativEl& e) {
        cout << e.what();
    } catch (const NemFa& e) {
        cout << e.what();
    }

    cout << "\n<--- TESZTELES VEGE --->";
    return 0;
}