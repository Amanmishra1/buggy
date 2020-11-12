#ifndef WORD_H
#define WORD_H

class Word
{
public:
    Word(const char* data_) :
        data(::strdup(data_))
    {}

    Word() :
        data((char*)"")
    {}
	
	char * get_data() const
	{
		return data;
	}
	int get_count() const
	{
		return count;
	}
	void increment_count()
	{
		++count;
	}
	
private:
    char* data{nullptr};
    int count{0};
	
};

#endif