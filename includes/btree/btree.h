#ifndef BTREE_H
#define BTREE_H

#include <ostream>
#include <string>
#include "btree_array_functions.h"

using namespace std;

template <class T>
class BTree
{
public:
    BTree(bool dups = false);
    BTree(T *a, int size, bool dups = false);
    BTree(const BTree<T>& other);
    ~BTree();

    BTree<T>& operator =(const BTree<T>& RHS);

    void insert(const T& entry);                //insert entry into the tree

    void clear_tree();                          //clear this object
                                                //  (delete all nodes etc.)
    void copy_tree(const BTree<T>& other);      //copy other into this object

    bool contains(const T& entry);              //true if entry can be found in
                                                //                  the array
    T& get(const T& entry);                     //return a reference to entry
                                                //                  in the tree
    T* find(const T& entry);                    //return a pointer to this key.
                                                //          NULL if not there.

    int size() const;                           //count the number of elements
                                                //              in the tree
    bool empty() const;                         //true if the tree is empty

                                                //print a readable version of
                                                //                  the tree
    void print_tree(int level = 0, ostream &outs=cout) const;
    
    friend ostream& operator<<(ostream& outs, const BTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }

    string in_order();                          // traverse the tree in an
                                                // inorder fashion, return a 
                                                // string of all the data items
                                                // with vertical delimiters

//private:
public:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                               //true if duplicate keys may be
                                                //      inserted
    int data_count;                             //number of data elements
    T data[MAXIMUM + 1];                        //holds the keys
    int child_count;                            //number of children
    BTree* subset[MAXIMUM + 2];                 //subtrees

    bool is_leaf() const {return child_count==0;} //true if this is a leaf node

    //insert element functions
    void loose_insert(const T& entry);          //allows MAXIMUM+1 data
                                                //      elements in the root
    void fix_excess(int i);                     //fix excess of data elements
                                                //                  in child i

    //remove element functions:
    void loose_remove(const T& entry);          //allows MINIMUM-1 data
                                                //      elements in the root
    void fix_shortage(int i);                   //fix shortage of data elements
                                                //                  in child i

    void remove_biggest(T& entry);              //remove the biggest child of
                                                //          this tree->entry
    void rotate_left(int i);                    //transfer one element LEFT
                                                //              from child i
    void rotate_right(int i);                   //transfer one element RIGHT
                                                //              from child i
    void merge_with_next_subset(int i);         //merge subset i with subset
                                                //                      i+1

};

template <typename T>
BTree<T>::BTree(bool dups): data_count(0), child_count(0){}

//*********** BIG THREE *********
template <typename T>
BTree<T>::BTree(T *a, int size, bool dups){
    //Create an empty list
    data_count = 0;
    child_count = 0;
    
    //Adding to the tree from a list, start to finish
    for (int i = 0; i < size; i++){
        insert(a[i]);
    }
}

template <typename T>
BTree<T>::BTree(const BTree<T>& other){
    clear_tree();
    copy_tree(other);
}

template <typename T>
BTree<T>::~BTree(){
    clear_tree();
}
//******* END BIG THREE **********

template <typename T>
BTree<T>& BTree<T>::operator =(const BTree<T>& RHS){
    //Same address, do nothing
    if (this == &RHS)
        return *this;
    //Make sure this tree is empty
    clear_tree();
    
    copy_tree(RHS);
    return *this;
}

template <typename T>
void BTree<T>::insert(const T& entry){ 
    //Inserting the entry into the tree
    loose_insert(entry);
    
    //There is a excess problem at the root most node
    if (data_count > MAXIMUM){
        BTree<T>* growth = new BTree<T>();
        
        //copy data and subsets into the growth data[] and subset[]
        copy_array(growth->data, data, growth->data_count, data_count);
        copy_array(growth->subset, subset, growth->child_count, child_count);

        //Growth and its entire tree is now the only child of root
        data_count = 0;
        child_count = 1;
        subset[0] = growth;
        //Promote the middle val in growth to the root as the only value
        fix_excess(0);
    }
}


template <typename T>
void BTree<T>::clear_tree(){  //clear this object
    //No deletion, just closing the data[] by setting size to zero
    //data_count = 0;

    //Clear all of the childs data[] and their childrens data[] until you
    // hit a leaf node, null out all of the children
    if (!is_leaf()){
        for (int i = 0; i < child_count; i++){
            subset[i]->clear_tree();
            delete subset[i];
            subset[i] = nullptr;
        }
    }
    //All the children have been deleted and subsets are pointing to null
    // so now there are no children
    data_count = 0;
    child_count = 0;
}  


template <typename T>
void BTree<T>::copy_tree(const BTree<T>& other){  //copy other into this object
    //Copy this other nodes data into this objects data[]
    copy_array(data, other.data, data_count, other.data_count);
    
    //Set the same child count for this node as the other node
    child_count = other.child_count;

    //Copy all of the childs data[] and their childrens data[] until you
    // hit a leaf node
    for (int i = 0; i < child_count; i++){
        subset[i] = new BTree<T>();
        subset[i]->copy_tree(*other.subset[i]);
    }
}

