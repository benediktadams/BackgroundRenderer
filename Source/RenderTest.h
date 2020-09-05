/*
  ==============================================================================

    RenderTest.h
    Created: 3 Sep 2020 3:25:03pm
    Author:  Benedikt Sailer

  ==============================================================================
*/

#pragma once

#include "BackgroundRenderer.h"
#include <JuceHeader.h>

class RenderTest : public Component
{
public:
    RenderTest ();

private:
    BackgroundRenderer bgRenderer;
    
    void paint (Graphics& g) override;
    void resized () override;
    
    void aPaintCallThatsTooComplex (Graphics& g);
};
