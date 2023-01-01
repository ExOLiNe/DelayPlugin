/*
  ==============================================================================

    CircularIterator.cpp
    Created: 1 Jan 2023 9:15:58am
    Author:  Aleksandr

  ==============================================================================
*/

#include "CircularIterator.h"


SampleIterator::SampleIterator(sSize* endPointer, int endSize, sSize* beginPointer, int beginSize, int pos) :
        endPointer(endPointer), endSize(endSize), beginPointer(beginPointer), beginSize(beginSize), pos(pos)
{
}
sSize SampleIterator::operator*() const
{
    if (isInTheEnd())
    {
        return *(endPointer + pos);
    }
    else
    {
        if (pos - endSize > beginSize)
        {
            throw exception("out of bound");
        }

        return *(beginPointer + pos - endSize);
    }
}
SampleIterator* SampleIterator::operator->()
{
    return this;
}
SampleIterator* SampleIterator::operator++()
{
    pos++;
    return this;
}
SampleIterator SampleIterator::begin()
{
    return SampleIterator(this->endPointer, this->endSize, this->beginPointer, this->beginSize, 0);
}
SampleIterator SampleIterator::end()
{
    return SampleIterator(this->endPointer, this->endSize, this->beginPointer, this->beginSize, this->endSize + this->beginSize + 1);
}
bool SampleIterator::isInTheEnd() const
{
    return pos <= endSize - 1;
}