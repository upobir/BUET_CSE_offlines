#include <iostream>
#include <cassert>
#include <queue>
using namespace std;

class RedBlackTree{
private:
    enum nodeColor { RED, BLACK };
    class Node{
    public:
        int value;
        Node *left, *right, *parent;
        nodeColor color;

        Node(int _value): value(_value), left(nullptr), right(nullptr), parent(nullptr), color(RED){}
    } *root, *nil;

    void setChild(Node * P, Node * C, bool left){
        if(C != nullptr)  C->parent = P;
        if(P != nullptr){
            if(left) P->left = C;
            else     P->right = C;
        }
    }

    nodeColor getColor(Node * node){
        if(node == nullptr) return BLACK;
        return node->color;
    }

    void deleteTree(Node *node){
        if(node == nullptr) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

    string getTreeString(Node *node){
        if(node == nullptr) return "";
        string nodeString = to_string(node->value) + ":" + (node->color == RED? "r" : "b");
        if(node->left == nullptr && node->right == nullptr) return nodeString;
        return nodeString + "(" + getTreeString(node->left) + ")(" + getTreeString(node->right) + ")";
    }

    Node * getClosest(Node *subtree, int x){
        while(true){
            if(subtree->value == x){
                break;
            }
            else if(x < subtree->value){
                if(subtree->left == nullptr) break;
                else                         subtree = subtree->left;
            }
            else{
                if(subtree->right == nullptr) break;
                else                          subtree = subtree->right;
            }
        }
        return subtree;
    }

    ///left rotate->right child should be non-null
    ///   A
    ///    \
    ///     B
    ///right rotate->left child should be non-null
    ///   A
    ///  /
    /// B

    void rotate(Node *A, bool left){
        Node* B = (left)? A->right : A->left;

        if(left) setChild(A, B->left, false);
        else     setChild(A, B->right, true);

        if(A == root) root = B, B->parent = nullptr;
        else          setChild(A->parent, B, A->parent->left == A);

        setChild(B, A, left);
    }

    void insertFixup(Node * node){
        while(node != root && node->parent->color == RED){
            Node * grandparent = node->parent->parent;
            Node * uncle = (grandparent->left == node->parent)? grandparent->right : grandparent->left;

            if(getColor(uncle) == RED){
                node->parent->color = uncle->color = BLACK;
                uncle->parent->color = RED;
                node = uncle->parent;
            }
            else{
                if((grandparent->left == node->parent) != (node->parent->left == node)){
                    Node * temp = node->parent;
                    rotate(node->parent, !(node->parent->left == node));
                    node = temp;
                }

                rotate(grandparent, !(node->parent->left == node));
                grandparent->color = RED;
                grandparent->parent->color = BLACK;
                break;
            }
        }
        if(node == root) node->color = BLACK;
    }

    void setupNil(Node * node){
        node->right = nil;
        nil->parent = node;
    }

    void removeFixup(Node *node){
        while(node != root && node->color == BLACK){
            Node *sibling = (node->parent->left == node)? node->parent->right : node->parent->left;
            if(getColor(sibling) == RED){
                rotate(node->parent, node->parent->left == node);
                node->parent->color = RED;
                node->parent->parent->color = BLACK;
            }
            else if(getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) {
                sibling->color = RED;
                node = node->parent;
            }
            else if((node->parent->left == node && getColor(sibling->right) == BLACK) ||
                    (node->parent->right == node && getColor(sibling->left) == BLACK)){
                sibling->color = RED;
                if(node->parent->left == node) sibling->left->color = BLACK;
                else                           sibling->right->color = BLACK;
                rotate(sibling, node->parent->right == node);
            }
            else{
                nodeColor temp = node->parent->color;
                rotate(node->parent, node->parent->left == node);
                sibling->color = temp;
                sibling->left->color = BLACK;
                sibling->right->color = BLACK;
                break;
            }
        }
        if(node->color == RED)
            node->color = BLACK;
    }

public:
    RedBlackTree(): root(nullptr) {
        nil = new Node(-1000000000);
        nil->color = BLACK;
    }

    bool insert(int x){
        Node* newNode = new Node(x);
        if(root == nullptr){
            (root = newNode)->color = BLACK;
            return true;
        }

        Node* node = getClosest(root, x);
        while(node->value == x && node->right != nullptr){
            node = getClosest(node->right, x);
        }


//        if(node->value == x)
//            return false;
//        else
        setChild(node, newNode, (x < node->value));
        insertFixup(newNode);
        return true;
    }

    bool find(int x){
        if(root == nullptr) return false;

        Node* node = getClosest(root, x);
        return (node->value == x);
    }

    bool remove(int x){
        if(root == nullptr) return false;

        Node *node = getClosest(root, x);
        if(node->value != x) return false;

        Node *replacement, *doubleColored;
        nodeColor extra = node->color;

        if(node->left == nullptr){
            if(node->right == nullptr) setupNil(node);
            doubleColored = replacement = node->right;
        }
        else if(node->right == nullptr){
            doubleColored = replacement = node->left;
        }
        else {
            replacement = getClosest(node->right, x-1);
            extra = replacement->color;
            replacement->color = node->color;
            if(replacement->right == nullptr) setupNil(replacement);

            doubleColored = replacement->right;
            setChild(replacement->parent, replacement->right, replacement->parent->left == replacement);
            setChild(replacement, node->left, true);
            setChild(replacement, node->right, false);
        }

        if(node == root) {
            root = replacement;
            setChild(nullptr, replacement, true);
        }
        else
            setChild(node->parent, replacement, node->parent->left == node);

        delete node;
        if(extra == BLACK){
            removeFixup(doubleColored);
        }
        if(nil->parent != nullptr){
            if(nil->parent->left == nil) nil->parent->left = nullptr;
            else nil->parent->right = nullptr;
            nil->parent = nullptr;
        }
        if(nil == root) root = nullptr;
        return true;
    }

    string getTreeString(){ return getTreeString(root); }

    ~RedBlackTree(){ deleteTree(root); }

//    void debug(){
//        if(root == nullptr){
//            cerr<<endl;
//            return;
//        }
//        queue<int> qL;
//        queue<Node*> qN;
//        int cur = 0;
//        qN.push(root); qL.push(0);
//        string line(70, ' ');
//        while(!qN.empty()){
//            Node* node = qN.front(); int level = qL.front();
//            qN.pop(); qL.pop();
//
//            if(level != cur){
//                cerr<<line<<endl;
//                cur = level;
//                line = string(70, ' ');
//            }
//
//            string part = to_string(node->value)+":";
//            part.push_back((node->color == RED?char(219):char(176)));
//            line.replace(node->value*4, part.size(), part);
//
//            if(node->left != nullptr){
//                for(int i = node->value*4-1; i>node->left->value*4; i--)
//                    line[i] = 196;
//                line[node->left->value*4] = 218;
//                qN.push(node->left);
//                qL.push(level+1);
//            }
//
//            if(node->right != nullptr){
//                for(int i = node->value*4+4; i<node->right->value*4; i++)
//                    line[i] = 196;
//                line[node->right->value*4] = 191;
//                qN.push(node->right);
//                qL.push(level+1);
//            }
//
//        }
//        cerr<<line<<endl;
//    }
};

int main(){
    string type;
    int value;
    RedBlackTree tree;

    while(cin>>type>>value){
        if(type == "F"){
            if(tree.find(value))
                cout<<"True"<<endl;
            else
                cout<<"False"<<endl;
        }
        else if(type == "I"){
            tree.insert(value);
            cout<<tree.getTreeString()<<endl;
//            tree.debug();
        }
        else if(type == "D"){
            tree.remove(value);
            cout<<tree.getTreeString()<<endl;
//            tree.debug();
        }
        else{
            assert("invalid command");
        }
    }
}

/*
sample:
F 1
I 1
I 3
I 2
I 5
I 7
I 4
I 6
I 8
I 10
D 7
D 1
D 8
F 2

I 7 I 3 I 9 I 1 I 2
*/
