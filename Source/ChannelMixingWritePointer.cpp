/*
  ==============================================================================

    ChannelMixingWritePointer.cpp
    Created: 3 Jan 2023 7:41:10pm
    Author:  Aleksandr

  ==============================================================================
*/

#include "ChannelMixingWritePointer.h"

ChannelMixingWritePointer::ChannelMixingWritePointer(const vector<const float*>& pointers, bool mixAll) : pointers(pointers), mixAll(mixAll)
{
}

float ChannelMixingWritePointer::get(int channel, int index)
{
    float value = 0;
    if (mixAll)
    {
        for (int channel = 0; channel < pointers.size(); ++channel)
        {
            value += pointers[channel][index];
        }
        value /= pointers.size();
    }
    else
    {
        value = pointers[channel][index];
    }
    return value;
}