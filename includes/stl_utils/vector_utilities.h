#ifndef VECTOR_UTILITIES_H
#define VECTOR_UTILITIES_H

#include <ostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename T>
ostream& operator << (ostream& outs, const vector<T>& v) {
    for (int i = 0; i <v.size(); i++){
        //At the last element in the vector
        if (i == v.size()-1)
            outs << v.at(i);
        else 
            outs << v.at(i) << ", ";
    }
    return outs;
}

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme){ 
    //Adding this item to the vector list
    list.push_back(addme);
    return list;
}

//Add all values in the RHS vector into the LHS vector
template<typename T>
vector<T>& operator+(vector<T>& LHS, vector<T>RHS){
    for(int i = 0; i < RHS.size();i++){
        LHS.push_back(RHS[i]);
    }
    return LHS;
} 

template <typename T>
vector<T> intersection(vector<T> v1, vector<T> v2){
    vector<T> v3;

    sort(v1.begin(), v1.end());
    sort(v2.begin(), v2.end());

    set_intersection(v1.begin(),v1.end(),
                          v2.begin(),v2.end(),
                          back_inserter(v3));
    return v3;
}

#endif//VECTOR_UTILITIES_H