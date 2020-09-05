/*
  ==============================================================================

    RenderTest.cpp
    Created: 3 Sep 2020 3:25:03pm
    Author:  Benedikt Sailer

  ==============================================================================
*/

#include "RenderTest.h"

RenderTest::RenderTest ()
{

}

void RenderTest::paint(Graphics &g)
{
    if (bgRenderer.updatedCaller.get())
    {
        bgRenderer.updatedCaller.set(false);
    }
    else
    {
        bgRenderer.addRenderCall
        ({
            [&](Graphics& bgG)
            {
                aPaintCallThatsTooComplex(bgG);
            },
            this
        }, getWidth(), getHeight());
    }
    
    Image imgToPaint = bgRenderer.getLatestImage();
    g.drawImage (imgToPaint,
                 getLocalBounds().toFloat(),
                 RectanglePlacement::fillDestination);
}

void RenderTest::resized ()
{
    bgRenderer.clearExceptLatest();
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
    
    
    // PARAMETERS
    float crossfade = JUCE_LIVE_CONSTANT(0.0f);
    
    Colour centerColourA(Colours::black); //
    Colour centerColourB(Colours::white);
    
    Colour outerColour(0xffffff); // white
    
    float invrt = jmin( jmax(JUCE_LIVE_CONSTANT(0.f), 1.0f), 0.f); // oder 1.0f
    DBG("invert: " << invrt );
    
//    Range range(0.f, 1.f);
    
    
    DBG("\n\n");
    for ( auto i = 0; i <= precision; ++i )
    {
        conicPath.clear();
        auto x = (float)i / precision; // goes from 1/1000 to 1
        
        
        auto radialGradientFunc = abs( std::sin( tau * x * JUCE_LIVE_CONSTANT(2.5) ) ); // 5 oder 10 mal von 0. nach 1.0
        DBG("radialGradientFunc: " << radialGradientFunc);
        
        
        outerColour =  outerColour.withRotatedHue( radialGradientFunc )
                            .withBrightness(abs ( invrt - radialGradientFunc)) // immer von dark to bright oder umgekehrt
                            .withSaturation(0.0f);
        
        conicPath.addCentredArc(centre.getX(), centre.getY(), conicRadius, conicRadius, 0, 0, tau * x, true );
        Line<float> conicLine(centre, conicPath.getCurrentPosition());
        
        Colour currentColour = centerColourA.withRotatedHue(tau * crossfade);
        
        
        g.setGradientFill(ColourGradient (currentColour, centre,
                                          outerColour, conicPath.getCurrentPosition(),
                                          true/*, // We don't want a radial pattern
                                          true*/) ); // We DO want a conical pattern)
        
//        g.setColour(conicColour);
        g.drawLine(conicLine);
    }
}
