#include <iostream>
#include "arrayfunct.h"
#include "btree.h"
#include "string.h"

using namespace std;

void printf();
void test_btree_auto(int tree_size=5000, int how_many=500, bool report=false);
bool test_btree_auto(int how_many, bool report=true);
int Random(int lo, int hi);
void btree_test();

int main(){

}


int Random(int lo, int hi)
{
    int r = rand()%(hi-lo+1)+lo;

    return r;
}

void btree_test(){
    cout <<endl<<endl<<endl<< "===============================" << endl<<endl<<endl<<endl;

    //------------------------------------------
    srand(time(0));
    //------------------------------------------


    test_btree_auto(1000,10000,true);

    cout <<endl<<endl<<endl<< "===============================" << endl;
}

void test_btree_auto(int tree_size, int how_many, bool report){
    bool verified = true;
    for (int i = 0; i< how_many; i++){
        if (report){
            cout<<"*********************************************************"<<endl;
            cout<<" T E S T:    "<<i<<endl;
            cout<<"*********************************************************"<<endl;
        }
        if (!test_btree_auto(tree_size, report)){
            cout<<"T E S T :   ["<<i<<"]    F A I L E D ! ! !"<<endl;
            verified = false;
            return;
        }

    }
    cout<<"**************************************************************************"<<endl;
    cout<<"**************************************************************************"<<endl;
    cout<<"             E N D     T E S T: "<<how_many<<" tests of "<<tree_size<<" items: ";
    cout<<(verified?"VERIFIED": "VERIFICATION FAILED")<<endl;
    cout<<"**************************************************************************"<<endl;
    cout<<"**************************************************************************"<<endl;

}

bool test_btree_auto(int how_many, bool report){
    const int MAX = 1000;
//    assert(how_many < MAX);
    BTree<int> bt;
    int a[MAX];
    int original[MAX];
    int deleted_list[MAX];

    int original_size;
    int size;
    int deleted_size = 0;

    //fill a[ ]
    for (int i= 0; i< how_many; i++){
        a[i] = i;
    }
    //shuffle a[ ]: Put this in a function!
    for (int i = 0; i< how_many; i++){
        int from = Random(0, how_many-1);
        int to = Random(0, how_many -1);
        int temp = a[to];
        a[to] = a[from];
        a [from] = temp;
    }
    //copy  a[ ] -> original[ ]:
    size = how_many;
    original_size = how_many;
    copy_array(original, a, original_size, size);
    for (int i=0; i<size; i++){
        bt.insert(a[i]);

    }
    if (report){
        cout<<"========================================================"<<endl;
        cout<<"  "<<endl;
        cout<<"========================================================"<<endl;
        cout<<bt<<endl<<endl;
    }
    for (int i= 0; i<how_many; i++){
        int r = Random(0, how_many - i - 1);
        if (report){
            cout<<"========================================================"<<endl;
//            cout<<bt<<endl;
            cout<<". . . . . . . . . . . . . . . . . . . . . . . . . . . . "<<endl;
//            cout<<"deleted: "; print_array(deleted_list, deleted_size);
//            cout<<"   from: "; print_array(original, original_size);
            cout<<endl;
//            cout<<"  REMOVING ["<<a[r]<<"]"<<endl;
            cout<<"========================================================"<<endl;
        }
        bt.remove(a[r]);

//        cout << "after remove: " << a[r] << endl;
//        cout << bt << endl;


        delete_item(a, r, size, deleted_list[deleted_size++]);

        if (!bt.is_valid()){
            cout<<setw(6)<<i<<" I N V A L I D   T R E E"<<endl;
//            cout<<"Original Array: "; print_array(original, original_size);
//            cout<<"Deleted Items : "; print_array(deleted_list, deleted_size);
            cout<<endl<<endl<<bt<<endl<<endl;
            return false;
        }

    }

    if (report) cout<<" V A L I D    T R E E"<<endl;
    return true;
}

void printf(){
    BTree<int> top;
    top.data[0] = 50;
    top.data[1] = 70;
    top.data_count = 2;
    top.child_count = 3;

    BTree<int> left;
    left.data[0] = 20;
    left.data[1] = 30;
    left.data_count = 2;
    left.child_count = 0;

    BTree<int> middle;
    middle.data[0] = 60;
    middle.data[1] = 65;
    middle.data_count = 2;
    middle.child_count = 0;
    BTree<int> right;
    right.data[0] = 80;
    right.data[1] = 85;
    right.data_count = 2;
    right.child_count = 0;
    top.subset[0] = &left;
    top.subset[1] = &middle;
    top.subset[2] = &right;
    cout<<top;

    cout << "\n\n\n=================END=============\n\n\n";
}

