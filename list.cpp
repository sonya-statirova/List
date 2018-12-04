#include "list.h"

//--------------------struct-constructors---------------------------

Node::Node()
{
  canary1_ = (size_t) &canary1_;
  data_ = NULL;
  next_ = NULL;
  prev_ = NULL;
  checksum_ = (size_t) next_ ^ (size_t) prev_;
  canary2_ = (size_t) &canary2_;
}

//--------------------------

Node::Node(char* data)
{
  canary1_ = (size_t) &canary1_;
  data_ = (char*) calloc(strlen(data) + 1, sizeof(char));
  strcpy(data_, data);
  next_ = NULL;
  prev_ = NULL;
  checksum_ = (size_t) next_ ^ (size_t) prev_;
  canary2_ = (size_t) &canary2_;
}

//--------------------------

List::List()
{
  canary1_ = (size_t) &canary1_;
  head_ = NULL;
  tail_ = NULL;
  count_ = 0;
  checksum_ = ((size_t) head_ ^ (size_t) tail_) & (size_t) count_;
  canary2_ = (size_t) &canary2_;
}

//----------------------struct-destructors-------------------------

Node::~Node()
{
  canary1_ = 0;

  if (data_ != NULL)
    free(data_);

  next_ = NULL;
  prev_ = NULL;
  checksum_ = 0;
  canary2_ = 0;
}

//-------------------------

List::~List()
{
  canary1_ = 0;
  head_ = NULL;
  tail_ = NULL;
  count_ = 0;
  checksum_ = 0;
  canary2_ = 0;
}

//----------------------struct-functions-----------------------------

int Node::ChangeData(char* data)
{
  if (data_ != NULL)
    free(data_);

  data_ = (char*) calloc(strlen(data) + 1, sizeof(char));
  strcpy(data_, data);

  return 0;
}

//-----------------------------------------------

char* Node::ReturnData()
{
  return data_;
}

//-----------------------------------------------

int List::PutInHead(Node* node)
{
  if (ListOk() == -1 || node->NodeOk() == -1)
    return -1;

  if (count_ == 0)
    tail_ = node;
  else
    head_->prev_ = node;

  node->next_ = head_;
  node->prev_ = NULL;
  head_ = node;
  count_++;

  if (count_ > 1)
    node->next_->checksum_ = (size_t) (node->next_->next_) ^ (size_t) (node->next_->prev_);

  node->checksum_ = (size_t) (node->next_) ^ (size_t) (node->prev_);
  checksum_ = ((size_t) head_ ^ (size_t) tail_) & (size_t) count_;

  return 0;
}

//----------------------------------------------

Node* List::DeleteFromHead()
{
  if (ListOk() == -1)
    return NULL;

  Node* deleted = head_;

  if (count_ == 0)
    return NULL;
  else if (count_ > 1)
  {
    head_ = head_->next_;
    head_->prev_ = NULL;
    deleted->next_ = NULL;
  }
  else
  {
    head_ = NULL;
    tail_ = NULL;
  }

  count_--;

  if (count_ > 0)
    head_->checksum_ = (size_t) (head_->next_) ^ (size_t) (head_->prev_);

  deleted->checksum_ = (size_t) (deleted->next_) ^ (size_t) (deleted->prev_);
  checksum_ = ((size_t) head_ ^ (size_t) tail_) & (size_t) count_;

  return deleted;
}

//-----------------------------------------------

int List::PutInTail(Node* node)
{
  if (ListOk() == -1 || node->NodeOk() == -1)
    return -1;

  if (count_ == 0)
    head_ = node;
  else
    tail_->next_ = node;

  node->prev_ = tail_;
  node->next_ = NULL;
  tail_ = node;
  count_++;

  if (count_ > 1)
    node->prev_->checksum_ = (size_t) (node->prev_->next_) ^ (size_t) (node->prev_->prev_);

  node->checksum_ = (size_t) (node->next_) ^ (size_t) (node->prev_);
  checksum_ = ((size_t) head_ ^ (size_t) tail_) & (size_t) count_;

  return 0;
}

//----------------------------------------------

Node* List::DeleteFromTail()
{
  if (ListOk() == -1)
    return NULL;

  Node* deleted = tail_;

  if (count_ == 0)
    return NULL;
  else if (count_ > 1)
  {
    tail_ = tail_->prev_;
    tail_->next_ = NULL;
    deleted->prev_ = NULL;
  }
  else
  {
    head_ = NULL;
    tail_ = NULL;
  }

  count_--;

  if (count_ > 0)
    tail_->checksum_ = (size_t) (tail_->next_) ^ (size_t) (tail_->prev_);

  deleted->checksum_ = (size_t) (deleted->next_) ^ (size_t) (deleted->prev_);
  checksum_ = ((size_t) head_ ^ (size_t) tail_) & (size_t) count_;

  return deleted;
}

