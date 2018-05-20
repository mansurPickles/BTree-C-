#ifndef ARRAYFUNCT_H
#define ARRAYFUNCT_H
#include "iostream"
#include "string.h"

using namespace std;

template <class T>
T maximal(const T& a, const T& b);                      //return the larger of the two items

//swap the two items
template <class T>
void swaps(T& a, T& b){
    T temp = a;
    a = b;
    b = temp;
}

//return index of the largest item in data
template <class T>
int index_of_maximal(T data[ ], int n){

    int highest = 0;
    for (int i=0; i< n; i++){
        if (data[i]>highest){
            highest = i;
        }
    }
    return highest;
}

//insert entry into the sorted array data with length n
template <class T>
void ordered_insert(T data[], int& n, T entry);

template <class T>
int first_ge(const T data[ ], int n, const T& entry);   //return the first element in data that is
//not less than entry

template <class T>
void attach_item(T data[ ], int& n, const T& entry);    //append entry to the right of data

//template <class T>
//void insert_item(T* data, int i, int& n, T entry);    //insert entry at index i in data

template <class T>
void detach_item(T data[ ], int& n, T& entry);          //remove the last element in data and place
//it in entry

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry);   //delete item at index i and place it in entry

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2);   //append data2 to the right of data1

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2);   //move n/2 elements from the right of data1
//and move to data2

template <class T>
void copy_array(T dest[], const T src[],
                int& dest_size, int src_size);              //copy src[] into dest[]

template <class T>
void print_array(const T data[], int n, int pos = -1);  //print array data

template <class T>
bool is_gt(const T data[], int n, const T& item);       //item > all data[i]

template <class T>
bool is_le(const T data[], int n, const T& item);       //item <= all data[i]

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list); //print vector list

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme); //list.push_back addme



template <class T>
void ordered_insert(T data[], int& n, T entry){

    data[n] = entry;

    for (int i=n; i>0; i--){
        if (data[i] < data[i-1] && i!=0){
            swaps(data[i],data[i-1]);
        }
    }
    n++;
}



template <class T>
void insert_item(T* data, int i, int& n, T entry){

    int m = n;
    while (m > i){
        data[m] = data[m-1];
        m--;
    }
    data[i] = entry;
    n++;

}

template <class T>
int first_ge(const T data[ ], int n, const T& entry){


    for (int i=0; i< n; i++){
        if (data[i] >= entry){
            return i;
        }
    }
}


template <class T>
void attach_item(T data[ ], int& n, const T& entry){
    data[n] = entry;
    n++;
}

template <class T>
void detach_item(T data[ ], int& n, T& entry){
    entry = data[n-1];
    n--;
}

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2){

    int size = n1/2;
    int start = n1 - size;

    for (int i= 0; i<size; i++){
        data2[i] = data1[start + i];
    }
    n2 = size;
    n1 = n1 - n2;
    return;

}

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2){

    int end = n2;

    for (int i=0; i<end; i++){
        data1[n1] = data2[i];
        n1++;
        n2--;
    }

}

template <class T>
void copy_array(T dest[], const T src[], int& dest_size, int src_size){

    for (int i=0; i< src_size; i++){
        dest[i] = src[i];
    }
    dest_size = src_size;

}

template <class T>
bool is_gt(const T data[], int n, const T& item){
    bool gt = true;

    for (int i=0; i< n; i++){
        if (item<data[i]){
            gt = false;
            return gt;
        }
    }
    return gt;
}

template <class T>
bool is_le(const T data[], int n, const T& item){
    bool le = true;

    for (int i=0; i<n; i++){
        if (item>data[i]){
            le = false;
            return le;
        }

        else {
            return le;
        }
    }
}

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry){

    entry = data[i];

    for (int j=i; j<n; j++){
        data[j] = data[j+1];
    }
    n--;
}

template <class T>
void print_array(const T data[], int n, int pos){
    for (int i=0; i< n; i++){
        cout << data[i] << endl;
    }
}





#endif // ARRAYFUNCT_H
