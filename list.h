#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//--------------------------structs--------------------------------

class List;

class Node
{
  private:
    size_t canary1_;
    char* data_;
    Node* next_;
    Node* prev_;
    size_t checksum_;

  public:
    Node();
    Node(char* data_);
    ~Node();
    friend List;
    int ChangeData(char* data);
    char* ReturnData();

    int NodeOk();
  private:
    size_t canary2_;
};

//---------------------------

class List
{
  private:
    size_t canary1_;
    Node* head_;
    Node* tail_;
    size_t count_;
    size_t checksum_;

  public:
    List();
    ~List();
    int PutInHead(Node* node);
    Node* DeleteFromHead();
    int PutInTail(Node* node);
    Node* DeleteFromTail();
    int PutBeforeCur(Node* current, Node* node);
    int PutAfterCur(Node* current, Node* node);
    int DeleteCur(Node* current);
    int PrintList();

    int ListOk();
  private:
    size_t canary2_;
};
