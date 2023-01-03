/*
  ==============================================================================

    CircularIterator.h
    Created: 1 Jan 2023 12:34:40pm
    Author:  Aleksandr

  ==============================================================================
*/

#pragma once

class CircularIterator
{
public:
    CircularIterator(float* ptr, int size, int pos);
    ~CircularIterator();
    void operator+=(int steps);
    void operator=(int numb);
    int operator-(CircularIterator& another);
    void setPos(int newPos);
    int getPos();
    int getValue();
private:
    CircularIterator();
    float* ptr;
    int size;
    int pos;
};