#include <stdio.h>
#include <stdlib.h>

#define BUFLEN 100
#define DATALEN 30

//--------------------------structs--------------------------------

struct Node
{
  size_t canary1;
  char* data;
  Node* next;
  Node* prev;
  Node();
  size_t checksum;
  size_t canary2;
};

//---------------------------

struct List
{
  size_t canary1;
  Node* head;
  Node* tail;
  unsigned count;
  List();
  size_t checksum;
  size_t canary2;
};

//--------------------struct-constructors---------------------------

Node::Node()
{
  canary1 = (size_t) &canary1;
  data = NULL;
  next = NULL;
  prev = NULL;
  checksum = (size_t) next ^ (size_t) prev;
  canary2 = (size_t) &canary2;
}

//--------------------------

List::List()
{
  canary1 = (size_t) &canary1;
  head = NULL;
  tail = NULL;
  count = 0;
  checksum = ((size_t) head ^ (size_t) tail) & (size_t) count;
  canary2 = (size_t) &canary2;
}

//-----------------------functions-------------------------------

int PutInHead(List* strings, Node* element)
{
  element->next = strings->head;
  element->prev = NULL;
  strings->head = element;
  (strings->count)++;

  element->checksum = (size_t) (element->next) ^ (size_t) (element->prev);
  strings->checksum = ((size_t) (strings->head) ^ (size_t) (strings->tail)) & (size_t) (strings->count);

  return 0;
}

//----------------------------------------------

Node* DeleteFromHead(List* strings)
{
  Node* deleted = strings->head;

  if (strings->count == 0)
    return NULL;
  else if (strings->count > 1)
    strings->head = strings->head->next;
  else
  {
    strings->head = NULL;
    strings->tail = NULL;
  }

  (strings->count)--;

  strings->checksum = ((size_t) (strings->head) ^ (size_t) (strings->tail)) & (size_t) (strings->count);

  return deleted;
}

//-----------------------------------------------

int PutInTail(List* strings, Node* element)
{
  element->prev = strings->tail;
  element->next = NULL;
  strings->tail = element;
  (strings->count)++;

  element->checksum = (size_t) (element->next) ^ (size_t) (element->prev);
  strings->checksum = ((size_t) (strings->head) ^ (size_t) (strings->tail)) & (size_t) (strings->count);

  return 0;
}

//----------------------------------------------

Node* DeleteFromTail(List* strings)
{
  Node* deleted = strings->tail;

  if (strings->count == 0)
    return NULL;
  else if (strings->count > 1)
    strings->tail = strings->tail->prev;
  else
  {
    strings->head = NULL;
    strings->tail = NULL;
  }

  (strings->count)--;

  strings->checksum = ((size_t) (strings->head) ^ (size_t) (strings->tail)) & (size_t) (strings->count);

  return deleted;
}

//-----------------------------------------------

int PutBeforeCur(List* strings, Node* current, Node* element)
{
  if (strings->head == current)
    strings->head = element;

  element->prev = current->prev;
  element->next = current;
  current->prev = element;
  (strings->count)++;

  element->checksum = (size_t) (element->next) ^ (size_t) (element->prev);
  current->checksum = (size_t) (current->next) ^ (size_t) (current->prev);
  strings->checksum = ((size_t) (strings->head) ^ (size_t) (strings->tail)) & (size_t) (strings->count);

  return 0;
}

//---------------------------------------------

int PutAfterCur(List* strings, Node* current, Node* element)
{
  if (strings->tail == current)
    strings->tail = element;

  element->next = current->next;
  element->prev = current;
  current->next = element;
  (strings->count)++;

  element->checksum = (size_t) (element->next) ^ (size_t) (element->prev);
  current->checksum = (size_t) (current->next) ^ (size_t) (current->prev);
  strings->checksum = ((size_t) (strings->head) ^ (size_t) (strings->tail)) & (size_t) (strings->count);

  return 0;
}

//---------------------------------------------

int DeleteCur(List* strings, Node* current)
{
  if (current->prev != NULL && current->next != NULL)
    {
      current->prev->next = current->next;
      current->next->prev = current->prev;
    }
  else if (current->prev != NULL)
  {
    strings->tail = current->prev;
    current->prev->next = NULL;
  }
  else if (current->next != NULL)
  {
    strings->head = current->next;
    current->next->prev = NULL;
  }
  else
  {
    strings->head = NULL;
    strings->tail = NULL;
  }

  (strings->count)--;

  strings->checksum = ((size_t) (strings->head) ^ (size_t) (strings->tail)) & (size_t) (strings->count);

  return 0;
}

//-----------------------------------------------

int NodeOk(Node* element)
{
  if (element->canary1 != (size_t) &(element->canary1) || element->canary2 != (size_t) &(element->canary2) \
  || element->checksum != ((size_t) (element->next) ^ (size_t) (element->prev)))
    return -1;
  else
    return 0;
}
//----------------------------------------------

int ListOk(List* strings)
{
  if (strings->canary1 != (size_t) &(strings->canary1) || strings->canary2 != (size_t) &(strings->canary2) \
  || strings->checksum != (((size_t) (strings->head) ^ (size_t) (strings->tail)) & (size_t) (strings->count)))
    return -1;
  else
    return 0;
}

//-------------------------functions---------------------------

Node* MakeNewNode(FILE* file)
{
  Node* element;
  element = (Node*) calloc(1, sizeof(Node));
  element->data = (char*) calloc(DATALEN, sizeof(char));

  if (fscanf(file, "%[A-Za-z]", element->data) != EOF)
  {
    printf("I\'m alive\n");
    return element;
  }
  else
    return NULL;
}

//-----------------------------------

int Hash2(Node* element)
{
  int len = 0;
  for (len = 0; element->data[len] !='\0'; len++)
    ;

  return (len % BUFLEN);
}

//----------------------------------

int Distribution2(List* buf2, FILE* file)
{
  Node* element = MakeNewNode(file);
  while (element != NULL)
  {
    PutInTail(&(buf2[Hash2(element)]), element);
    element = MakeNewNode(file);
  }

  return 0;
}

//-----------------------------------

int PrintResult(List* buf)
{
  Node* cur = NULL;

  for (int i = 0; i < BUFLEN; i++)
  {
    printf("%d. ", i);
    cur = buf[i].head;
    while (cur != NULL)
    {
      printf("%s ", cur->data);
      cur = cur->next;
    }
    printf("\n");
  }
  printf("\n");

  return 0;
}

//---------------------------main-------------------------------

int main()
{
  FILE* file = fopen("text.txt", "r");
  List* buf2 = (List*) calloc(BUFLEN, sizeof(List));
  printf("aaa\n");
  Distribution2(buf2, file);
  printf("Aaa\n");
  PrintResult(buf2);
  printf("AAA\n");
  return 0;
}
