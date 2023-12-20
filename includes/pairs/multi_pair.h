#ifndef MULTI_PAIR_H
#define MULTI_PAIR_H

#include <vector>

/*--------------------------------------------------------------------------------
* Multi_Pair: Simple class that holds a key and a vector value and can be compared        
* to other pairs through the key. Vector value allows * for mutliple values under
* the same key
*/
//--------------------------------------------------------------------------------



template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;

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

#endif //MULTI_PAIR_H