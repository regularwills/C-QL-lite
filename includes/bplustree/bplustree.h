#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iomanip>
#include <string>
#include <cassert>
#include "btree_array_functions.h"

template <class T>
class BPlusTree
{
public:
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it=NULL, int _key_ptr = 0):
                                                it(_it), key_ptr(_key_ptr){}

        T operator *(){
            return it->data[key_ptr];
        } 
        Iterator operator++(int un_used){
            Iterator temp = *this;
            key_ptr++;
            if (key_ptr == it->data_count){
                it = it->next;
                key_ptr = 0;
            }
            return temp;
        }
        
        Iterator operator++(){
            key_ptr++;
            if (key_ptr > it->data_count){
                it = it->next;
                key_ptr = 0;
            }
            return this;
        }

        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return lhs.it == rhs.it;
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return lhs.it != rhs.it;
        }
        
        void print_Iterator(){
            if (it){
                cout<<"iterator: ";
                print_array(it->data, it->data_count, key_ptr);
            }
            else{
                cout<<"iterator: NULL, key_ptr: "<<key_ptr<<endl;
            }
        }
        bool is_null(){return !it;}
        void info(){
            cout<<endl<<"Iterator info:"<<endl;
            cout<<"key_ptr: "<<key_ptr<<endl;
            cout<<"it: "<<*it<<endl;
        }

    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree(bool dups = false);
    BPlusTree(T *a, int size, bool dups = false);

    //big three:
    BPlusTree(const BPlusTree<T>& other);
    ~BPlusTree();
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS);

    void copy_tree(const BPlusTree<T>& other);  //copy other into this object
    void copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node);

    void insert(const T& entry);   //insert entry into the tree
    void clear_tree();             //clear this object (delete all nodes etc.)

    bool contains(const T& entry) const; //true if entry can be found
    T& get(const T& entry);              //return a reference to entry
    const T& get(const T& entry)const;   //return a reference to entry
    T& get_existing(const T& entry);     //return a reference to entry
    Iterator find(const T& key);         //return an iterator to this key.
                                         //     NULL if not there.
    Iterator lower_bound(const T& key);  //return first that goes NOT BEFORE
                                         // key entry or next if does not
                                         // exist: >= entry
    Iterator upper_bound(const T& key);  //return first that goes AFTER key
                                         //exist or not, the next entry  >entry

    int size() const;                    //count the number of elements

    bool empty() const;                  //true if the tree is empty

    void print_tree(int level = 0,
                    ostream &outs=cout) const;
    friend ostream& operator<<(ostream& outs,
                               const BPlusTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }

    bool is_valid();
    string in_order();

    Iterator begin();
    Iterator end();
    ostream& list_keys(Iterator from = NULL, Iterator to = NULL){
        if (from == NULL) from = begin();
        if (to == NULL) to = end();
        for (Iterator it = from; it != to; it++)
            cout<<*it<<" ";
        return cout;

    }

public:
//private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                        //true if duplicate keys are allowed
    int data_count;                      //number of data elements
    T data[MAXIMUM + 1];                 //holds the keys
    int child_count;                     //number of children
    BPlusTree* subset[MAXIMUM + 2];      //subtrees
    BPlusTree* next;
    bool is_leaf() const
    {return child_count==0;}             //true if this is a leaf node

    T* find_ptr(const T& entry);         //return a pointer to this key.
                                         // NULL if not there.

    //insert element functions
    void loose_insert(const T& entry);   //allows MAXIMUM+1 data elements in
                                         //   the root
    void fix_excess(int i);              //fix excess in child i

    //remove element functions:
    void loose_remove(const T& entry);  //allows MINIMUM-1 data elements
                                        //  in the root

    BPlusTree<T>* fix_shortage(int i);  //fix shortage in child i
    // and return the smallest key in this subtree
    BPlusTree<T>* get_smallest_node();
    void get_smallest(T& entry);      //entry := leftmost leaf
    void get_biggest(T& entry);       //entry := rightmost leaf
    void remove_biggest(T& entry);    //remove the biggest child of tree->entry
    void transfer_left(int i);        //transfer one element LEFT from child i
    void transfer_right(int i);       //transfer one element RIGHT from child i
    BPlusTree<T> *merge_with_next_subset(int i); //merge subset i with  i+1
};

