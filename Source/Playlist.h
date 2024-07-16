/*
  ==============================================================================

    Track.h
    Created: 3 Mar 2023 2:55:30pm
    Author:  Loh Qi Yang

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Playlist
{
public:
    Playlist(juce::File _file);
    juce::File file;
    juce::URL URL;
    juce::String title;
    juce::String length;
    
    bool operator==(const juce::String& other) const;
};
