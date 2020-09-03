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
        std::function<void()> /*onFinishedRendering*/
        > BgRenderingCall;

class BackgroundRenderer : public Thread
{
public:
    BackgroundRenderer ();
    ~BackgroundRenderer ();
    
    void addRenderCall (BgRenderingCall call, int width, int height);
    Image& getLatestImage ();
    void clearExceptLatest ();
    
    Atomic<bool> updatedCaller { false };
    
private:
    CriticalSection cs;
    std::queue<BgRenderingCall> renderCalls;
    std::queue<std::pair<int, int>> renderSizes;
    
    CriticalSection imageCs;
    Image latestImage;
    
    void run () override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BackgroundRenderer)
};