//---------------------------------------------------------------------
//            B PLUS TREE PUBLIC FUNCTIONS
//---------------------------------------------------------------------

template <typename T>
BPlusTree<T>::BPlusTree(bool dups):dups_ok(dups), data_count(0),
                                   child_count(0), next(NULL){}

template <typename T>
BPlusTree<T>::BPlusTree(T *a, int size, bool dups){
    //Create an empty tree
    data_count = 0;
    child_count = 0;
    next = nullptr;
    
    //Adding to the tree from a list, start to finish
    for (int i = 0; i < size; i++){
        insert(a[i]);
    }
}

//----------------- BIG THREE ------------------------------------
template <typename T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& other):
                        data_count(0), child_count(0){
    clear_tree();
    copy_tree(other);
}

template <typename T>
BPlusTree<T>::~BPlusTree(){
    clear_tree();
}

template <typename T>
BPlusTree<T>& BPlusTree<T>::operator =(const BPlusTree<T>& RHS){
    //Check if the same object
    if (this == &RHS){
        return *this;
    }
    //Make sure tree is clear
    clear_tree();
    copy_tree(RHS);
    return *this;
}

//----------------- BIG THREE ------------------------------------

template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other){
    //Copy this other nodes data into this objects data[]
    BPlusTree<T>* node_ptr = nullptr;
    copy_tree(other, node_ptr);
}                                                            //copy other into this object

template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node){

    copy_array(data, other.data, data_count, other.data_count);
    
    //Set the same child count for this node as the other node
    child_count = other.child_count;

    //Copy all of the childs data[] and their childrens data[] until you
    // hit a leaf node
    for (int i = 0; i < child_count; i++){
        subset[i] = new BPlusTree<T>();        
        subset[i]->copy_tree(*other.subset[i], last_node);

        if (subset[i]->is_leaf() && last_node != subset[i]){
            if (last_node != nullptr){
                last_node->next = subset[i];
            }
            last_node = subset[i];
        }
    }
}

//Insert entry into the tree
template <typename T>
void BPlusTree<T>::insert(const T& entry){
    //Inserting the entry into the tree
    loose_insert(entry);
    
    //There is a excess problem at the root most node
    if (data_count > MAXIMUM){
        BPlusTree<T>* growth = new BPlusTree<T>();
        
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

//Clear this object and delete all the nodes
template <typename T>
void BPlusTree<T>::clear_tree(){
    //No deletion, just closing the data[] by setting size to zero
    data_count = 0;

    //Clear all of the childs data[] and their childrens data[] until you
    // hit a leaf node, null out all of the children
    for (int i = 0; i < child_count; i++){
        subset[i]->clear_tree();
        delete subset[i];
        subset[i]->next = nullptr;
        subset[i] = nullptr;
    }
    //All the children have been deleted and subsets are pointing to null
    // so now there are no children
    child_count = 0;
}                                                           

template <typename T>
bool BPlusTree<T>::contains(const T& entry) const{
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i] == entry);
    //We found the entry in this node at this tree/subtree's data[]
    if (found){
        return true;
    }
    //Havn't found the entry in the tree and there is no where else to look
    if (!found && is_leaf()){
        return false;
    }
    //If there is a subtree to keep searching through, search that subtree
    else {
        return subset[i]->contains(entry);
    }
}       

//Return a reference to entry
template <typename T>
T& BPlusTree<T>::get(const T& entry){
    //If entry is not in the tree, add it to the tree
    //assert(contains(entry));

    const bool debug = false;
    if (!contains(entry))
        insert(entry);

    return get_existing(entry);
}                                                           

//Return a reference to this entry
template <typename T>
const T& BPlusTree<T>::get(const T& entry)const{
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i] == entry);
    //We found the entry in this node at this tree/subtree's data[]
    if (found){
        return data[i];
    }
    //Havn't found the entry in the tree and there is no where else to look
    if (!found && is_leaf()){
        cout << "Entry not found!" << endl;
        assert(found);
    }
    //If there is a subtree to keep searching through, search that subtree
    else {
        return subset[i]->get(entry);
    }
}                                                   

