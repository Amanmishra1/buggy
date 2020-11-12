#include "buggy.h"
#include "word.h"

std::mutex g_lockTST;

static TST s_wordsTST;
static Word s_word;
static int s_totalFound;

TST::~TST()
{
  if (root_ != NULL)
  {
    del(root_);
  }
}

void TST::del(Node *n)
{
  if (n == NULL)
    return;
  del(n->left);
  del(n->mid);
  del(n->right);
  delete n;
}

Node *TST::put(Node *x, string key, int d)
{
  char c = key.at(d);
  if (x == NULL)
  {
    x = new Node();
    x->c = c;
    x->count = 0;
  }
  if (c < x->c)
    x->left = put(x->left, key, d);
  else if (c > x->c)
    x->right = put(x->right, key, d);
  else if (d < key.length() - 1)
    x->mid = put(x->mid, key, d + 1);
  else
    x->count++;
  return x;
}

void TST::put(string key)
{
  root_ = put(root_, key, 0);
}

int TST::get(string key)
{
  Node *x = get(root_, key, 0);
  if (x == NULL)
    return 0;
  return x->count;
}

Node *TST::get(Node *x, string key, int d)
{
  if (!x)
    return x;
  char c = key.at(d);
  if (c < x->c)
    return get(x->left, key, d);
  else if (c > x->c)
    return get(x->right, key, d);
  else if (d < key.length() - 1)
    return get(x->mid, key, d + 1);
  else
    return x;
}

int TST::printAll()
{
  queue<string> collectAll;
  collect(root_, "", collectAll);
  if (collectAll.empty())
    return 0;
  int size = collectAll.size();
  while (collectAll.size() > 0)
  {
    std::cout << "\n  " << collectAll.front();
    collectAll.pop();
  }
  printf("\n");
  return size;
}

void TST::collect(Node *x, string prefix, queue<string> &q)
{
  if (x == NULL)
    return;
  char c = x->c;
  ;
  if (x->count > 0)
    q.push(prefix + c);
  collect(x->left, prefix, q);

  collect(x->mid, prefix + c, q);

  collect(x->right, prefix, q);
}

// Worker thread: consume words passed from the main thread and insert them
// in the 'word list' (s_wordsTST), while removing duplicates. Terminate when
// the word 'end' is encountered.
static void workerThread()
{
  bool endEncountered = false;
  bool found = false;

  while (!endEncountered)
  {
    if (s_word.data_[0]) // Do we have a new word?
    {
      Word *w = new Word(s_word); // Copy the word

      s_word.data_[0] = 0; // Inform the producer that we consumed the word

      endEncountered = std::strcmp(w->data_, "end") == 0;

      if (!endEncountered)
      {
        g_lockTST.lock();
        s_wordsTST.put(string(w->data_));
        g_lockTST.unlock();
      }
      delete w;
    }
  }
};

// Read input words from STDIN and pass them to the worker thread for
// inclusion in the word list.
// Terminate when the word 'end' has been entered.
//
static void readInputWords()
{
  bool endEncountered = false;

  std::thread *worker = new std::thread(workerThread);

  // std::string str;
  // std::getline(std::cin, str);

  char *linebuf = new char[32];
  s_word.data_ = new char[32];

  while (!endEncountered)
  {
    if (!std::gets(linebuf)) // EOF?
      return;

    endEncountered = std::strcmp(linebuf, "end") == 0;

    // Pass the word to the worker thread
    std::strcpy(s_word.data_, linebuf);
    while (s_word.data_[0])
      ; // Wait for the worker thread to consume it
  }

  worker->join(); // Wait for the worker to terminate
}

// Repeatedly ask the user for a word and check whether it was present in the word list
// Terminate on EOF
//

static void lookupWords()
{
  bool found;
  char *linebuf = new char[32];

  for (;;)
  {
    std::printf("\nEnter a word for lookup:");
    if (!std::gets(linebuf)) // EOF?
      return;

    // Search for the word
    int count = s_wordsTST.get(string(linebuf));
    if (count > 0)
      found = true;

    if (found)
    {
      std::printf("SUCCESS: '%s' was present %d times in the initial word list\n",
                  linebuf, count);
      ++s_totalFound;
    }
    else
      std::printf("'%s' was NOT found in the initial word list\n", linebuf);
  }

  delete linebuf;
}

int main()
{
  try
  {
    std::cout << "\n=== Provide list of words (terminate when the word 'end' is entered) :\n";
    readInputWords();

    std::cout << "\n=== Word List:\n";
    s_totalFound = s_wordsTST.printAll();

    lookupWords();

    std::cout << "\n=== Total words found: " << s_totalFound;
  }
  catch (std::exception &e)
  {
    std::cout << "exception: " << e.what();
  }

  return 0;
}
