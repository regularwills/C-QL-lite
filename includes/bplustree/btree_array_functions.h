#ifndef BTREE_ARRAY_FUNCS_H
#define BTREE_ARRAY_FUNCS_H

#include <iostream>
#include <ostream>
#include <vector>

using namespace std;

//--------------------------------------------
// Helper functions for the lower level array movements
// necessary for the BPlusTree class
//--------------------------------------------

template <class T>
T maximal(const T& a, const T& b){   
 //Return the larger of the two items
    if (a > b) {return a; }
    else { return b; }
}

template <class T>
void swap(T& a, T& b){  
    T temp = a;
    a = b;
    b = temp; 
}

template <class T>
int index_of_maximal(T data[ ], int n){ 
    //return index of the largest item in data
    int index = 0;
    int max = 0;

    for (int i = 0; i < n; i++){
        if (data[i] > max){
            max = data[i];
            index = i;
        }
    }
    return index;
}

template <class T>
void ordered_insert(T data[ ], int& n, T entry){        
    //insert entry into the sorted array data with length n

    int index;
    //Starting from the end, shift each item in data down by one until a place for entry is found
    for (index = n - 1; (index >= 0 && data[index] > entry); index--){
        data[index + 1] = data[index];
    }
    data[index + 1] = entry;
    n++;
}

template <class T>
int first_ge(const T data[ ], int n, const T& entry){   
    //return the first element in data that is not less than entry
    for (int index = 0; index < n; index++){
        if (data[index] >= entry){
            return index;
        }
    }
    return n;
}

template <class T>
void attach_item(T data[ ], int& n, const T& entry){  
    //append entry to the right of data
    data[n] = entry;
    n++;
}


template <class T>
void insert_item(T data[ ], int i, int& n, T entry){    
    //insert entry at index i in data
    for (int index = n; index > i; index--){
        data[index] = data[index-1];
    }
    data[i] = entry;
    n++;
}

template <class T>
void detach_item(T data[ ], int& n, T& entry){ 
    //remove the last element in data and place it in entry
    n--;
    entry = data[n];
}

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry){   
    //delete item at index i and place it in entry
    entry = data[i];
    n--;

    //Starting from the ith item, shift each data position one over to the left
    for (int index = i; index < n; index++){
        data[index] = data[index+1];
    }
}

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2){   
    //append data2 to the right of data1
    for (int i = 0; i < n2; i++){
        data1[n1] = data2[i];
        n1++;
    }
    n2 = 0;
}

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2){   
    //move n/2 elements from the right of data1 and move to data2
    
    //Starting in the middle of the first array, copy the values onward
    int middle_index = n1/2;
    n2 = 0;
    for (int i = middle_index; i < n1; i++){
        data2[n2] = data1[i];
        n2++;
    }
    n1 = n1/2;
}

template <class T>
void copy_array(T dest[], const T src[],
                int& dest_size, int src_size){   
    //copy src[] into dest[]
    
    //Iterating through the src array and copying values
    for (int i = 0; i < src_size; i++){
        dest[i] = src[i];
        dest_size++;
    }
}

template <class T>
void print_array(const T data[], int n, int pos = -1){ 
    if (pos == -1){ return; }

    for (int i = pos; i < n; i++){
        cout << "["<< data[i] <<"]";
    }
}

template <class T>
bool is_gt(const T data[], int n, const T& item){ 
    //item > all data[i]
    
    for (int i = 0; i < n; i++){
        // item is less or equal to an item in data
        if (item <= data[i]){   
            return false;
        }
    }
    return true;
}

template <class T>
bool is_le(const T data[], int n, const T& item){ 
    //item <= all data[i]
    
    for (int i = 0; i < n; i++){
        //item is greater than an item in data
        if (item > data[i]){    
            return false;
        }
    }
    return true;
}

#endif//BTREE_ARRAY_FUNCTIONS_H