#ifndef BUGGY_H
#define BUGGY_H

#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <queue>
#include <iostream>

using namespace std;

struct Word
{
  char * data;
  int count;

  Word ( char * data_ ) :
    data( ::strdup(data_) )
  {};
  Word () :
    data((char *)"")
  {};
  ~Word() {
       if (data) delete data;
  };
  Word( Word & source){ data = new char[ strlen(source.data)];
                        strcpy(data, source.data); };

};

class Node 
{
  public:
    int count;
    char c;
    Node * left, *mid, *right;
    
    char getC(){return c;};
};

class TST
{
  Node *root_;
 public:
  Node * put(Node * x, string key, int d);
  void put(string key);
  Node * getRoot(){ return root_;};
  bool contains(string key);
  int get(std::string key) ;
  Node * get(Node  *x, std::string key, int d) ;
  int printAll();
  void collect(Node * x, string prefix,  queue<string>& q);
  ~TST();
  void del(Node * n);

};

#endif
