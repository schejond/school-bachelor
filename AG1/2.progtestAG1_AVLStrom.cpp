#include <iostream>

using namespace std;

//class RemovingNodeFromTreeUnhandledCase {};

//class FindingClossestBiggerUnhandledCase {};

class Node {
public:
    Node *parent;
    Node *leftChild;
    Node *rightChild;
    long long int key;

    Node *&chooseCorrectChildPointer() {
        if(this->parent->rightChild == this) {
            return this->parent->rightChild;
        } else {
            return this->parent->leftChild;
        }
    }

    explicit Node(const long long int key)
            : parent(nullptr), leftChild(nullptr), rightChild(nullptr), key(key) {}
};

Node *findNode(Node *currentNode, const long long int key) {
    if (key == currentNode->key) {
        return currentNode;
    }

    if (key < currentNode->key) {
        if (currentNode->leftChild == nullptr) {
            return nullptr;
        }
        return findNode(currentNode->leftChild, key);
    }

    if (currentNode->rightChild == nullptr) {
        return nullptr;
    }
    return findNode(currentNode->rightChild, key);
}

Node *findClosestBiggerKey(Node *currentNode, const long long int key, const bool doSearch, const bool print) {
    //najdu pozici vrcholu s danym klicem
    if (doSearch) {
        currentNode = findNode(currentNode, key);
    }
    //pokud jsem nenasel hodnotu klice
    if (currentNode == nullptr) {
        if (print) {
            cout << "notfound" << endl;
        }
        return nullptr;
    }
    //pokud existuje pravy child
    if (currentNode->rightChild != nullptr) {
        currentNode = currentNode->rightChild;
        Node *tmp = currentNode;
        while(currentNode != nullptr) {
            tmp = currentNode;
            currentNode = currentNode->leftChild;
        }
        return tmp;
    }
    //pokud nemam tatu (jsem root) (a nemam praveho syna)
    if (currentNode->parent == nullptr) {
        if (print) {
            cout << "nosuccessor" << endl;
        }
        return nullptr;
    }
    //nemam praveho syna a jsem levy potomek
    if (currentNode->parent->leftChild == currentNode) {
        return currentNode->parent;
    }
    //nemam praveho syna a jsem pravy potomek
    if (currentNode->parent->rightChild == currentNode) {
        currentNode = currentNode->parent;
        while (currentNode != nullptr) {
            if (currentNode->parent == nullptr) {
                if (print) {
                    cout << "nosuccessor" << endl;
                }
                return nullptr;
            }
            if (currentNode->parent->leftChild == currentNode) {
                return currentNode->parent;
            }
            currentNode = currentNode->parent;
        }
    }
    return nullptr;
    //throw FindingClossestBiggerUnhandledCase();
}

void printParent(Node *currentNode, const long long int key) {
    currentNode = findNode(currentNode, key);
    //node not found
    if (currentNode == nullptr) {
        cout << "notfound" << endl;
        return;
    }
    cout << currentNode->parent->key << endl;
}

/*Node *findMin(Node *currentNode) {

}*/

