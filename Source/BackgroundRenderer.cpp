/*
  ==============================================================================

    BackgroundRenderer.cpp
    Created: 3 Sep 2020 3:24:46pm
    Author:  Benedikt Sailer

  ==============================================================================
*/

#include "BackgroundRenderer.h"

BackgroundRenderer::BackgroundRenderer (BgRenderingCall call)
:
Thread ("BgRenderingHelper" + Uuid().toString()),
renderCall (call)
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


void BackgroundRenderer::run ()
{
    while (!threadShouldExit())
    {
        Thread::wait (1);
        
        if (shouldRender.get())
        {
            shouldRender.set (false);
            Image img { Image::ARGB,
                        renderSize.first.get(),
                        renderSize.second.get(),
                        true };
            Graphics g { img };
            
            renderCall.first (g);
            
            {
                const ScopedLock sl (imageCs);
                latestImage = img;
            }

            
            MessageManager::callAsync ([&]()
            {
                 updatedCaller.set (true);
                if (auto comp = renderCall.second)
                    comp->repaint();
                
               
                
            });
            
        }
    }
}

void BackgroundRenderer::draw (Graphics& g, const Rectangle<float>& area, RectanglePlacement rectPlacement)
{
    if (updatedCaller.get ())
    {
        updatedCaller.set (false);
    }
    else if (!shouldRender.get()
             || renderSize.first.get() != area.getWidth()
             || renderSize.second.get() != area.getWidth())
    {
        renderSize.first.set (area.getWidth());
        renderSize.second.set (area.getHeight());
        shouldRender.set (true);
    }
       
    Image imgToPaint { getLatestImage() };
    g.drawImage (imgToPaint,
                 area,
                 rectPlacement);
}

Image& BackgroundRenderer::getLatestImage ()
{
    const ScopedLock sl (imageCs);
    return latestImage;
}


