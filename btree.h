#ifndef BTREE_H
#define BTREE_H
#include <iostream>
#include <iomanip>
#include "arrayfunct.h"

template <class T>
class BTree
{
public:
    BTree(bool dups = false){
        data_count = 0;
        child_count = 0;
        dups_ok = dups;
    }
    //big three:
    BTree(const BTree<T>& other){
        data_count = 0;
        child_count = 0;
        this->clear_tree();
        this->copy_tree(other);

    }
    ~BTree(){
        clear_tree();
    }
    BTree<T>& operator =(const BTree<T>& RHS){
        this->clear_tree();
        this->copy_tree(RHS);
        this->dups_ok = RHS.dups_ok;
        return *this;
    }

    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree

    void clear_tree();                          //clear this object (delete all nodes etc.)
    void copy_tree(const BTree<T>& other);      //copy other into this object

    bool contains(const T& entry);              //true if entry can be found in the array
    T& get(const T& entry);                     //return a reference to entry in the tree
    T* find(const T& entry);                    //return a pointer to this key. NULL if not there.

    int size() const;                           //count the number of elements in the tree
    bool empty() const;                         //true if the tree is empty

    void print_tree(int level, ostream &outs=cout) const; //print a readable version of the tree

    bool is_valid();
    friend ostream& operator<<(ostream& outs, const BTree<T>& print_me){
        //        cout<<"[---"<<print_me.child_count<<", "<<print_me.data_count<<"---]"<<endl;;
        print_me.print_tree(0, outs);
        //        cout<<"- - - - - - "<<endl;
        return outs;
    }

public:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                                   //true if duplicate keys may be inserted
    int data_count=0;                                 //number of data elements
    T data[MAXIMUM + 1];                            //holds the keys
    int child_count=0;                                //number of children
    BTree* subset[MAXIMUM + 2];                     //subtrees

    bool is_leaf() const {return child_count==0;}   //true if this is a leaf node

    //insert element functions
    void loose_insert(const T& entry);              //allows MAXIMUM+1 data elements in the root
    void fix_excess(int i);                         //fix excess of data elements in child i

    //remove element functions:
    void loose_remove(const T& entry);              //allows MINIMUM-1 data elements in the root
    void fix_shortage(int i);                       //fix shortage of data elements in child i

    void remove_biggest(T& entry);                  //remove the biggest child of this tree->entry
    void rotate_left(int i);                        //transfer one element LEFT from child i
    void rotate_right(int i);                       //transfer one element RIGHT from child i
    void merge_with_next_subset(int i);             //merge subset i with subset i+1

};

template <class T>
void BTree<T>::print_tree(int level, ostream &outs) const{

    //print if leaf
    if (is_leaf()){
        for(int i = data_count-1; i >= 0; i--){
            outs << setw(4*level) << "[" << data[i]  << "]\n";
        }

    }

    else {
        // recursive call to right child
        subset[child_count-1]->print_tree(level+1, outs);

        //print all the rest of the data and children
        for(int i = data_count-1; i >= 0; i--){

            //print yourself
            outs << setw(4*level) << "[" << data[i] <<"]\n";

            // recursive call to left child
            subset[i]->print_tree(level+1, outs);

        }
    }
}

template <class T>
void  BTree<T>::insert(const T& entry){

    loose_insert(entry);

    //GROW TREE
    if (data_count>MAXIMUM){
        BTree<T>* newRoot = new BTree<T>;
        copy_array(newRoot->data,data,newRoot->data_count,data_count);
        copy_array(newRoot->subset,subset,newRoot->child_count,child_count);
        child_count = 1;
        data_count = 0;
        this->subset[0] = newRoot;
        fix_excess(0);
    }

}

template <class T>
void BTree<T>::loose_insert(const T& entry){

    int i = first_ge(data, data_count, entry);

    bool found = (i<data_count && data[i] == entry);

    if (found){
        //do something for dupes
    }

    else if (!found && is_leaf()){
        ordered_insert(data,data_count,entry);

    }

    else if(!is_leaf()){
        subset[i]->loose_insert(entry);

        if (subset[i]->data_count>MAXIMUM){
            fix_excess(i);
        }
    }


}

