/*
  ==============================================================================

    BackgroundRenderer.cpp
    Created: 3 Sep 2020 3:24:46pm
    Author:  Benedikt Sailer

  ==============================================================================
*/

#include "BackgroundRenderer.h"

BackgroundRenderer::BackgroundRenderer ()
:
Thread ("BgRenderingHelper" + Uuid().toString())
{
    startThread ();
}

BackgroundRenderer::~BackgroundRenderer ()
{
    if (isThreadRunning ())
    {
        signalThreadShouldExit ();
        stopThread (2000);
    }
}

void BackgroundRenderer::addRenderCall (BgRenderingCall call, int width, int height)
{
    const ScopedLock sl (cs);
    renderCalls.push (call);
    renderSizes.push ({width, height});
}

void BackgroundRenderer::run ()
{
    while (!threadShouldExit())
    {
        Thread::wait (1);
        
        BgRenderingCall currentCall { nullptr, nullptr };
        std::pair<int, int> size;
        {
            const ScopedLock sl (cs);
            if (!renderCalls.empty())
            {
                currentCall = renderCalls.front();
                renderCalls.pop();
                
                size = renderSizes.front();
                renderSizes.pop ();
            }
        }
        
        if (currentCall.first != nullptr && currentCall.second != nullptr)
        {
            Image img { Image::ARGB, size.first, size.second, true };
            Graphics g { img };
            currentCall.first (g);
            
            {
                const ScopedLock sl (imageCs);
                latestImage = img;
            }
 
            MessageManager::callAsync ([&, caller = currentCall.second]()
            {
                updatedCaller.set (true);
                if (caller) caller->repaint();
            });
            
            clearExceptLatest ();
        }
    }
}

Image& BackgroundRenderer::getLatestImage ()
{
    const ScopedLock sl (imageCs);
    return latestImage;
}

void BackgroundRenderer::clearExceptLatest ()
{
    const ScopedLock sl (cs);
    while (renderCalls.size() > 1) renderCalls.pop ();
    while (renderSizes.size() > 1) renderSizes.pop ();
}
