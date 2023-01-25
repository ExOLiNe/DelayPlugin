/*
  ==============================================================================

    ChannelMixingWritePointer.h
    Created: 3 Jan 2023 7:41:10pm
    Author:  Aleksandr

  ==============================================================================
*/

#pragma once

#include <vector>

using namespace std;

class ChannelMixingWritePointer
{
public:
    ChannelMixingWritePointer(const vector<const float*>& pointers, bool mixAll);
    float get(int channel, int index);
private:
    const vector<const float*>& pointers;
    bool mixAll;
};