template <class T>
void BTree<T>::fix_excess(int i){

    //this node's child i has one too many items: 3 steps:
    //1. add a new subset at location i+1 of this node
    //2. split subset[i] (both the subset array and the data array) and move half into
    // subset[i+1] (this is the subset we created in step 1.)
    //3. detach the last data item of subset[i] and bring it and insert it into this node's data[]
    // //Note that this last step may cause this node to have too many items. This is OK. This will be
    // dealt with at the higher recursive level. (my parent will fix it!)


    // add blank node to child's right data
    insert_item(subset,i+1,child_count,new BTree<T>);

    //split the left child's info to the blank right
    split(subset[i]->subset, subset[i]->child_count, subset[i+1]->subset, subset[i+1]->child_count);
    split(subset[i]->data, subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);

    // now detach the last item on left child's data
    T item;
    detach_item(subset[i]->data, (subset[i]->data_count), item);

    // insert it into parent's node data
    insert_item(data, i, data_count,item);


}

template <class T>
void BTree<T>::clear_tree(){
    //checks to see if its leaf this is also the base case
    if (is_leaf()){
        return;
    }

    //deletes children from parent's node
    for (int i=0; i<child_count; i++){
        subset[i]->clear_tree();
        subset[i]->data_count = 0;
        delete subset[i];
    }
    child_count = 0;
    data_count = 0;

}


template <class T>
void BTree<T>::loose_remove(const T& entry){

    //    cout << "in remove\n";

    //gets index of first larger item encountered
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i] == entry);

    // case 1: item not found
    if (is_leaf() && !found){
        //nothing to do, item not found
    }

    // case 2: leaf and found
    else if (is_leaf() && found){
        //        cout << "found and is leaf\n";

        //since its a leaf we simply delete the item
        T temp;
        delete_item(data,i,data_count,temp);
        //        cout << "returned after delete: " << data_count << endl;
    }

    // case 3: not a leaf and not found
    if (!is_leaf() &&!found){
        //        cout << "not found, recursion \n";

        //recursively check subset to find item
        subset[i]->loose_remove(entry);

        //after removing check to see if subset's data_count is less than minimum if so fix shortage
        if(subset[i]->data_count < MINIMUM){
            fix_shortage(i);
        }
    }

    //case 4: found and not a leaf// inner node
    else if(found && !is_leaf()){
        T item = entry;

        //get the biggest item from subset
        subset[i]->remove_biggest(item);

        //set data[i] to it
        data[i] = item;

        //        cout << "item: " << item << endl;

        //if subset[i].datacount is less than minimum fix shortage
        if(subset[i]->data_count < MINIMUM){
            //            cout << "subset i datacount below minimum, fix shortage! \n";
            fix_shortage(i);
        }
    }
}

template <class T>
void  BTree<T>::remove_biggest(T& entry){
    if (!is_leaf()){
        //always goes to furtherst right child
        subset[child_count-1]->remove_biggest(entry);

        //checks to see if rights child's datacount is now less than minimum and fixes shortage
        if (subset[child_count-1]->data_count< MINIMUM){
            fix_shortage(child_count-1);
        }
    }

    else {
        //is leaf so detach item and set it equal to entry
        detach_item(data,data_count,entry);
    }

}

template <class T>
void  BTree<T>::fix_shortage(int i){

    // i == 0 checking to see if we can borrow from right child
    if(i < child_count-1 && subset[i+1]->data_count > MINIMUM){
        cout<<"rotate left"<<endl;
        rotate_left(i);
        print_tree(0);

    }


    //  checking to see if we can borrow from left child
    else if(i > 0 && subset[i-1]->data_count > MINIMUM){
        //        cout<<"rotate right"<<endl;
        rotate_right(i);
        print_tree(0);
    }


    else if(i==0){
        //        cout<<"merge to the right"<<endl;
        merge_with_next_subset(i);
        print_tree(0);


    }

    else {
        //        cout<<"merge left"<<endl;

        T entry;
        // detach & store item at root (to move down)
        delete_item(data, i-1, data_count, entry);

        // insert item at left child
        ordered_insert(subset[i-1]->data, subset[i-1]->data_count, entry);

        // append child[i+1](right) to child[i](left)
        merge(subset[i-1]->data, subset[i-1]->data_count, subset[i]->data, subset[i]->data_count);
        merge(subset[i-1]->subset, subset[i-1]->child_count, subset[i]->subset, subset[i]->child_count);
        BTree<T>* temp;

        // delete child[i] since merged

        delete_item(subset, i, child_count, temp);
        delete temp;
        print_tree(0);

    }
}