template <typename T>
bool BTree<T>::contains(const T& entry){ //true if entry can be found in the array
    //No address in data was found that contained entry and the item is not in the tree
    if (find(entry) == nullptr){
        return false;
    }
    //Entry was found in the tree
    return true;
}

template <typename T>
T& BTree<T>::get(const T& entry){  //return a reference to entry in the tree
    if (!contains(entry)){
        insert(entry);
    }
    
    return *(find(entry));
}                            

template <typename T>
T* BTree<T>::find(const T& entry){   //return a pointer to this key. NULL if not there
    int i = first_ge(data, data_count, entry);
    bool found = (data[i] == entry);

    //We found the entry in this node at this tree/subtree's data[]
    if (found){
        return &data[i];
    }
    //Havn't found the entry in the tree and there is no where else to look
    if (is_leaf()){
        return nullptr;
    }
    //If there is a subtree to keep searching through, search that subtree
    else {
        return subset[i]->find(entry);
    }
}

template <typename T>
int BTree<T>::size() const{  //count the number of elements in the tree
    //Tree size will keep track of all the data at each level
    int tree_size = data_count;

    //Go through all the levels and increment tree size
    for (int i = 0; i < child_count; i++){
        //Getting the sizes of all child size and their children
        tree_size += subset[i]->size();
    }
    return tree_size;
}                              

template <typename T>
bool BTree<T>::empty() const{  //true if the tree is empty
    return data_count == 0;
}

template <typename T>
void BTree<T>::print_tree(int level, ostream &outs) const{
    //Print the last child
    bool debug = false;
    if (!is_leaf()){
        subset[child_count-1]->print_tree(level+1);
        outs << setw(4*level)<<""<<"⎴"<<endl;
        if (debug){
            //FOR DEBUG: OUTPUT CHILD AND DATA COUNT
            outs << setw(4*level)<<"" << data_count << endl;
            outs << setw(4*level)<<"" << "__" << endl;
            outs << setw(4*level)<<"" << child_count << endl;
            outs << setw(4*level)<<"" << "__" << endl;
        }
        for (int i = data_count-1; i >= 0; i--){
            outs << setw(4*level)<<""<< data[i] << endl;
            if (i == 0){
                outs << setw(4*level) <<""<< "⎵" << endl;
            }
            subset[i]->print_tree(level+1);
        }
    }
    //Print the other subtrees
    else {
        outs << setw(4*level)<<""<<"⎴"<<endl;
        if (debug){
            //FOR DEBUG: OUTPUT CHILD AND DATA COUNT
            outs << setw(4*level)<<"" << data_count << endl;
            outs << setw(4*level)<<"" << "__" << endl;
            outs << setw(4*level)<<"" << child_count << endl;
            outs << setw(4*level)<<"" << "__" << endl;
        }
        for (int i = data_count-1; i >= 0; i--){
            outs << setw(4*level)<<""<< data[i] << endl;
        }
    outs << setw(4*level) <<""<< "⎵" << endl;
    }
}

template <typename T>
string BTree<T>::in_order(){                          
// traverse the tree in an inorder fashion, return a 
// string of all the data items with vertical delimiters
    string in_order = "";

     if (is_leaf()){
        for (int i = 0; i < data_count; i++){
        in_order += to_string(data[i]) + "|";
        }
    }
    else {
        for (int i = 0; i < child_count; i++){
            in_order += subset[i]->in_order();
            if (i < data_count){
                in_order += to_string(data[i]) + "|";
            }
        }
    }
    return in_order;
}

//insert element functions
template <typename T>
void BTree<T>::loose_insert(const T& entry){  //allows MAXIMUM+1 data elements in the root
    //Index is the first index in data[] that is >= than entry
    int index = first_ge(data, data_count, entry);

    //Entry is not already in our tree data
    if (data[index] != entry){
        //Root has no children and were adding the entry and data[index]
        if (is_leaf()){
            insert_item(data, index, data_count, entry);
        }
        //Proper spot hasn't been found at this node and not a leaf node
        // check an appropriate subset for a proper spot for entry
        else {
            subset[index]->loose_insert(entry);
            if (subset[index]->data_count > MAXIMUM){
                fix_excess(index);
             }
        }
    }
}

template <typename T>
void BTree<T>::fix_excess(int i){ //fix excess of data elements in child i

    //Split subset data and child arrays into two arrays
    //Half of each array will go into existing subtree, the other half a new subtree 
    BTree<T>* new_child = new BTree<T>();
    split(subset[i]->data, subset[i]->data_count, new_child->data, new_child->data_count);
    split(subset[i]->subset, subset[i]->child_count, new_child->subset, new_child->child_count);

    //Delete the item from the child and add to the parent
    T new_parent_val;
    delete_item(new_child->data, 0, new_child->data_count, new_parent_val);
    insert_item(data, i, data_count, new_parent_val);

    //Add the new child subset into the parents list of subsets
    insert_item(subset, i+1, child_count, new_child);
}


#endif//BTREE_H