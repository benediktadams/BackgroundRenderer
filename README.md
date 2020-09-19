# JUCE Background Renderer
## Helper tool that processes JUCE paint calls on a worker thread and displays the rendering result as image with a delay

If you have a JUCE paint() call that's too complex to handle on the message thread, this tool will allow you to handle the rendering on a worker thread and then display the result when it's done. 

For a usage example have a look at RenderTest.h/.cpp
```
void RenderTest::aPaintCallThatsTooComplex (Graphics& g) 
```
contains a paint routine, that, as the name suggests, is too complex and takes too long to perform on the message thread (Try cranking up the number in the JUCE_LIVE_CONSTANT editor to make it even more complex)

To handle the rendering of that call on the background helper add an instance of ```BackgroundRenderer``` to your class containing the paint call and set it up as demonstrated in the ```RenderTest``` example. This will perform the paint call on a background thread and display the result when it's finished. 