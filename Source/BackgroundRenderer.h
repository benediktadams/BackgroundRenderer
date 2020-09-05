/*
  ==============================================================================

    BackgroundRenderer.h
    Created: 3 Sep 2020 3:24:46pm
    Author:  Benedikt Sailer

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

typedef std::pair
        <
        std::function<void(Graphics& g)> /*call*/,
        Component* /*caller*/
        > BgRenderingCall;

class BackgroundRenderer : public Thread
{
public:
    BackgroundRenderer (BgRenderingCall call);
    ~BackgroundRenderer ();
    
    void draw (Graphics& g,
               const Rectangle<float>& area,
               RectanglePlacement rectPlacement = RectanglePlacement::fillDestination);
    
    Image& getLatestImage ();
    
    Atomic<bool> updatedCaller { false };
    
private:
    CriticalSection cs;
    BgRenderingCall renderCall;
    std::pair<Atomic<int>, Atomic<int>> renderSize;
    
    CriticalSection imageCs;
    Image latestImage;
    
    void run () override;
    Atomic<int> shouldRender { false };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BackgroundRenderer)
};
