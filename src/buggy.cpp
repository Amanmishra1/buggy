#include "buggy.h"
#include "word.h"

bool wordSort(Word* word1, Word* word2)
{
   return std::strcmp(word1->get_data(), word2->get_data()) < 0;
}

static std::queue<Word *> inputQueue;
static std::vector<Word *> s_wordsArray;
static int s_totalFound;
std::mutex mutexLock;

/**
 * processWord	Process the user input while removing duplicates and insert them
 * 				in the 'word list' (s_wordsArray)
 *
 * @param[in] wordInput Word entered by the user.
 * @return void
 */
static void processWord(Word *wordInput)
{
  bool found = false;

  // Do not insert duplicate words
  for (auto p : s_wordsArray)
  {

    if (!std::strcmp(p->get_data(), wordInput->get_data()))
    {
      p->increment_count();
      found = true;
      break;
    }
  }

  if (!found)
  {
    wordInput->increment_count();
    s_wordsArray.push_back(wordInput);
  }
  else
  {
    //De allocate memory for rejected words.
    free(wordInput->get_data());
    free(wordInput);
  }
  found = false; //Reset the flag for next word
}

static void workerThread()
{
  bool endEncountered{false};
  bool isQueueEmpty{true};
  Word *word{nullptr};

  while (!endEncountered)
  {
    {
      std::lock_guard<std::mutex> lck{mutexLock};
      isQueueEmpty = inputQueue.empty();
    }

    if (!isQueueEmpty)
    {
      {
        std::lock_guard<std::mutex> lck{mutexLock};
        word = inputQueue.front();
        inputQueue.pop();
      }

      endEncountered = std::strcmp(word->get_data(), "end") == 0;

      if (!endEncountered)
      {
        processWord(word);
      }
    }
  }
}

static bool processInput(std::string inputWord)
{
  bool isEnd = false;
  std::size_t pos = inputWord.find(" "); //handle cases in multiple words

  if (!pos) 
  {
    pos = inputWord.length();
  }
  std::string currentWord = inputWord.substr(0, pos);

  if (currentWord.length() != 0)
  {
    if ((currentWord.compare("end")) == 0)
    {
      isEnd = true;
    }
    //Lock the queue
    {
      std::lock_guard<std::mutex> lck{mutexLock};
      Word *word = new Word((const char *)currentWord.c_str());
      inputQueue.push(word);
    }
  }

  return isEnd;
}

static void readInputWords()
{
  bool endEncountered = false;

  std::thread *worker = new std::thread(workerThread);

  std::string userInput;
  std::cout << "\nEnter list of words, 'end' to terminate entering : \n";

  while (!endEncountered)
  {
    if (!std::getline(std::cin, userInput))
    {
      return;
    }
    else
    {
      endEncountered = processInput(userInput);
    }
  }

  worker->join(); // Wait for the worker to terminate
}

static void searchInput(std::string searchWord)
{
  bool found = false; //Initialize flag as 'Not found'
  Word *w = new Word(searchWord.c_str());

  // Search for the word
  unsigned i;

  for (i = 0; i < s_wordsArray.size(); ++i)
  {
    if (std::strcmp(s_wordsArray[i]->get_data(), w->get_data()) == 0)
    {
      found = true;
      break;
    }
  }

  if (found)
  {
    std::printf("SUCCESS: '%s' was present %d times in the initial word list\n",
                s_wordsArray[i]->get_data(), s_wordsArray[i]->get_count());
    ++s_totalFound;
  }
  else
  {
    std::printf("'%s' was NOT found in the initial word list\n", w->get_data());
  }
  found = false; //Reset the flag
  free(w);       //Clear the allocated memory
}

static void lookupWords()
{
  std::string userSearchInput;

  for (;;)
  {
    std::printf("\nEnter a word for lookup:");
    if (!std::getline(std::cin, userSearchInput))
    {
      return;
    }

    // Initialize the word to search for
    std::size_t pos = userSearchInput.find(" "); // position of "live" in str
    if (!pos)
    {
      pos = userSearchInput.length();
    }

    std::string word = userSearchInput.substr(0, pos);
    if (word.length() > 0)
    {
      searchInput(word);
    }
  }
}

int main()
{
  try
  {
    readInputWords();

    // Sort the words alphabetically

    std::sort(s_wordsArray.begin(), s_wordsArray.end(), wordSort);

    // Print the word list
    std::printf("\n=== Word list:\n");

    for (auto p : s_wordsArray)
    {
      std::printf("%s %d\n", p->get_data(), p->get_count());
    }

    lookupWords();

    printf("\n=== Total words found: %d\n", s_totalFound);
  }
  catch (std::exception &e)
  {
    std::printf("error %s\n", e.what());
  }

  return 0;
}