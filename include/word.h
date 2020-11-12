#ifndef WORD_H
#define WORD_H

struct Word
{
    char *data_;

    Word(char *data) : data_(::strdup(data)){};
    Word() : data_((char *)""){};
    ~Word()
    {
        if (data_)
            delete data_;
    };
    Word(Word &input)
    {
        data_ = new char[strlen(input.data_)];
        strcpy(data_, input.data_);
    };
};

#endif