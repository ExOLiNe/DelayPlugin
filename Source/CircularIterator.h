/*
  ==============================================================================

    CircularIterator.h
    Created: 1 Jan 2023 9:15:58am
    Author:  Aleksandr

  ==============================================================================
*/

#pragma once

#include <exception>

using namespace std;

using sSize = float;

class SampleIterator
{
public:
    SampleIterator(sSize* endPointer, int endSize, sSize* beginPointer, int beginSize, int pos = 0);
    sSize operator*() const;
    SampleIterator* operator->();
    SampleIterator* operator++();
    SampleIterator begin();
    SampleIterator end();
private:
    bool isInTheEnd() const;
    int pos;
    sSize* endPointer;
    int endSize;
    sSize* beginPointer;
    int beginSize;
};