// Selim Sheta, 2021. You may reuse this code.
// Haas Effect Processor - Uses the Haas effect to spread a mono signal in the stereo field.

#include <vector>

/* Use the ProcessSample function to process a mono signal. The output will be an std::vector in the form [LeftChSample, RightChSample]. 
Depth is the delay time in ms, and Separation controls the Wet/Dry ratio in the Left and Right channel:
Separation = 1 :  the Left channel is 100% wet and the Right channel is 0% wet. 
Separation = 0 : Both channels are 50% wet.
Separation = -1 : the Left channel is 0% wet and the Right channel is 100% wet.  
For best results keep Separation between 0.5 - 1, & Depth between 5 - 40. */
class HaasEffect
{
public:
    HaasEffect();
    HaasEffect(int InSampleRate, float InDepth, float InSeparation);
    virtual ~HaasEffect() {}
    std::vector<float> ProcessSample(float InSample);
    void SetDepth(float InDepth);
    void SetSeparation(float InSeparation);
protected:
    float Separation = 0.5f;
    float Depth = 0.01f;
    int SampleRate = 44100;
    int ReadPointer = 0;
    int WritePointer = 0;
    std::vector<float> DelayBuffer;
    float ApplyDelay(float InSample);
};
