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

class Node
{
public:
  int count;
  char c;
  Node *left, *mid, *right;

  char getC() { return c; };
};

class TST
{
private:
  Node *root_;
public:
  Node *put(Node *x, string key, int d);
  void put(string key);
  Node *getRoot() { return root_; };
  int get(std::string key);
  Node *get(Node *x, std::string key, int d);
  int printAll();
  void collect(Node *x, string prefix, queue<string> &q);
  ~TST();
  void del(Node *n);
};

#endif
