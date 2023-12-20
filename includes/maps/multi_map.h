#ifndef MULTI_MAP_H
#define MULTI_MAP_H

#include <vector>
#include "../bplustree/bplustree.h"

template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K& k=K()){
        key = k;
    }
    MPair(const K& k, const V& v){
        key = k;
        value_list.push_back(v);
    }
    MPair(const K& k, const vector<V>& vlist){
        key = k;
        value_list = vlist;
    }
    //--------------------------------------------------------------------------------
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me){
        outs <<print_me.key<<": ";
        
        for (int i = 0; i < print_me.value_list.size(); i++){
            outs << print_me.value_list.at(i) << " ";
        }
        return outs;
    }
    
    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key == rhs.key;
    }
    friend bool operator !=(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key != rhs.key;
    }
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key < rhs.key;
    }
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key <= rhs.key;
    }
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key > rhs.key;
    }
    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return lhs.key >= rhs.key;
    }
    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return rhs; //Overwrite lefthand side
    }
};

template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator{
    public:
        friend class MMap;
        Iterator(typename map_base::Iterator it = NULL): _it(it){}
        Iterator operator ++(int unused){
            _it++;
            return _it;
        }
        Iterator operator ++(){
            ++_it;
        }
        MPair<K, V> operator *(){
           return *_it; 
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return lhs._it == rhs._it;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return lhs._it != rhs._it;
        }

    private:
        typename map_base::Iterator _it;
    };

    MMap(): mmap(map_base()){}

//  Iterators
    Iterator begin();
    Iterator end();
    Iterator lower_bound(const K& key);  
    Iterator upper_bound(const K& key); 

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
    void erase(const K& key);
    void clear();

//  Operations:
    bool contains(const K& key) const ;
    vector<V> &get(const K& key);

    Iterator find(const K& key);
    int count(const K& key);

    bool is_valid();
    void print_lookup();

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me.mmap<<endl;
        return outs;
    }

private:
    BPlusTree<MPair<K, V> > mmap;
};

//  Iterators
template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::begin(){
    return mmap.begin();
}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::end(){
    return mmap.end();
}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::lower_bound(const K& key){
    return mmap.lower_bound(key);
}  

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::upper_bound(const K& key){
    return mmap.lower_bound(key);
} 

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::find(const K& key){
    return mmap.find(key);
}

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
void MMap<K,V>::erase(const K& key){

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
int MMap<K,V>::count(const K& key){
    int count = 0;
    MMap<K,V>::Iterator it = mmap.begin();
    for (; it != mmap.end(); it++){
        count++;
    }
    return count;
}

template <typename K, typename V>
bool MMap<K,V>::is_valid(){
    return mmap.is_valid();
}

template <typename K, typename V>
void MMap<K,V>::print_lookup(){
    assert(!mmap.empty());
    
    //Walking through each node and printing the mmap as a table
    MMap<K,V>::Iterator it = mmap.begin();

    while (it != mmap.end()){
        cout << *it << endl;
        it++;
    }
}

#endif//MULTIMAP_H
