/*
  ==============================================================================

    CircularIterator.cpp
    Created: 1 Jan 2023 12:34:40pm
    Author:  Aleksandr

  ==============================================================================
*/

#include "CircularIterator.h"
#include <cstdlib>

CircularIterator::CircularIterator(int size, int pos) : size(size)
{
    setPos(pos);
}

CircularIterator::~CircularIterator()
{

}

int CircularIterator::getPos()
{
    return this->pos;
}

void CircularIterator::operator+=(int steps)
{
    setPos(pos + steps);
}

int CircularIterator::operator-(CircularIterator& another)
{
    return this->pos - another.getValue();
}

void CircularIterator::setPos(int newPos)
{
    pos = (size + newPos % size) % size;
}

void CircularIterator::operator=(int numb)
{
    setPos(numb);
}

int CircularIterator::getValue()
{
    return pos;
}