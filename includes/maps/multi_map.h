#ifndef MULTIMAP_H
#define MULTIMAP_H

#include <vector>
#include "../btree/btree.h"
#include "../pairs/multi_pair.h"

template <typename K, typename V>
class MMap {

public:
    typedef BTree<MPair<K, V> > map_base;

    MMap(): mmap(map_base()){}

//  Capacity
    int size() const;
    bool empty() const;

//  Element Access
    const vector<V>& operator[](const K& key) const;
    vector<V>& operator[](const K& key);
    vector<V>& at(const K& key);
    const vector<V>& at(const K& key) const;


//  Modifiers
    void insert(const K& k, const V& v);
    void clear();

//  Operations:
    bool contains(const K& key) const ;
    vector<V> &get(const K& key);

    K* find(const K& key);
    int count(const K& key);

    bool is_valid();
    void print_lookup();

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me.mmap<<endl;
        return outs;
    }

private:
    BTree<MPair<K, V> > mmap;
};

//  Capacity
template <typename K, typename V>
int MMap<K,V>::size() const{
    return mmap.size();
}

template <typename K, typename V>
bool MMap<K,V>::empty() const{
    return mmap.empty();
}

//  Element Access
template <typename K, typename V>
const vector<V>& MMap<K,V>::operator[](const K& key) const{
    return (mmap.get(key)).value_list;
}

template <typename K, typename V>
vector<V>& MMap<K,V>::operator[](const K& key){
    return (mmap.get(key)).value_list;
}

template <typename K, typename V>
vector<V>& MMap<K,V>::at(const K& key){
    return (mmap.get(key)).value_list;
}

template <typename K, typename V>
const vector<V>& MMap<K,V>::at(const K& key) const{
    return (mmap.get(key)).value_list;
}

//  Modifiers
template <typename K, typename V>
void MMap<K,V>::insert(const K& k, const V& v){
    mmap.insert(MPair<K,V>(k,v));
}

template <typename K, typename V>
void MMap<K,V>::clear(){
    mmap.clear_tree();
}

//  Operations:
template <typename K, typename V>
bool MMap<K,V>::contains(const K& key) const {
    return mmap.contains(key);
}

template <typename K, typename V>
vector<V>& MMap<K,V>::get(const K& key){
    return mmap.get(key);
}


template <typename K, typename V>
bool MMap<K,V>::is_valid(){
    return mmap.is_valid();
}


#endif//MULTIMAP_H

