/*
  ==============================================================================

    CircularIterator.h
    Created: 1 Jan 2023 9:15:58am
    Author:  Aleksandr

  ==============================================================================
*/

#pragma once

using namespace std;

using sSize = float;

class SlicedArray
{
public:
    SlicedArray(sSize& ptr, int startPoint, int size);
    sSize operator[](int it);
    SlicedArray();
    SlicedArray(const SlicedArray&&);
    SlicedArray& operator=(const SlicedArray&&) noexcept;
    ~SlicedArray();
private:
    sSize* ptr;
    int startPoint;
    int size;
};