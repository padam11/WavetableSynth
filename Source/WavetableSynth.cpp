/*
  ==============================================================================

    WavetableSynth.cpp
    Created: 25 Jul 2023 1:26:30am
    Author:  Rohan Buch

  ==============================================================================
*/

#include "WavetableSynth.h"

void WavetableSynth::prepareToPlay(double sampleRate)
{
    this->sampleRate = sampleRate;

    initializeOscillators();
}

void WavetableSynth::initializeOscillators()
{
   oscillators.clear(); // [1]
   constexpr auto OSCILLATOR_COUNT = 128;
   const auto sineWaveTable = generateSineWaveTable(); // [2]

   for (auto i = 0; i < OSCILLATOR_COUNT; ++i)   // [3]
   {
      oscillators.emplace_back(sineWaveTable, sampleRate); // [4]
   }
}

std::vector<float> WavetableSynth::generateSineWaveTable()
{
   constexpr auto WAVETABLE_LENGTH = 64;
   const auto PI = std::atanf(1.f) * 4;
   std::vector<float> sineWaveTable = std::vector<float>(WAVETABLE_LENGTH);

   for (auto i = 0; i < WAVETABLE_LENGTH; ++i)
   {
      sineWaveTable[i] = std::sinf(2 * PI * static_cast<float>(i) / WAVETABLE_LENGTH);
   }

    return sineWaveTable;
}

void WavetableSynth::processBlock(juce::AudioBuffer<float>& buffer,
                                  juce::MidiBuffer& midiMessages)
{
    auto currentSample = 0;

    for (const auto midiMetadata : midiMessages)
    {
        const auto message = midiMetadata.getMessage();
        const auto messagePosition = static_cast<int>(message.getTimeStamp());

        render(buffer, currentSample, messagePosition);
        currentSample = messagePosition;
        handleMidiEvent(message);
    }

    render(buffer, currentSample, buffer.getNumSamples());
}

void WavetableSynth::render(juce::AudioBuffer<float>& buffer,
                            int beginSample, int endSample)
{
    auto* firstChannel = buffer.getWritePointer(0);
    for (auto& oscillator : oscillators)
    {
        if (oscillator.isPlaying())
        {
            for (auto sample = beginSample; sample < endSample; ++sample)
            {
                firstChannel[sample] += oscillator.getSample();
            }
        }
    }

    for (int channel = 1; channel < buffer.getNumChannels(); ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        std::copy(firstChannel + beginSample,
            firstChannel + endSample,
            channelData + beginSample);
    }
}

void WavetableSynth::handleMidiEvent(const juce::MidiMessage& midiMessage)
{
    if (midiMessage.isNoteOn())
    {
        const auto oscillatorId = midiMessage.getNoteNumber();
        const auto frequency = midiNoteNumberToFrequency(oscillatorId);
        oscillators[oscillatorId].setFrequency(frequency);
    }
    else if (midiMessage.isNoteOff())
    {
        const auto oscillatorId = midiMessage.getNoteNumber();
        oscillators[oscillatorId].stop();
    }
    else if (midiMessage.isAllNotesOff())
    {
        for (auto& oscillator : oscillators)
        {
            oscillator.stop();
        }
    }
}

float WavetableSynth::midiNoteNumberToFrequency(const int midiNoteNumber)
{
    constexpr auto A4_FREQUENCY = 440.f;
    constexpr auto A4_NOTE_NUMBER = 69.f;
    constexpr auto NOTES_IN_AN_OCTAVE = 12.f;
    return A4_FREQUENCY *
            std::powf(2,
                (static_cast<float>(midiNoteNumber) - A4_NOTE_NUMBER) / NOTES_IN_AN_OCTAVE);
}

