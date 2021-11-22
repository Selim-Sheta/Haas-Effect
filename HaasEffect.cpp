// Selim Sheta, 2021. You may reuse this code.

#include "HaasEffect.h"

const int MAX_DELAY_TIME_INT = 1;
const float MAX_DELAY_TIME = (float)MAX_DELAY_TIME_INT;

// Clamping function
float Clamp(float InParam, float InMin, float InMax) {
    return std::min(std::max(InParam, InMin), InMax);
}

/*### Haas Effect  ###*/

// Default constructor
HaasEffect::HaasEffect() {
    DelayBuffer.resize(MAX_DELAY_TIME_INT * SampleRate, 0.0f);
    ReadPointer = (int)((MAX_DELAY_TIME - Depth) * (float)SampleRate) - 1;
    WritePointer = 0;
}

// Constructor
HaasEffect::HaasEffect(int InSampleRate, float InDepth, float InSeparation) {
    SampleRate = InSampleRate;
    Depth = Clamp(InDepth/1000.0f, 0.0f, MAX_DELAY_TIME - 0.1f);
    Separation = Clamp(InSeparation, -1.0f, 1.0f);
    DelayBuffer.resize(MAX_DELAY_TIME_INT * SampleRate, 0.0f);
    ReadPointer = (int)((MAX_DELAY_TIME - Depth) * (float)SampleRate) - 1;
    WritePointer = 0;
}

// Process one sample
std::vector<float> HaasEffect::ProcessSample(float InSample) {
    float DelayedSample = ApplyDelay(InSample);
    float LeftSample = DelayedSample * ((Separation + 1.0f) / 2.0f) + InSample * ((1.0f - Separation) / 2.0f);
    float RightSample = DelayedSample * ((1.0f - Separation) / 2.0f) + InSample * ((Separation + 1.0f) / 2.0f);
    std::vector<float> Output{ LeftSample, RightSample };
    return Output;
}

// Set Depth parameter (ms)
void HaasEffect::SetDepth(float InDepth) {
    Depth = Clamp(InDepth / 1000.0f, 0.0f, MAX_DELAY_TIME-0.1f);
    ReadPointer = WritePointer - (int)(Depth * (float)SampleRate);
    while (ReadPointer < 0) {
        ReadPointer += MAX_DELAY_TIME_INT * SampleRate;
    }
    while (ReadPointer >= MAX_DELAY_TIME_INT * SampleRate) {
        ReadPointer--;
    }
}

// Set Separation parameter
void HaasEffect::SetSeparation(float InSeparation) {
    Separation = Clamp(InSeparation, -1.0f, 1.0f);
}

// Function to apply delay
float HaasEffect::ApplyDelay(float InSample) {
    // Get delayed output
    float Output = DelayBuffer[ReadPointer];
    // Updated buffer
    DelayBuffer[WritePointer] = InSample;
    // Increment pointers
    ReadPointer++;
    WritePointer++;
    // Wrap pointers
    if (ReadPointer >= MAX_DELAY_TIME_INT * SampleRate) {
        ReadPointer = 0;
    }
    if (WritePointer >= MAX_DELAY_TIME_INT * SampleRate) {
        WritePointer = 0;
    }
    return Output;
}