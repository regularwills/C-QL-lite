#ifndef PAIR_H
#define PAIR_H

#include <ostream>
using namespace std;

//--------------------------------------------------------------------------------
/*    Pair: Simple class that holds a key and a value and can be compared by the key
*/
//--------------------------------------------------------------------------------

template <typename K, typename V>
struct Pair{
    K key;
    V value;

    Pair(const K& k=K(), const V& v=V()): key(k), value(v){}
    
    friend ostream& operator <<(ostream& outs, const Pair<K, V>& print_me){
        outs << "["<<print_me.key<<":"<<print_me.value<<"]";
        return outs;
    }

    //Operators all comparing the keys
    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key == rhs.key;
    }
    friend bool operator !=(const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key != rhs.key;
    }
    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key < rhs.key;
    }
    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key > rhs.key;
    }
    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key <= rhs.key;
    }
    friend bool operator >= (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return lhs.key >= rhs.key;
    }
    friend Pair<K, V> operator + (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return rhs; //Overwrite lefthand side
    }
};

#endif //PAIR_H