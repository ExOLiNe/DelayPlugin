/*
  ==============================================================================

    CircularIterator.cpp
    Created: 1 Jan 2023 9:15:58am
    Author:  Aleksandr

  ==============================================================================
*/

#include "SlicedArray.h"
#include "JuceHeader.h"

using namespace juce;

SlicedArray::SlicedArray(sSize& ptr, int startPoint, int size) :
        ptr(&ptr), startPoint(startPoint), size(size)
{
}

sSize SlicedArray::operator[](int it)
{
    return *(ptr + (startPoint + it) % size);
}

SlicedArray::SlicedArray() : ptr(nullptr), startPoint(0), size(0)
{
}

SlicedArray::SlicedArray(const SlicedArray&& other) : ptr(other.ptr), startPoint(other.startPoint), size(other.size)
{
}

SlicedArray& SlicedArray::operator=(const SlicedArray&& other) noexcept
{
    if (this != &other)
    {
        ptr = other.ptr;
        startPoint = other.startPoint;
        size = other.size;
    }
    return *this;
}

SlicedArray::~SlicedArray()
{

}