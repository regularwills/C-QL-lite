#ifndef MAP_H
#define MAP_H

#include "../btree/btree.h"
#include "../pairs/pair.h"

//Wrapper for the Btree class

template <typename K, typename V>
class Map {
public:

    typedef BTree<Pair<K, V> > map_base;  

//  Constructor
    Map():key_count(0), map(map_base()){}


//  Capacity
    int size() const;
    bool empty() const;

//  Element Access
    V& operator[](const K& key);
    V& at(const K& key);
    const V& at(const K& key) const;


//  Modifiers
    void insert(const K& k, const V& v);
    void clear();
    V get(const K& key);

//  Operations:
    K* find(const K& key);
    bool contains(const Pair<K, V>& target) const;

    bool is_valid(){return map.is_valid();}

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me){
        outs<<print_me.map<<endl;
        return outs;
    }
private:
    int key_count;
    BTree<Pair<K, V> > map;
};


//  Capacity
template <typename K, typename V>
int Map<K,V>::size() const{
    return map.size();
}

template <typename K, typename V>
bool Map<K,V>::empty() const{
    return key_count == 0;
}

//  Element Access
template <typename K, typename V>
V& Map<K,V>::operator[](const K& key){
    return (map.get(key)).value;
}

template <typename K, typename V>
V& Map<K,V>::at(const K& key){
    return (map.get(key)).value;
}

template <typename K, typename V>
const V& Map<K,V>::at(const K& key) const{
    return (map.get(key)).value;
}

//  Modifiers
template <typename K, typename V>
void Map<K,V>::insert(const K& k, const V& v){
    map.insert(Pair<K,V>(k,v));
}

 
template <typename K, typename V>
void Map<K,V>::clear(){
    key_count = 0;
    map.clear_tree();
}

template <typename K, typename V>
V Map<K,V>::get(const K& key){
   return map.get(key);
}

//  Operations:
template <typename K, typename V>
K* Map<K,V>::find(const K& key){
    return map.find(key);
}

template <typename K, typename V>
bool Map<K,V>::contains(const Pair<K, V>& target) const{
    return map.contains(target);
}




#endif//MAP_H
