#include <iostream>
#include "btree.h"
#include "bstfunctions.h"

using namespace std;
void testPrint();

int main()
{

}

testPrint(){

    //USE THIS TO TEST YOUR PRINT FUNCTIONS

    //ROOT
    BTree<int> top;
    top.data[0] = 50;
    top.data[1] = 70;
    top.data_count = 2;
    top.child_count = 3;

    //LEFT CHILD
    BTree<int> left;
    left.data[0] = 20;
    left.data[1] = 30;
    left.data_count = 2;
    left.child_count = 0;

    //MIDDLE CHILD
    BTree<int> middle;
    middle.data[0] = 60;
    middle.data[1] = 65;
    middle.data_count = 2;
    middle.child_count = 0;

    //RIGHT CHILD
    BTree<int> right;
    right.data[0] = 60;
    right.data[1] = 65;
    right.data_count = 2;
    right.child_count = 0;

    //ATTACH CHILDREN TO PARENT
    top.subset[0] = &left;
    top.subset[1] = &middle;
    top.subset[2] = &right;

    //PRINT TREE
    cout<<top;
}
