#include<random>
#include<iostream>
#include<vector>

using namespace std;

#define MAX_LEVEL 32

template<typename Key_T, typename Mapped_T>
class SkipList {

public:
    class Node {
    public:
	    Key_T key;
        Mapped_T val;
	    vector<Node*> next, prev;
        bool isSentinel;
        int level;

	    Node(Key_T key, Mapped_T val, int level, bool isSentinel = false) {
	        this->key = key;
            this->val = val;
            this->level = level;
            this->isSentinel = isSentinel;
	        next = vector<Node*>(level+1, nullptr);
            prev = vector<Node*>(level+1, nullptr);
            static bool isSeed = false;
            if(!isSeed) {
                srand(time(NULL));
                isSeed = true;
            }
        }      
    };

	SkipList() {
	    level = 0;
        s = 0;
	    sentinel = new Node(0, 0, 0, true);
        sentinel->next[0] = sentinel;
        sentinel->prev[0] = sentinel;
    }

    ~SkipList() {
        Node* current = sentinel->next[0];
        while(!current->isSentinel) {
            Node* next = current->next[0];
            delete current;
            current = next;
        }
        delete sentinel;
    }

    int generateLevel() {
	    int level = 0;
        while(rand() % 2) {
            level++;
        }
        return level;
    }
	
    void insert(Key_T key, Mapped_T val) {
        int lvl = generateLevel();
        int prevLevel = lvl;
        Node* newNode = new Node(key, val, lvl);
        if(lvl > level) {
            (sentinel->next).resize(lvl+1, newNode);
            (sentinel->prev).resize(lvl+1, newNode);
            for(int i = level+1; i <= lvl; i++) {
                newNode->next[i] = sentinel;
                newNode->prev[i] = sentinel;
            }
            prevLevel = level;
            level = lvl;
        }
        Node* current;
        vector<Node*> beforeNewNode(prevLevel+1, nullptr);
        for(int i = prevLevel; i >= 0; i--) {
            if(i == prevLevel) {
                current = sentinel;
            }
            else {
                current = beforeNewNode[i+1];
            }
            while(!(current->next[i]->isSentinel) and current->next[i]->key < key) {
                current = current->next[i];
            }
            beforeNewNode[i] = current;
            newNode->prev[i] = current;
            newNode->next[i] = current->next[i];
            current->next[i]->prev[i] = newNode;
            current->next[i] = newNode;
        }
        s++;
    }

    void erase(Key_T key) {
        Node* node = this->find(key);
        if(node == nullptr) {
            throw out_of_range("Element not in map");
        }
        for(int i = 0; i < node->next.size(); i++) {
            node->prev[i]->next[i] = node->next[i];
            node->next[i]->prev[i] = node->prev[i];    
        }
        delete node;
        Node* current = sentinel;
        for(int i = level; i >= 0; i--) {
            if(current->next[i]->isSentinel) {
                current->next.resize(i);
                level--;
            }
            else {
                break;
            }
        }
    }

    Node* find(Key_T key) {
        if(s == 0) {
            return nullptr;
        }
        Node* current = sentinel;
        int lvl = level;
        while(1) {
            if(lvl == 0) {
                if((current->isSentinel and current->next[lvl]->key > key) or (current->key < key and current->next[lvl]->key > key 
                    and !current->isSentinel) or (current->key < key and current->next[lvl]->isSentinel)) {
                    return nullptr;
                }
            }
            if(current->key == key and !(current->isSentinel)) {
                return current;
            }
            if(current->next[lvl]->key <= key and !(current->next[lvl]->isSentinel)) {
                current = current->next[lvl];
            }
            else {
                lvl--;
            }
        }
    }
	
    void displayList() {
	    cout<<"\n*****Skip List*****"<<"\n";
	    for(int i=0;i<=level;i++) {
		    Node *node = sentinel->next[i];
		    cout << "Level " << i << ": ";
		    while (!(node->isSentinel)) {
			    cout << node->key<<"(" << node->val << ")" << " ";
			    node = node->next[i];
		    }
		    cout << "\n";
	    }
    }

    size_t size() {
        return s;
    }

    void test() {

	    SkipList<int, int> lst;

        lst.insert(25,1);
	    lst.insert(12,2);
	    lst.insert(19,3);
	    lst.insert(17,4);
        lst.insert(3,5);
	    lst.insert(7,6);
	    lst.insert(9,7);
	    lst.insert(26,8);
        lst.insert(6,9);
	    lst.insert(21,10);
	    lst.displayList();
        lst.erase(3);
        lst.displayList();
        /*for(int i = 0; i < 100; i++) {
            Node* node = lst.find(i);
            if(node == nullptr) {
                cout << "NOT FOUND" << endl;
            }
            else {
                cout << "Node with key " << node->key << " was found" << endl;
            }       
        }*/
    }

private:
    int level;
    size_t s;
	Node *sentinel;
};

// Driver to test above code

int main() {
    SkipList<int, int> lst;
    lst.test();
}