//Return a reference to entry, if entry is not in the tree, assert
template <typename T>
T& BPlusTree<T>::get_existing(const T& entry){
    assert(contains(entry));

    const bool debug = false;
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);
    if (is_leaf()){
        if (found){
            return data[i];
        }
        else {
            if (debug) cout<<"get_existing was called with nonexistent entry"<<endl;
            assert(found);
        }
    }
    if (found) //inner node
        return subset[i+1]->get(entry);
        //or just return true?
    else //not found yet...
        return subset[i]->get(entry);
} 

//Return an iterator to this key. NULL if not there.
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::find(const T& key){
    int i = first_ge(data, data_count, key);
    bool found = (i<data_count && data[i] == key);
    //We found the entry in this node at this tree/subtree's data[]
    if (found){
        return Iterator(this, i);
    }
    //Havn't found the entry in the tree and there is no where else to look
    if (!found && is_leaf()){
        return Iterator(nullptr);
    }
    //If there is a subtree to keep searching through, search that subtree
    else {
        return subset[i]->find(key);
    }
}

template <typename T>                                        
typename BPlusTree<T>::Iterator BPlusTree<T>::lower_bound(const T& key){
    BPlusTree<T>::Iterator it = begin();

    while (it != end()){
        if (*it >= key){
            return it;
        }
        it++;
    }
    return Iterator(NULL);
}                                       //return first that goes NOT BEFORE
                                        // key entry or next if does not
template <typename T>                                        // exist: >= entry
typename BPlusTree<T>::Iterator BPlusTree<T>::upper_bound(const T& key){
    BPlusTree<T>::Iterator it = begin();
    while (it != end()){
        if (*it > key){
            //cout << "Returning it here" << endl;
            it++;
            return it;
        }
        it++;
    }
    //cout << "Returning NULL here" << endl;
    return Iterator(NULL);
}                                       //return first that goes AFTER key
                                        //exist or not, the next entry  >entry

template <typename T>
int BPlusTree<T>::size() const{
    int counter = 0;
    for (int i = 0; i < child_count; i++){
        counter += subset[i]->size();
    }
    return (is_leaf() ? data_count : counter);
}                    //count the number of elements

template <typename T>
bool BPlusTree<T>::empty() const{
    return data_count == 0;
}                  //true if the tree is empty

template <typename T>
void BPlusTree<T>::print_tree(int level, ostream &outs) const{
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
        if (next != NULL){
            outs << setw(4*level)<<""<<"^"<<endl;
        }
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
bool BPlusTree<T>::is_valid(){
//Check to see if all B+ tree conditions are met with the current (sub)tree:
    //Arrived at the bottom of the tree
    if (is_leaf()) {return true;}

    //data[last item] is NOT <= all subset[last subtree]->data
    if (!is_le(subset[child_count-1]->data, subset[child_count-1]->data_count, 
                                                         data[data_count-1])){
        return false;                                                    
    }
 
    //Check if all the data in this tree is greater than the childrens data
    for (int i = 0; i < data_count; i++){
        if (!is_gt(subset[i]->data, subset[i]->data_count, data[i])){
            return false;
        }
    }
    
    // //Check for every data[i] that it is in the leaves as well
    for (int i = 0; i < data_count; i++){
        T _data = data[i];
        bool leaf_data_exists = contains(_data);

        if (!leaf_data_exists){
            return false;
        }
    }
 
    //Check validity of each subsets own tree
    for (int i = 0; i < child_count; i++){
        if (!subset[i]->is_valid()){
            return false;
        }
    }
    return true;
}

template <typename T>
string BPlusTree<T>::in_order(){
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

//---------------------------------------------------------------------
//            B E G I N ( )   &   E N D ( )
//---------------------------------------------------------------------
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::begin(){
    return Iterator(get_smallest_node());
}
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::end(){
    return Iterator(NULL);
}

//---------------------------------------------------------------------
//            PRIVATE FUNCTIONS BPLUS TREE
//---------------------------------------------------------------------

template <typename T>
T* BPlusTree<T>::find_ptr(const T& entry){
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i] == entry);

    //We found the entry in this node at this tree/subtree's data[]
    if (found){
        return &data[i];
    }
    //Havn't found the entry in the tree and there is no where else to look
    if (!found && is_leaf()){
        return nullptr;
    }
    //If there is a subtree to keep searching through, search that subtree
    else {
        return subset[i]->find_ptr(entry);
    }
}                                       //return a pointer to this key.
                                        // NULL if not there.

