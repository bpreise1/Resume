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
            class SentinelNode {
            public:
                int level;
                std::vector<SentinelNode*> next, prev;
                SentinelNode(int level_):
                        level(level_),
                        next(std::vector<SentinelNode*>(level+1, nullptr)),
                        prev(std::vector<SentinelNode*>(level+1, nullptr))
                {
                    static bool isSeed = false;
                    if(!isSeed) {
                        srand(time(NULL));
                    }
                }
                virtual ~SentinelNode() = default;
            };
            class Node : public SentinelNode {
            public:
                ValueType vals;

                Node(ValueType vals_, int level):SentinelNode(level), vals(vals_) {
                }
            };

            SkipList() {
                level = 0;
                s = 0;
                sentinel = new SentinelNode(0);
                sentinel->next[0] = sentinel;
                sentinel->prev[0] = sentinel;
            }

            ~SkipList() {
                SentinelNode* current = sentinel->next[0];
                while(dynamic_cast<Node*>(current)) {
                    SentinelNode* next = current->next[0];
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

            SentinelNode* insert(ValueType vals) {
                int lvl = generateLevel();
                int prevLevel = lvl;
                SentinelNode* newNode = new Node(vals, lvl);
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
                SentinelNode* current;
                std::vector<SentinelNode*> beforeNewNode(prevLevel+1, nullptr);
                for(int i = prevLevel; i >= 0; i--) {
                    if(i == prevLevel) {
                        current = sentinel;
                    }
                    else {
                        current = beforeNewNode[i+1];
                    }
                    while(dynamic_cast<Node*>(current->next[i]) and dynamic_cast<Node*>(current->next[i])->vals.first < vals.first) {
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
                SentinelNode* node = this->find(key);
                for(size_t i = 0; i < node->next.size(); i++) {
                    node->prev[i]->next[i] = node->next[i];
                    node->next[i]->prev[i] = node->prev[i];
                }
                delete node;
                SentinelNode* current = sentinel;
                for(int i = level; i >= 0; i--) {
                    if(!dynamic_cast<Node*>(current->next[i])) {
                        current->next.resize(i);
                        level--;
                    }
                    else {
                        break;
                    }
                }
                s--;
            }

            SentinelNode* find(const Key_T key) {
                if(s == 0) {
                    return sentinel;
                }
                SentinelNode* current = sentinel;
                Node* true_current = dynamic_cast<Node*>(current);
                int lvl = level;
                while(1) {
                    Node* pot_next = dynamic_cast<Node*>(current->next[lvl]);
                    if(lvl == 0) {
                        if((!true_current and pot_next and key < pot_next->vals.first) or
                           (true_current and true_current->vals.first < key and pot_next and key < pot_next->vals.first)
                           or (true_current and true_current->vals.first < key and !pot_next)) {
                            return sentinel;
                        }
                    }
                    if(true_current and true_current->vals.first == key) {
                        return current;
                    }
                    if(pot_next and (pot_next->vals.first < key or pot_next->vals.first == key)) {
                        current = current->next[lvl];
                        true_current = pot_next;
                    }
                    else {
                        lvl--;
                    }
                }
            }

            size_t size() {
                return s;
            }

            SentinelNode* getSentinel() {
                return sentinel;
            }

        private:
            int level;
            size_t s;
            SentinelNode *sentinel;
        };

    public:
        class Iterator;
        class ConstIterator;
        class ReverseIterator;

        Map() : list(new SkipList()){
            
        }

        Map(const Map &map) : list(new SkipList) {
            for(auto i = map.begin(); i != map.end(); i++) {
                list->insert(*i);
            }
        }

        Map &operator=(const Map &map) {
            for(auto i = map.begin(); i != map.end(); i++) {
                list->insert(*i);
            }
            return *this;
        }

        Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> init_list) : list(new SkipList()){
            for(auto val : init_list) {
                insert(val);
            }
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
            return list->getSentinel()->next[0];
        }

        Iterator end() {
            return list->getSentinel();
        }

        ConstIterator begin() const {
            return list->getSentinel()->next[0];
        }

        ConstIterator end() const {
            return list->getSentinel();
        }

        ReverseIterator rbegin() {
            return list->getSentinel()->prev[0];
        }

        ReverseIterator rend() {
            return list->getSentinel();
        }

        Iterator find(const Key_T &key) {
            return list->find(key);
        }

        ConstIterator find(const Key_T &key) const {
            return list->find(key);
        }

        Mapped_T &at(const Key_T &key) {
            auto found = find(key);
            if (found == end()) {
                throw std::out_of_range("Element not in map");
            }
            return found->second;
        }

        const Mapped_T &at(const Key_T &key) const {
            auto found = find(key);
            if (found == end()) {
                throw std::out_of_range("Element not in map");
            }
            return found->second;
        }

        Mapped_T &operator[](const Key_T &key) {
            typename SkipList::SentinelNode* ptr = list->find(key);
            if(!dynamic_cast<typename SkipList::Node*>(ptr)) {
                ValueType val{key, Mapped_T()};
                ptr = list->insert(val);
                return dynamic_cast<typename SkipList::Node*>(ptr)->vals.second;
            }
            return dynamic_cast<typename SkipList::Node*>(ptr)->vals.second;
        }

        std::pair<Iterator, bool> insert(const ValueType &values) {
            auto found = find(values.first);
            if(found == end()) {
                return std::pair<Iterator, bool>(Iterator(list->insert(values)), true);
            }
            return std::pair<Iterator, bool>(found, false);
        }

        template <typename IT_T>
        void insert(IT_T range_beg, IT_T range_end) {
            for(IT_T i = range_beg; i != range_end; i++) {
                insert(*i);
            }
        }


        void erase(const Key_T &key) {
            auto found = find(key);
            if (found == end()) {
                throw std::out_of_range("Element not in map");
            }
            list->erase(key);
        }

        void erase(Iterator pos) {
            auto found = find(pos->first);
            if (found == end()) {
                throw std::out_of_range("Element not in map");
            }
            erase(pos->first);
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

        class Iterator {
        public:
            typename SkipList::SentinelNode* ptr;

            Iterator() = delete;

            Iterator(typename SkipList::SentinelNode* ptr) {
                this->ptr = ptr;
            }

            Iterator &operator++() {
                ptr = ptr->next[0];
                return *this;
            }

            Iterator operator++(int) {
                Iterator& retval = *this;
                ++(*this);
                return retval;
            }

            Iterator &operator--() {
                ptr = ptr->prev[0];
                return *this;
            }

            Iterator operator--(int) {
                Iterator& retval = *this;
                --(*this);
                return retval;
            }

            ValueType &operator*() const {
                return dynamic_cast<typename SkipList::Node*>(ptr)->vals;
            }

            ValueType *operator->() const {
                return &(dynamic_cast<typename SkipList::Node*>(ptr)->vals);
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
            typename SkipList::SentinelNode* ptr;

            ConstIterator() = delete;

            ConstIterator(typename SkipList::SentinelNode* ptr) {
                this->ptr = ptr;
            }

            ConstIterator(const Iterator &it) {
                this->ptr = it.ptr;
            }

            ConstIterator &operator++() {
                ptr = ptr->next[0];
                return *this;
            }

            ConstIterator operator++(int) {
                ConstIterator& retval = *this;
                ++(*this);
                return retval;
            }

            ConstIterator &operator--() {
                ptr = ptr->prev[0];
                return *this;
            }

            ConstIterator operator--(int) {
                ConstIterator& retval = *this;
                --(*this);
                return retval;
            }

            const ValueType &operator*() const {
                return dynamic_cast<typename SkipList::Node*>(ptr)->vals;
            }

            const ValueType *operator->() const {
                return &(dynamic_cast<typename SkipList::Node*>(ptr)->vals);
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
            typename SkipList::SentinelNode* ptr;

            ReverseIterator() = delete;

            ReverseIterator(typename SkipList::SentinelNode* ptr) {
                this->ptr = ptr;
            }

            ReverseIterator &operator++() {
                ptr = ptr->prev[0];
                return *this;
            }

            ReverseIterator operator++(int) {
                ReverseIterator& retval = *this;
                ++(*this);
                return retval;
            }

            ReverseIterator &operator--() {
                ptr = ptr->next[0];
                return *this;
            }

            ReverseIterator operator--(int) {
                ReverseIterator& retval = *this;
                --(*this);
                return retval;
            }

            ValueType &operator*() const {
                return dynamic_cast<typename SkipList::Node*>(ptr)->vals;
            }

            ValueType *operator->() const {
                return &(dynamic_cast<typename SkipList::Node*>(ptr)->vals);
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

#endif
