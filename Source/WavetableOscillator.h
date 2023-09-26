/*
  ==============================================================================

    WavetableOscillator.h
    Created: 25 Jul 2023 1:34:18am
    Author:  Rohan Buch

  ==============================================================================
*/

#pragma once

#include <vector>

class WavetableOscillator
{
public:
   WavetableOscillator(std::vector<float> waveTable, double sampleRate); // [1]
   WavetableOscillator(const WavetableOscillator&) = delete; // [2]
   WavetableOscillator& operator=(const WavetableOscillator&) = delete; // [2]
   WavetableOscillator(WavetableOscillator&&) = default; // [3]
   WavetableOscillator& operator=(WavetableOscillator&&) = default; //[3]

   float getSample();   // [4]
   void setFrequency(float frequency);   // [5]
   void stop();   // [6]
   bool isPlaying() const;   // [7]

private:
   float interpolateLinearly() const;   // [8]

   float index = 0.f;   // [9]
   float indexIncrement = 0.f;   // [10]
   std::vector<float> waveTable;    //  [11]
   double sampleRate;   //  [12]
};
