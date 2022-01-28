#include <iostream>
#include <utility>
#include <assert.h>
#include "Map.hpp"

int main() {
    cs540::Map<int, int> myMap;
    myMap.insert(std::make_pair(0, 1));
    myMap.insert(std::make_pair(12, 2));
    myMap.insert(std::make_pair(36, 3));
    myMap.insert(std::make_pair(99, 4));
    myMap.insert(std::make_pair(111, 5));
    myMap.insert(std::make_pair(1011, 6));
    myMap.insert(std::make_pair(20, 7));
    myMap.insert(std::make_pair(70, 8));
    myMap.insert(std::make_pair(36, 9));
    myMap.insert(std::make_pair(9999, 10));

    std::cout << "Iterate forward with Iterator:" << std::endl;
    for(cs540::Map<int,int>::Iterator i = myMap.begin(); i != myMap.end(); i++) {
        std::cout << "Key: " << i->first << ", Value: " << i->second << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Iterate forward with ConstIterator:" << std::endl;
    for(cs540::Map<int,int>::ConstIterator i = myMap.begin(); i != myMap.end(); i++) {
        std::cout << "Key: " << i->first << ", Value: " << i->second << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Iterate backward with ReverseIterator:" << std::endl;
    for(auto i = myMap.rbegin(); i != myMap.rend(); i++) {
        std::cout << "Key: " << i->first << ", Value: " << i->second << std::endl;
    }
    std::cout << std::endl;    

    cs540::Map<int, int> myMap2(myMap);
    std::cout << "Duplicate map with copy constructor and compare equality:" << std::endl;
    if(myMap == myMap2) {
        std::cout << "Maps are equal!" << std::endl;
    }
    std::cout << std::endl;
    
    cs540::Map<int, int> myMap3 = myMap;
    std::cout << "Duplicate map with '=' operator and compare equality:" << std::endl;
    if(myMap == myMap3) {
        std::cout << "Maps are equal!" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Attemt to find element in map:" << std::endl;
    if(myMap.find(36) != myMap.end()) {
        std::cout << "Element found!" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Attemt to find element not in map:" << std::endl;
    if(myMap.find(-1) == myMap.end()) {
        std::cout << "Element not found!" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Erase element by key:" << std::endl;
    assert(myMap.find(20) != myMap.end());
    myMap.erase(20);
    if(myMap.find(20) == myMap.end()) {
        std::cout << "Element successfully erased!" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Erase element by iterator:" << std::endl;
    assert(myMap.find(0) != myMap.end());
    myMap.erase(myMap.begin());
    if(myMap.find(0) == myMap.end()) {
        std::cout << "Element successfully erased!" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Test indexing using at():" << std::endl;
    assert(myMap.at(70) == 8);
    std::cout << "Value corresponding to key 8: " << myMap.at(70) << std::endl;
    std::cout << std::endl;

    std::cout << "Test insert using '[]' operator:" << std::endl;
    myMap[69];
    if(myMap.find(69) != myMap.end()) {
        std::cout << "Element successfully inserted!" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Test size() and clear():" << std::endl;
    std::cout << "Size before clear(): " << myMap.size() << std::endl;
    myMap.clear();
    std::cout << "Size after clear(): " << myMap.size() << std::endl;
}