//-----------------------------------------------

int List::PutBeforeCur(Node* current, Node* node)
{
  if (ListOk() == -1 || current->NodeOk() == -1 || node->NodeOk() == -1)
    return -1;

  if (head_ == current)
    head_ = node;
  else
    current->prev_->next_ = node;

  node->prev_ = current->prev_;
  node->next_ = current;
  current->prev_ = node;

  count_++;

  if (node->prev_ != NULL)
    node->prev_->checksum_ = (size_t) (node->prev_->next_) ^ (size_t) (node->prev_->prev_);

  node->checksum_ = (size_t) (node->next_) ^ (size_t) (node->prev_);
  current->checksum_ = (size_t) (current->next_) ^ (size_t) (current->prev_);
  checksum_ = ((size_t) head_ ^ (size_t) tail_) & (size_t) count_;

  return 0;
}

//---------------------------------------------

int List::PutAfterCur(Node* current, Node* node)
{
  if (ListOk() == -1 || current->NodeOk() == -1 || node->NodeOk() == -1)
    return -1;

  if (tail_ == current)
    tail_ = node;
  else
    current->next_->prev_ = node;

  node->next_ = current->next_;
  node->prev_ = current;
  current->next_ = node;

  count_++;

  if (node->next_ != NULL)
    node->next_->checksum_ = (size_t) (node->next_->next_) ^ (size_t) (node->next_->prev_);

  node->checksum_ = (size_t) (node->next_) ^ (size_t) (node->prev_);
  current->checksum_ = (size_t) (current->next_) ^ (size_t) (current->prev_);
  checksum_ = ((size_t) head_ ^ (size_t) tail_) & (size_t) count_;

  return 0;
}

//---------------------------------------------

int List::DeleteCur(Node* current)
{
  if (ListOk() == -1 || current->NodeOk() == -1)
    return -1;

  if (current->prev_ != NULL && current->next_ != NULL)
    {
      current->prev_->next_ = current->next_;
      current->next_->prev_ = current->prev_;
      current->prev_->checksum_ = (size_t) (current->prev_->next_) ^ (size_t) (current->prev_->prev_);
      current->next_->checksum_ = (size_t) (current->next_->next_) ^ (size_t) (current->next_->prev_);
    }
  else if (current->prev_ != NULL)
  {
    tail_ = current->prev_;
    current->prev_->next_ = NULL;
    current->prev_->checksum_ = (size_t) (current->prev_->next_) ^ (size_t) (current->prev_->prev_);
  }
  else if (current->next_ != NULL)
  {
    head_ = current->next_;
    current->next_->prev_ = NULL;
    current->next_->checksum_ = (size_t) (current->next_->next_) ^ (size_t) (current->next_->prev_);
  }
  else
  {
    head_ = NULL;
    tail_ = NULL;
  }

  current->prev_ = NULL;
  current->next_ = NULL;

  count_--;

  current->checksum_ = (size_t) (current->next_) ^ (size_t) (current->prev_);
  checksum_ = ((size_t) head_ ^ (size_t) tail_) & (size_t) count_;

  return 0;
}

//-----------------------------------------------

int Node::NodeOk()
{
  if (canary1_ != (size_t) &canary1_ || canary2_ != (size_t) &canary2_ \
|| checksum_ != ((size_t) next_ ^ (size_t) prev_))
    return -1;
  else
    return 0;
}

//----------------------------------------------

int List::ListOk()
{
  if (canary1_ != (size_t) &canary1_ || canary2_ != (size_t) &canary2_ \
|| checksum_ != (((size_t) head_ ^ (size_t) tail_) & (size_t) count_))
    return -1;
  else
    return 0;
}

//----------------------------------------------

int List::PrintList()
{
  if (ListOk() == -1)
    return -1;

  if (count_ == 0)
    return 0;

  size_t i = 1;
  Node* current = head_;
  //Node* cur = tail_;
  if (current->NodeOk() == -1)
    return -1;

  printf("%lu. %s\n", i, current->data_);

  for (i = 2; i <= count_; i++)
  //for (i = count_; i >= 2; i--)
  {
    current = current->next_;
    //cur = cur->prev_;
    if (current->NodeOk() == -1)
      return -1;

    printf("%lu. %s\n", i, current->data_);
  }

  return 0;
}
