#include <Bela.h>
#include <libraries/SampleLoader/SampleLoader.h>
#include <cmath>
#include <vector>

// Example dataset: pedestrian counts per location (0–10 scale)
std::vector<int> pedestrianDensity = {1, 3, 6, 2, 8, 4, 9, 5};

unsigned int currentIndex = 0;

// Drum sample
SampleLoader drum;
int drumPosition = -1; // -1 means not currently playing

// Beat timing
int samplesUntilNextBeat = 0;

bool setup(BelaContext *context, void *userData)
{
    // Load your drum sample into project folder (e.g. "drum.wav")
    if(drum.load("drum.wav") == false) {
        rt_printf("Error: couldn't load drum sample\n");
        return false;
    }
    // Ensure sample is mono
    drum.setShouldLoop(false);
    return true;
}

void render(BelaContext *context, void *userData)
{
    for(unsigned int n = 0; n < context->audioFrames; n++) {

        if(samplesUntilNextBeat <= 0) {
            // Trigger drum
            drumPosition = 0;

            // Get current density value
            int density = pedestrianDensity[currentIndex];

            // Map density to tempo:
            // density 0 → ~1 beat per second
            // density 10 → ~10 beats per second
            float beatsPerSecond = 1.0 + density * 0.9;
            float secondsPerBeat = 1.0f / beatsPerSecond;
            samplesUntilNextBeat = (int)(secondsPerBeat * context->audioSampleRate);

            // Step dataset index
            currentIndex = (currentIndex + 1) % pedestrianDensity.size();
        }

        float out = 0.0;

        // If drum is playing, fetch sample
        if(drumPosition >= 0 && drumPosition < drum.size) {
            out = drum[d]rumPosition];
            drumPosition++;
        } else {
            drumPosition = -1; // finished
        }

        samplesUntilNextBeat--;

        // Output to all channels
        for(unsigned int ch = 0; ch < context->audioOutChannels; ch++) {
            audioWrite(context, n, ch, out * 0.8f);
        }
    }
}

void cleanup(BelaContext *context, void *userData) {}
