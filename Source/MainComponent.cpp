#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    glContext.setComponentPaintingEnabled (true);
    glContext.attachTo (*this);
    
    addAndMakeVisible (test);
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void MainComponent::resized()
{
    test.setBounds (getLocalBounds());
}
