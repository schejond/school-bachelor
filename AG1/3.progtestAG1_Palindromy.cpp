#include <iostream>
#include <vector>
//#include <map>
#include <stdlib.h>

#define lli long long int

using namespace std;

class Graph {
public:
    vector<vector<pair<char, lli>>> forwardPaths;
    vector<vector<pair<char, lli>>> backwardsPaths;

    lli startingNode;
    lli endingNode;

//    map<pair<int, int>, int> coordinatesToNmbOfPalindromicPathsBetweenThem;
    vector<vector<int>> coordinatesToNmbOfPalindromicPathsBetweenThem;

    explicit Graph(const lli nodesCnt, const lli startingNode, const lli endingNode)
            : forwardPaths(nodesCnt), backwardsPaths(nodesCnt), startingNode(startingNode), endingNode(endingNode)
             , coordinatesToNmbOfPalindromicPathsBetweenThem(nodesCnt) {
        for (lli i = 0 ; i < nodesCnt ; i++) {
            coordinatesToNmbOfPalindromicPathsBetweenThem[i].resize(nodesCnt);
            for (lli j = 0 ; j < nodesCnt ; j++) {
                coordinatesToNmbOfPalindromicPathsBetweenThem[i][j] = -1;
            }
        }
//        forwardPaths.resize(nodesCnt);
//        backwardsPaths.resize(nodesCnt);
    }


    void addPath(const lli from, const lli to, const char tone) {
        forwardPaths[from].emplace_back(make_pair(tone, to));//todo or push_back
        backwardsPaths[to].emplace_back(make_pair(tone, from));
    }

    lli cntPalindromicPaths(const lli & forwardNode, const lli & backwardNode, const char fTone, const char bTone) {
//        cout << "aktualni node zpredu: " << forwardNode << endl;
//        cout << "aktualni node zezadu: " << backwardNode << endl;

        //pokud sem se sem nedostal stejnym tonem vratim 0
        if (fTone != bTone) {
//            cout << "vracim 0" << endl;
            return 0;
        }

        //pokud je to ten samy, vrat 1
        if (forwardNode == backwardNode) {
//            cout << "vracim 1" << endl;
            return 1;
        }

//        cout << "pokracuji dal v rekurzi -> neukoncil jsem se" << endl;

        //todo pokud je predpocitana cesta pro (zeStartuIndex, zKonceIndex) vrat ho
        if (coordinatesToNmbOfPalindromicPathsBetweenThem[forwardNode][backwardNode] != -1) {
            return coordinatesToNmbOfPalindromicPathsBetweenThem[forwardNode][backwardNode];
        }

        lli sum = 0;
        for (auto nextFNode : forwardPaths[forwardNode]) {
//            cout << "z aktualniho nodu se dopredu dostanu do: " << nextFNode.second << endl;
            for (auto nextBNode : backwardsPaths[backwardNode]) {
//                cout << "z aktualniho nodu se dozadu dostanu do: " << nextBNode.second << endl;
                //pokud jsou vedle sebe
                if (nextBNode.second == forwardNode && nextFNode.second == backwardNode) {
//                    cout << "davam sum + 1" << endl;
                    sum += 1;
                    sum = sum%1000000007;
                    continue;
                }
//                cout << "volam zanoreni pro: " << nextFNode.second << " a " << nextBNode.second << endl;
                sum += cntPalindromicPaths(nextFNode.second, nextBNode.second, nextFNode.first, nextBNode.first);
                sum = sum%1000000007;
            }
        }
        coordinatesToNmbOfPalindromicPathsBetweenThem[forwardNode][backwardNode] = sum;

        return sum;
    }

    void printForwardPaths() {
        cout << "PRINTING FORWARD PATHS!" << endl;
        for (unsigned lli i = 0 ; i < forwardPaths.size() ; i++) {
            cout << "from node with number: " << i << " :" << endl;
            for (unsigned lli j = 0 ; j < forwardPaths[i].size() ; j++) {
                cout << "path to: " << forwardPaths[i][j].second << " with tone: " << forwardPaths[i][j].first << endl;
            }
            cout << "------------------------------" << endl;
        }
    }

    void printBackwardPaths() {
        cout << "PRINTING BACKWARD PATHS!" << endl;
        for (unsigned lli i = 0 ; i < backwardsPaths.size() ; i++) {
            cout << "from node with number: " << i << " :" << endl;
            for (unsigned lli j = 0 ; j < backwardsPaths[i].size() ; j++) {
                cout << "path to: " << backwardsPaths[i][j].second << " with tone: " << backwardsPaths[i][j].first << endl;
            }
            cout << "------------------------------" << endl;
        }
    }
};

int main() {
    lli nodeCnt, k;//k je pocet nodu ktere se mohou lisit
    lli startingNode, endingNode;
    vector<vector<pair<char, lli>>> forwardPaths;
    vector<vector<pair<char, lli>>> backwardsPaths;

    cin >> nodeCnt >> k;
    cin >> startingNode >> endingNode;
    Graph g(nodeCnt, startingNode, endingNode);
    for (lli i = 0 ; i < nodeCnt ; i++) {
        lli nmbOfNeighbours;
        cin >> nmbOfNeighbours;
        if (nmbOfNeighbours == 0) {
            continue;
        }
        for (lli j = 0 ; j < nmbOfNeighbours ; j++) {
            lli neighbourCityIndex;
            char tone;
            cin >> neighbourCityIndex >> tone;
            g.addPath(i, neighbourCityIndex, tone);
        }
    }

    const lli res = g.cntPalindromicPaths(startingNode, endingNode, 'z', 'z');
    cout << res%1000000007 << endl;



//    g.printForwardPaths();
//
//    cout << endl;
//
//    g.printBackwardPaths();
    return 0;
}