void removeNodeFromTree(Node *currentNode, const long long int key, Node *&root) {
    currentNode = findNode(currentNode, key);
    //vrchol s danym klicem neexistuje
    if (currentNode == nullptr) {
        return;
    }

    //pokud to je list (nema potomka)
    if (currentNode->rightChild == nullptr && currentNode->leftChild == nullptr) {
        //vymazu info z parenta (pokud existuje) a pak smazu
        if (currentNode->parent != nullptr) {
            currentNode->chooseCorrectChildPointer() = nullptr;
        } else {//tzn currentNode je root a nema potomka -> root bude po smazani null
            root = nullptr;
        }
        delete currentNode;
        return;
    }
    //pokud mam pouze leveho potomka
    if (currentNode->rightChild == nullptr && currentNode->leftChild != nullptr) {
        //pokud mam parenta tak v nem zmenim info
        if (currentNode->parent != nullptr) {
            currentNode->chooseCorrectChildPointer() = currentNode->leftChild;
        } else {//currenNode je root a je treba aby po smazani byl novy root jeho potomek
            root = currentNode->leftChild;
        }
        //presmeruji potomka na noveho spravneho otce
        currentNode->leftChild->parent = currentNode->parent;
        delete currentNode;
        return;
    }
    //pokud mam pouze praveho potomka
    if (currentNode->rightChild != nullptr && currentNode->leftChild == nullptr) {
        //pokud ma parenta tak v nem zmenim info
        if (currentNode->parent != nullptr) {
            currentNode->chooseCorrectChildPointer() = currentNode->rightChild;
        } else {//currenNode je root a je treba aby po smazani byl novy root jeho potomek
            root = currentNode->rightChild;
        }
        //presmeruji potomka na noveho spravneho otce
        currentNode->rightChild->parent = currentNode->parent;
        delete currentNode;
        return;
    }
    //pokud mam oba potomky
    if (currentNode->rightChild != nullptr && currentNode->leftChild != nullptr) {
        Node *closestNodeWithBiggerKey = findClosestBiggerKey(currentNode, key, false, false);
        //prohodim hodnoty klicu
        currentNode->key = closestNodeWithBiggerKey->key;
        //pokud mel closestNodeWithBiggerKey praveho potomka, presmeruji ho na rodice od ClosestNodeWithBiggerKey
        if (closestNodeWithBiggerKey->rightChild != nullptr) {
            closestNodeWithBiggerKey->chooseCorrectChildPointer() = closestNodeWithBiggerKey->rightChild;
            closestNodeWithBiggerKey->rightChild->parent = closestNodeWithBiggerKey->parent;
        } else {
            closestNodeWithBiggerKey->chooseCorrectChildPointer() = nullptr;
        }
        //case pokud muj nejblizsi nasledovnik je muj potomek
        /*if (currentNode->rightChild == closestNodeWithBiggerKey) {
            currentNode->rightChild = closestNodeWithBiggerKey->rightChild;
            //nastavim se jako novy otec
            if (currentNode->rightChild != nullptr) {//zbytecna podminka
                currentNode->rightChild->parent = currentNode;
            }
        } else {
            //closestNodeWithBiggerKey->chooseCorrectChildPointer() = nullptr;
        }*/

        delete closestNodeWithBiggerKey;
        return;
    }
    //return;
    //throw RemovingNodeFromTreeUnhandledCase();
}

void addNodeToTree(Node *currentNode, Node *newNode) {
    if (newNode->key == currentNode->key) {
        //hodnota se zadanym klicem jiz existuje, newNode smazu, protoze byl vytvoren zbytecne
        delete newNode;
        return;
    }
    //value is bigger -> go to the right subtree
    if (newNode->key > currentNode->key) {
        if (currentNode->rightChild == nullptr) {
            currentNode->rightChild = newNode;
            currentNode->rightChild->parent = currentNode;
            return;
        }
        return addNodeToTree(currentNode->rightChild, newNode);
    }
    //go to left subtree
    if (currentNode->leftChild == nullptr) {
        currentNode->leftChild = newNode;
        currentNode->leftChild->parent = currentNode;
        return;
    }
    return addNodeToTree(currentNode->leftChild, newNode);
}

void freeAllNodes(Node *root) {
    if (root->rightChild != nullptr) {
        freeAllNodes(root->rightChild);
    }
    if (root->leftChild != nullptr) {
        freeAllNodes(root->leftChild);
    }
    delete root;
}

