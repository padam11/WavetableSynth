/*
  ==============================================================================

    WavetableSynth.h
    Created: 25 Jul 2023 1:26:30am
    Author:  Rohan Buch

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "WavetableOscillator.h"

class WavetableSynth
{
public:
    void prepareToPlay(double sampleRate); //sets sample rate
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
private:
    static std::vector<float> generateSineWaveTable(); //generates 64 samples of sine
    static float midiNoteNumberToFrequency(int midiNoteNNumber); //midi to hz
    void initializeOscillators(); //inits 128 oscillators
    void handleMidiEvent(const juce::MidiMessage& midiMessage); //midi to params
    void render(juce::AudioBuffer<float>& buffer, int beginSample, int endSample);  // [6]
    
    double sampleRate;
    int samplesPerBlock;
    std::vector<WavetableOscillator> oscillators; //notes are assigned
    
};
