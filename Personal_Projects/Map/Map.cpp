#ifndef CS440_540_ASSIGNMENT02_MAP_HPP
#define CS440_540_ASSIGNMENT02_MAP_HPP

#include<random>
#include<iostream>
#include<vector>
#include<utility>
#include<initializer_list>

namespace cs540 {

    #define MAX_LEVEL 32;
    
    template<typename Key_T, typename Mapped_T>
    class Map {
    private:
        typedef std::pair<const Key_T, Mapped_T> ValueType;        

        class SkipList {
        public:
            class Node {
            public:
	            const Key_T key;
                Mapped_T val;
	            std::vector<Node*> next, prev;
                bool isSentinel;
                int level;
                ValueType vals;

	            Node(Key_T key_, Mapped_T val, int level, bool isSentinel = false) : key(key_), vals(ValueType(key_, val)) {
                    this->val = val;
                    this->level = level;
                    this->isSentinel = isSentinel;
	                next = std::vector<Node*>(level+1, nullptr);
                    prev = std::vector<Node*>(level+1, nullptr);
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
	            sentinel = new Node(Key_T(), Mapped_T(), 0, true);
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
	
            Node* insert(const Key_T key, Mapped_T val) {
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
                std::vector<Node*> beforeNewNode(prevLevel+1, nullptr);
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
                return newNode;
            }

            void erase(const Key_T key) {
                Node* node = this->find(key);
                if(node == nullptr) {
                    throw std::out_of_range("Element not in map");
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
                s--;
            }

            Node* find(const Key_T key) {
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

            size_t size() {
                return s;
            }

            Node* getSentinel() {
                return sentinel;
            }

            void displayList() {
	            std::cout<<"\n*****Skip List*****"<<"\n";
	            for(int i=0;i<=level;i++) {
		            Node *node = sentinel->next[i];
		            std::cout << "Level " << i << ": ";
		            while (!(node->isSentinel)) {
			            std::cout << node->key<<"(" << node->val << ")" << " ";
			            node = node->next[i];
		            }
		            std::cout << "\n";
	            }
            }

        private:
            int level;
            size_t s;
	        Node *sentinel;
        };

    public:
        class Iterator;
        class ConstIterator;
        class ReverseIterator;

        Map() {
            list = new SkipList();
        }
        
        Map(const Map &map) : list(new SkipList) {
            for(auto i = map.begin(); i != map.end(); i++) {
                ValueType val = *i;
                list->insert(val.first, val.second); 
            }
        }

        Map &operator=(const Map &map) {
            for(auto i = map.begin(); i != map.end(); i++) {
                ValueType val = *i;
                list->insert(val.first, val.second); 
            }
            return *this;
        }

        ~Map() {
            delete list;
        }

        size_t size() const {
            return list->size();
        }

        bool empty() const {
            return !size();
        }

        Iterator begin() {
            Iterator it(list->getSentinel()->next[0]);
            return it;
        }

        Iterator end() {
            Iterator it(list->getSentinel());
            return it;
        }

        ConstIterator begin() const {
            ConstIterator it(list->getSentinel()->next[0]);
            return it;
        }

        ConstIterator end() const {
            ConstIterator it(list->getSentinel());
            return it;
        }

        ReverseIterator rbegin() {
            ReverseIterator it(list->getSentinel()->next[0]);
            return it;
        }
        
        ReverseIterator rend() {
            ReverseIterator it(list->getSentinel());
            return it;
        }

        Iterator find(const Key_T &key) {
            typename SkipList::Node* ptr = list->find(key);
            if(ptr == nullptr) {
                return end();
            }
            return Iterator(ptr);
        }

        ConstIterator find(const Key_T &key) const {
            typename SkipList::Node* ptr = list->find(key);
            if(ptr == nullptr) {
                return end();
            }
            return ConstIterator(ptr);
        }

        Mapped_T &at(const Key_T &key) {
            typename SkipList::Node* ptr = list->find(key);
            if(ptr == nullptr) {
                throw std::out_of_range("Element not in map");
            }
            return ptr->val;
        }

        const Mapped_T &at(const Key_T &key) const {
            typename SkipList::Node* ptr = list->find(key);
            if(ptr == nullptr) {
                throw std::out_of_range("Element not in map");
            }
            return ptr->val;
        }

        Mapped_T &operator[](const Key_T &key) {
            typename SkipList::Node* ptr = list->find(key);
            if(ptr == nullptr) {
                ptr = list->insert(key, Mapped_T());
                return ptr->val;
            }
            return ptr->val;
        }

        std::pair<Iterator, bool> insert(const ValueType &values) {
            const Key_T key = values.first;
            Mapped_T val = values.second;
            typename SkipList::Node* ptr = list->find(key);
            if(ptr == nullptr) {
                return std::pair<Iterator, bool>(Iterator(list->insert(key, val)), true);
            }
            return std::pair<Iterator, bool>(Iterator(ptr), false);
        }

        template <typename IT_T>
        void insert(IT_T range_beg, IT_T range_end) {
            for(IT_T i = range_beg; i != range_end; i++) {
                insert(*i);
            }
        }

        void erase(Iterator pos) {
            list->erase(pos.ptr->key);
        }

        void erase(const Key_T &key) {
            list->erase(key);
        }

        void clear() {
            delete list;
            list = new SkipList;
        }

        friend bool operator==(const Map &m1, const Map &m2) {
            if(m1.size() != m2.size()) {
                return false;
            }
            auto it1 = m1.begin();
            auto it2 = m2.begin();
            while(it1 != m1.end()) {
                if(*it1 != *it2) {
                    
                    return false;
                }
                it1++;
                it2++;
            }
            return true;
        }

        friend bool operator!=(const Map &m1, const Map &m2) {
            return !(m1 == m2);
        }        
          
        friend bool operator<(const Map &m1, const Map &m2) {
            int longer = 1;
            auto it1 = m1.begin();
            auto it2 = m2.begin();
            if(m2.size() > m1.size()) {
                longer = 2;
            }
            if(longer == 1) {
                while(it2 != m2.end()) {
                    if(*it1 < *it2) {
                        return true;
                    }
                    else if(*it1 > *it2) {
                        return false;
                    }
                    *it1++;
                    *it2++;
                }
                return false;
            }
            else {
                while(it1 != m1.end()) {
                    if(*it1 < *it2) {
                        return true;
                    }
                    else if(*it1 > *it2) {
                        return false;
                    }
                    *it1++;
                    *it2++;
                }
                return true;
            }
        }  

        void displayList() {
            list->displayList();
        }

        class Iterator {        
        public:
            typename SkipList::Node* ptr;

            Iterator() = delete;

            Iterator(typename SkipList::Node* ptr) {
                this->ptr = ptr;
            }

            Iterator &operator++() {
                ptr = ptr->next[0];
                return *this;
            }    

            Iterator operator++(int) {
                Iterator retval = *this;
                ++(*this);
                return retval;
            }

            Iterator &operator--() {
                ptr = ptr->prev[0];
                return *this;
            }    

            Iterator &operator--(int) {
                Iterator retval = *this;
                --(*this);
                return retval;
            } 

            ValueType &operator*() const {
                return ptr->vals;
            }

            ValueType *operator->() const {
                return &(ptr->vals);
            }  

            friend bool operator==(const Iterator &i1, const Iterator &i2) {
                return i1.ptr == i2.ptr;
            }

            friend bool operator==(const Iterator &i1, const ConstIterator &i2) {
                return i1.ptr == i2.ptr; 
            }

            friend bool operator!=(const Iterator &i1, const Iterator &i2) {
                return !(i1 == i2);
            }

            friend bool operator!=(const Iterator &i1, const ConstIterator &i2) {
                return !(i1 == i2);
            }
        };

        class ConstIterator {      
        public:
            typename SkipList::Node* ptr;

            ConstIterator() = delete;

            ConstIterator(typename SkipList::Node* ptr) {
                this->ptr = ptr;
            }

            ConstIterator(const Iterator &it) {
                this->ptr = it->ptr;    
            }            

            ConstIterator &operator++() {
                ptr = ptr->next[0];
                return *this;
            }    

            ConstIterator operator++(int) {
                ConstIterator retval = *this;
                ++(*this);
                return retval;
            }

            ConstIterator &operator--() {
                ptr = ptr->prev[0];
                return *this;
            }    

            ConstIterator &operator--(int) {
                ConstIterator retval = *this;
                --(*this);
                return retval;
            } 

            const ValueType &operator*() const {
                return ptr->vals;
            }

            const ValueType *operator->() const {
                return &(ptr->vals);
            }  

            friend bool operator==(const ConstIterator &i1, const ConstIterator &i2) {
                return i1.ptr == i2.ptr;
            }

            friend bool operator==(const ConstIterator &i1, const Iterator &i2) {
                return i1.ptr == i2.ptr;
            }

            friend bool operator!=(const ConstIterator &i1, const ConstIterator &i2) {
                return !(i1 == i2);
            }

            friend bool operator!=(const ConstIterator &i1, const Iterator &i2) {
                return !(i1 == i2);
            }
        };

        class ReverseIterator {        
        public:
            typename SkipList::Node* ptr;

            ReverseIterator() = delete;

            ReverseIterator(typename SkipList::Node* ptr) {
                this->ptr = ptr;
            }

            ReverseIterator &operator++() {
                ptr = ptr->prev[0];
                return *this;
            }    

            ReverseIterator operator++(int) {
                ReverseIterator retval = *this;
                ++(*this);
                return retval;
            }

            ReverseIterator &operator--() {
                ptr = ptr->next[0];
                return *this;
            }    

            ReverseIterator &operator--(int) {
                ReverseIterator retval = *this;
                --(*this);
                return retval;
            } 

            ValueType &operator*() const {
                return ptr->vals;
            }

            ValueType *operator->() const {
                return &(ptr->vals);
            }  

            friend bool operator==(const ReverseIterator &i1, const ReverseIterator &i2) {
                return i1.ptr == i2.ptr;
            }

            friend bool operator!=(const ReverseIterator &i1, const ReverseIterator &i2) {
                return !(i1 == i2);
            }
        };

    private:
        Map<Key_T, Mapped_T>::SkipList* list;                    
    };
}

using namespace cs540;
using namespace std;

int main() {
    Map<int, int> lst;
    lst.insert(std::pair<int, int>(0, 1));
    lst.insert(std::pair<int, int>(-1, 2));
    lst.insert(std::pair<int, int>(37, 3));
    lst.insert(std::pair<int, int>(202, 4));
    lst.insert(std::pair<int, int>(10, 5));
    lst.insert(std::pair<int, int>(20, 6));
    lst.insert(std::pair<int, int>(3, 7));
    lst.insert(std::pair<int, int>(14, 8));
    lst.insert(std::pair<int, int>(-20, 9));
    lst.insert(std::pair<int, int>(11, 10));
    lst.displayList();
    Map<int, int> lst2(lst);
    lst.erase(3);
    lst.insert(make_pair(2, 7));
    if(lst < lst2) cout << "LESS" << endl;
}

#endif