//insert element functions
template <typename T>
void BPlusTree<T>::loose_insert(const T& entry){
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
}                                       //allows MAXIMUM+1 data elements in
                                        //   the root


//Does the linking and bringing the value up
template <typename T>
void BPlusTree<T>::fix_excess(int i){
    //Split subset data and child arrays into two arrays
    //Half of each array will go into existing subtree, the other half a new subtree 
    BPlusTree<T>* new_child = new BPlusTree<T>();
    split(subset[i]->data, subset[i]->data_count, new_child->data, new_child->data_count);
    split(subset[i]->subset, subset[i]->child_count, new_child->subset, new_child->child_count);


    //Delete the item from the child and add to the parent
    T new_parent_val;
    delete_item(new_child->data, 0, new_child->data_count, new_parent_val);
    insert_item(data, i, data_count, new_parent_val);

    //Add the new child subset into the parents list of subsets
    insert_item(subset, i+1, child_count, new_child);


    if (subset[i]->is_leaf()){
        insert_item(new_child->data, 0, new_child->data_count, new_parent_val); 

        new_child->next = subset[i]->next;
        subset[i]->next = new_child;
    }

} //fix excess in child i

template <typename T>
BPlusTree<T>* BPlusTree<T>::get_smallest_node(){
    if (is_leaf()){
        return this;
    }
    else {
        return subset[0]->get_smallest_node();
    }
}

template <typename T>
void BPlusTree<T>::get_smallest(T& entry){
    if (is_leaf()){
        entry = data[0];
        return;
    }
    else {
        return subset[0]->get_smallest(entry);
    }
}   

template <typename T>
void BPlusTree<T>::get_biggest(T& entry){
        if (is_leaf()){
        entry = data[data_count-1];
        return;
    }
    else {
        return subset[child_count-1]->get_biggest(entry);
    }
}       //entry := rightmost leaf
 
template <typename T>
void BPlusTree<T>::remove_biggest(T& entry){
        if (is_leaf()){
        detach_item(data, data_count, entry);
        return;
    }
    else {
        return subset[child_count-1]->remove_biggest(entry);
    }
}    //remove the biggest child of tree->entry
 
 template <typename T>
void BPlusTree<T>::transfer_left(int i){
    T item;
    BPlusTree<T>* new_btree = new BPlusTree<T>();

    //Delete the item from this data[] and insert it into ith childs data[]
    detach_item(data, data_count, item);
    attach_item(subset[i]->data, subset[i]->data_count, item);

    delete_item(subset[i+1]->data, 0, subset[i+1]->data_count, item);
    attach_item(data, data_count, item);

    if (subset[i+1]->child_count > 0){
        delete_item(subset[i+1]->subset, 0, subset[i+1]->child_count, new_btree);
        attach_item(subset[i]->subset, subset[i]->child_count, new_btree);
    }
}        //transfer one element LEFT from child i

template <typename T>
void BPlusTree<T>::transfer_right(int i){
    T item;
    BPlusTree<T>* new_btree = new BPlusTree<T>();
    
    //Delete the item from this data[] and insert it into ith childs data[]
    delete_item(data, 0, data_count, item);
    insert_item(subset[i]->data, 0, subset[i]->data_count, item);

    //Take the largest element and insert it into this roots data[]
    detach_item(subset[i-1]->data, subset[i-1]->data_count, item);
    insert_item(data, 0, data_count, item);

    if (subset[i-1]->child_count > 0){
        detach_item(subset[i-1]->subset, subset[i-1]->child_count, new_btree);
        insert_item(subset[i]->subset, 0, subset[i-1]->child_count, new_btree);
    }
}       //transfer one element RIGHT from child i

template <typename T>
BPlusTree<T>* BPlusTree<T>::merge_with_next_subset(int i){

} //merge subset i with  i+1



#endif//BPLUSTREE_H