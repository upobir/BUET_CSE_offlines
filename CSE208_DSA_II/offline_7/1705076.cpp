#include <iostream>
#include <string>
#include <queue>
#include <sstream>
using namespace std;

class Heap{
private:
    class Node{
    public:
        int key, degree;
        Node *sibling, *fChild, *lChild;

        Node(int x){
            key = x;
            degree = 0;
            sibling = fChild = lChild = nullptr;
        }

        void deleteNode(){
            if(sibling != nullptr){
                sibling->deleteNode();
                delete sibling;
            }
            if(fChild != nullptr){
                fChild->deleteNode();
                delete fChild;
            }
            return;
        }
    };

    Node *root;

    Node* merge(Node* a, Node* b){
        if(a->key > b->key)
            swap(a, b);
        if(a->fChild == nullptr){
            a->fChild = a->lChild = b;
        }
        else{
            a->lChild->sibling = b;
            a->lChild = b;
        }
        a->degree++;
        return a;
    }

    Node* progress(Node* a){
        Node * tmp = a->sibling;
        a->sibling = nullptr;
        return tmp;
    }

public:
    Heap(){
        root = nullptr;
    }

    int findMin(){
        if(root == nullptr){
            cout<<"invalid operation"<<endl;
            return -1;
        }
        Node * minNode = root;

        for(Node * cur = root; cur != nullptr; cur = cur->sibling){
            if(cur->key < minNode->key)
                minNode = cur;
        }

        return minNode->key;
    }

    int extractMin(){
        if(root == nullptr){
            cout<<"invalid operation"<<endl;
            return -1;
        }
        int minKey = findMin();
        Node * minNode;

        if(root->key == minKey){
            minNode = root;
            root = minNode->sibling;
        }
        else{
            Node * cur = root;
            while(true){
                if(cur->sibling->key == minKey){
                    minNode = cur->sibling;
                    cur->sibling = minNode->sibling;
                    break;
                }
                cur = cur->sibling;
            }
        }

        if(minNode->fChild != nullptr){
            Heap H;
            H.root = minNode->fChild;
            this->unite(H);
        }


        delete minNode;
        return minKey;
    }

    void insert(int x){
        if(root == nullptr){
            root = new Node(x);
            return;
        }

        Heap H;
        H.insert(x);
        this->unite(H);
    }

    void unite(Heap &H){
        if(root == nullptr){
            root = H.root;
            H.root = nullptr;
            return;
        }
        else if(H.root == nullptr){
            return;
        }

        Node *cur1 = root, *cur2 = H.root, *cur = nullptr, *carry = nullptr;
        root = nullptr;
        H.root = nullptr;

        while(cur1 != nullptr || cur2 != nullptr || carry != nullptr){
            if(carry != nullptr){
                if(cur1 != nullptr && cur1->degree == carry->degree){
                    carry = merge(cur1, carry);
                    cur1 = progress(cur1);
                    if(cur2 != nullptr && cur2->degree < carry->degree){
                        if(cur == nullptr)
                            cur = root = cur2;
                        else
                            cur = (cur->sibling = cur2);
                        cur2 = progress(cur2);
                    }
                }
                else if(cur2 != nullptr && cur2->degree == carry->degree){
                    carry = merge(cur2, carry);
                    cur2 = progress(cur2);
                    if(cur1 != nullptr && cur1->degree < carry->degree){
                        if(cur == nullptr)
                            cur = root = cur1;
                        else
                            cur = (cur->sibling = cur1);
                        cur1 = progress(cur1);
                    }
                }
                else{
                    if(cur == nullptr)
                        cur = root = carry;
                    else
                        cur = (cur->sibling = carry);
                    carry = nullptr;
                }
            }
            else{
                if(cur1 != nullptr && cur2 != nullptr && cur1->degree == cur2->degree){
                    carry = merge(cur1, cur2);
                    cur1 = progress(cur1);
                    cur2 = progress(cur2);
                }
                else{
                    Node *tmp = (cur1 != nullptr && (cur2 == nullptr || cur2->degree > cur1->degree))? cur1 : cur2;
                    if(cur == nullptr)
                        cur = root = tmp;
                    else
                        cur = (cur->sibling = tmp);
                    if(tmp == cur1)
                        cur1 = progress(cur1);
                    else
                        cur2 = progress(cur2);
                }
            }
        }
    }


    void print(){
        for(Node * cur = root; cur != nullptr; cur = cur->sibling){
            cout<<"Binomial Tree, B"<<cur->degree;
            queue<Node*> qN;   queue<int> qL;
            qN.push(cur);  qL.push(0);
            int curLevel = -1;

            while(!qN.empty()){
                if(qL.front() != curLevel)
                    cout<<endl<<"Level "<<(curLevel = qL.front())<<" : ";
                cout<<qN.front()->key<<" ";

                for(Node *child = qN.front()->fChild; child != nullptr; child = child->sibling){
                    qN.push(child);
                    qL.push(qL.front()+1);
                }

                qN.pop();
                qL.pop();
            }
            cout<<endl;
        }
    }

    ~Heap(){
        if(root != nullptr){
            root->deleteNode();
            delete root;
        }
    }
};

int main(){
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    string type;
    Heap H;
    while(cin>>type){
        switch(type[0]){
        case 'F' :
            {
                int x = H.findMin();
                cout<<"Find-Min returned "<<x<<endl;
                break;
            }
        case 'E' :
            {
                int x = H.extractMin();
                cout<<"Extract-Min returned "<<x<<endl;
                break;
            }
        case 'I' :
            {
                int x;
                cin>>x;
                H.insert(x);
                break;
            }
        case 'U' :
            {
                string s;
                getline(cin, s);
                stringstream ss(s);
                int x;
                Heap H2;
                while(ss >> x){
                    H2.insert(x);
                }
                H.unite(H2);

                break;
            }
        case 'P' :
            {
                H.print();
                break;
            }
        default:
            {
                cout<<"invalid command"<<endl;
                break;
            }
        }
    }
}
/*
I 5
I 10
I 4
I 1
I 11
I 3
I 12
I 7
I 8
I 9
I 2
I 6
*/