void rotateToRightBVS(Node *currentNode, Node *&root) {
    if (currentNode->leftChild == nullptr) {
        cout << "norotate" << endl;
        return;
    }

    //pokud mam parenta tak ho presmeruji na to aby jako potomka mel node nalevo podemnou
    Node *nodeToSwitchWithMe = currentNode->leftChild;
    if (currentNode->parent != nullptr) {
        currentNode->chooseCorrectChildPointer() = nodeToSwitchWithMe;
    } else {//tzn, ze currentNode je root
        root = nodeToSwitchWithMe;
    }
    //nodu nalevo podemnou zmenim otce ze me na myho tatu
    nodeToSwitchWithMe->parent = currentNode->parent;
    //dam si noveho tatu
    currentNode->parent = nodeToSwitchWithMe;
    //nakopiruji praveho syna meho leveho nodu aby byl ted muj levy syn
    currentNode->leftChild = nodeToSwitchWithMe->rightChild;
    //pokud existuje, dam mu info,ze ma noveho tatku
    if (currentNode->leftChild != nullptr) {
        currentNode->leftChild->parent = currentNode;
    }

    //a jemu dam sebe jako praveho potomka
    nodeToSwitchWithMe->rightChild = currentNode;
}

void rotateToLeftBVS(Node *currentNode, Node *&root) {
    if (currentNode->rightChild == nullptr) {
        cout << "norotate" << endl;
        return;
    }

    Node *nodeToSwitchWithMe = currentNode->rightChild;
    if (currentNode->parent != nullptr) {
        currentNode->chooseCorrectChildPointer() = nodeToSwitchWithMe;
    } else {
        root = nodeToSwitchWithMe;
    }

    nodeToSwitchWithMe->parent = currentNode->parent;
    currentNode->parent = nodeToSwitchWithMe;
    currentNode->rightChild = nodeToSwitchWithMe->leftChild;
    if(currentNode->rightChild != nullptr) {
        currentNode->rightChild->parent = currentNode;
    }

    nodeToSwitchWithMe->leftChild = currentNode;
}

int main() {
    int command = -1;
    long long int key;
    int side;

    Node *root = nullptr;
    Node *newNode = nullptr;
    Node *startingNode = nullptr;
    Node *closestNode = nullptr;

    while(command != 6) {
        cin >> command;
        startingNode = root;
        switch (command) {
            case 1:
                cin >> key;
                newNode = new Node(key);
                if (root == nullptr) {
                    root = newNode;
                    break;
                }
                startingNode = root;//zbytecne
                addNodeToTree(startingNode, newNode);
                break;
            case 2:
                cin >> key;
                if (root != nullptr) {
                    removeNodeFromTree(startingNode, key, root);
                }
                break;
            case 3:
                cin >> key;
                //if there is no root
                if (root == nullptr) {
                    cout << "notfound" << endl;
                    break;
                }
                //if it is root -> no parent
                if (root->key == key) {
                    cout << "noparent" << endl;
                    break;
                }
                printParent(startingNode, key);
                break;
            case 4:
                cin >> key;
                //if there is no root
                if (root == nullptr) {
                    cout << "notfound" << endl;
                    break;
                }

                closestNode = findClosestBiggerKey(startingNode, key, true, true);
                //if == nullptr, the result was already printed
                if (closestNode != nullptr) {
                    cout << closestNode->key << endl;
                }
                break;
            case 5:
                cin >> key;
                cin >> side;

                if (root == nullptr) {
                    cout << "notfound" << endl;
                    break;
                }
                startingNode = findNode(startingNode, key);
                if (startingNode == nullptr) {
                    cout << "notfound" << endl;
                    break;
                }
                if (side == 1) {
                    rotateToLeftBVS(startingNode, root);
                } else {
                    rotateToRightBVS(startingNode, root);
                }
                break;
            case 9:
                /*if (root != nullptr) {
                    cout << "root ma hodnotu klice: " << root->key << endl;
                } else {
                    cout << "root je null" << endl;
                }*/
                break;
            default:
                break;
        }
    }

    //dealokace dat
    if (root != nullptr) {
        freeAllNodes(root);
    }

    return 0;
}