template <class T>
void  BTree<T>::rotate_left(int i){
    //    cout<<"rotate left"<<endl;

    //delete item at data
    T item1;
    delete_item(data,i,data_count,item1);

    //insert it into subset[i] at first pos
    insert_item(subset[i]->data,0,subset[i]->data_count, item1);

    //delete item subset[i+1] at first pos
    T item2;
    delete_item(subset[i+1]->data,0,subset[i+1]->data_count,item2);
    ordered_insert(data, data_count, item2);

    //if subset[i+1] isnt a leaf move its left subchild to subset[i]'s
    if(!subset[i+1]->is_leaf()){
        BTree<T>* tempPtr;

        //setting tempPtr to the rights 0 subset
        delete_item(subset[i+1]->subset, 0, subset[i+1]->child_count, tempPtr);

        //attaching the tempPtr to the lefts subset on the furthest right
        attach_item(subset[i]->subset, subset[i]->child_count, tempPtr);
    }
}

template <class T>
void  BTree<T>::rotate_right(int i){

    //delete item at data
    T item1;
    detach_item(data, data_count, item1);

    //insert it into subset[i] at first pos

    //delete item subset[i] at first pos
    insert_item(subset[i]->data, 0, subset[i]->data_count, item1);
    T item2;
    detach_item(subset[i-1]->data, subset[i-1]->data_count, item2);
    ordered_insert(data, data_count, item2);

    //if subset[i-1] isnt a leaf move its left subchild to subset[i]'s
    if(!subset[i-1]->is_leaf()){
        BTree<T>* tempPtr;

        // detach left child's greatest child
        detach_item(subset[i-1]->subset, subset[i-1]->child_count, tempPtr);

        // insert that into right child's at 0 position
        insert_item(subset[i]->subset, 0, subset[i]->child_count, tempPtr);
    }
}

template <class T>
void  BTree<T>::merge_with_next_subset(int i){
    //    cout << "in merge\n";
    T entry;

    // delete & store item at root (to move down)
    delete_item(data, i, data_count, entry);

    // insert item at child i
    ordered_insert(subset[i]->data, subset[i]->data_count, entry);

    // apprents left's data/subset to right's right side
    BTree<T>* temp=subset[i+1];
    merge(subset[i]->data, subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);
    if(subset[i+1]->child_count>0){
        merge(subset[i]->subset, subset[i]->child_count, subset[i+1]->subset, subset[i+1]->child_count);
    }
    BTree<T>* p;

    // delete right child since merged
    delete_item(subset, i+1, child_count, p);
    delete temp;
    temp = NULL;
}

template <class T>
void  BTree<T>::remove(const T& entry){
    //Loose_remove the entry from this tree.
    loose_remove(entry);

    //once you return from loose_remove, the root (this object) may
    //have no data and only a single subset now, the tree must shrink:
    if(data_count < MINIMUM && child_count >0){
        //        cout<<"shrinking"<<endl;


        //set tempPtr to only child
        BTree<T>* shrinkPtr = subset[0];

        //copy all of child's data to root
        copy_array(data, shrinkPtr->data, data_count, shrinkPtr->data_count);
        copy_array(subset, shrinkPtr->subset, child_count, shrinkPtr->child_count);

        //delete the temPtr which is already copied
        shrinkPtr->child_count = 0;
        delete shrinkPtr;
        shrinkPtr = NULL;
    }
}
template <class T>
T* BTree<T>::find(const T& entry){
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i] == entry);

    if (found){
        return &data[i];
    }

    else if (!found && is_leaf()){
        return NULL;

    }

    else if(!is_leaf()){
        subset[i]->find(entry);

    }

    else {
        return NULL;
    }
}


template <typename T>
bool BTree<T>::is_valid(){
    if(data_count > 0 && child_count >= 0)
        return true;
    else if(data_count == 0 && child_count == 0)
        return true;
    else
        return false;
}
template <typename T>
void BTree<T>::copy_tree(const BTree<T>& other){


    copy_array(data,other.data,data_count,other.data_count);
    child_count = other.child_count;

    for (int i=0; i< child_count; i++){
        subset[i] = new BTree<T>;
        subset[i]->copy_tree(*other.subset[i]);
    }


}

#endif // BTREE_H

