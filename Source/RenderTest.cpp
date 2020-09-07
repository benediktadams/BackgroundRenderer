/*
  ==============================================================================

    RenderTest.cpp
    Created: 3 Sep 2020 3:25:03pm
    Author:  Benedikt Sailer

  ==============================================================================
*/

#include "RenderTest.h"

RenderTest::RenderTest ()
:
bgRenderer ({[&](Graphics& bgG) { aPaintCallThatsTooComplex(bgG); }, this})
{
}

void RenderTest::paint (Graphics &g)
{
    bgRenderer.draw (g, getLocalBounds().toFloat());
}

void RenderTest::aPaintCallThatsTooComplex (Graphics& g)
{
    auto radius = (float)jmin(getWidth(), getHeight());
    auto x = ( getWidth() - radius ) * 0.5f;
    auto y = ( getHeight() - radius ) * 0.5f;
    
    Rectangle<float> circleArea ( x, y, radius, radius );
    
    g.setColour(Colours::darkgrey);
    g.fillEllipse(circleArea);
    
    Point<float> centre ( getWidth()*0.5f, getHeight()*0.5f );
    auto conicRadius = radius * 0.5f;
    
    auto tau = MathConstants<float>::twoPi;

    auto precision = 1000;
    Path conicPath;
    
    float crossfade = (0.5f);
    
    Colour centerColourA(Colours::black); //
    Colour centerColourB(Colours::white);
    
    Colour outerColour(0xffffff); // white
    
    for ( auto i = 0; i <= precision; ++i )
    {
        conicPath.clear();
        auto x = (float)i / precision; // goes from 1/1000 to 1
        
        auto radialGradientFunc = abs( std::sin( tau * x * JUCE_LIVE_CONSTANT(2.5) ) );
        
        outerColour =  outerColour.withRotatedHue( radialGradientFunc )
                            .withBrightness(abs ( radialGradientFunc))
                            .withSaturation(0.0f);
        
        conicPath.addCentredArc(centre.getX(), centre.getY(), conicRadius, conicRadius, 0, 0, tau * x, true );
        Line<float> conicLine(centre, conicPath.getCurrentPosition());
        
        Colour currentColour = centerColourA.withRotatedHue(tau * crossfade);
        
        
        g.setGradientFill(ColourGradient (currentColour, centre,
                                          outerColour, conicPath.getCurrentPosition(),
                                          true) );
        g.drawLine(conicLine);
    }
}
