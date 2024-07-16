/*
  ==============================================================================

    Track.cpp
    Created: 3 Mar 2023 2:55:30pm
    Author:  Loh Qi Yang

  ==============================================================================
*/

#include "Playlist.h"
#include <filesystem>

Playlist::Playlist(juce::File _file) : file(_file),
                                 title(_file.getFileNameWithoutExtension()),
                                 URL(juce::URL{ _file })
{
    DBG("Created new track with title: " << title);
}

bool Playlist::operator==(const juce::String& other) const
{
    return title == other;
}
