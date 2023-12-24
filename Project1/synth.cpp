#include <math.h>
#include "synth.h"
#include <cmath>
#include <vector>
#include <thread>
#include <portaudio.h>
#include <mutex>
#include <cstdlib>
#include <ctime>

extern std::mutex myMutex;
using namespace std;

#define NUM_SECONDS (20)
#define SEMI_TONE pow(2,1/12.)
#define CENT pow(3*SEMI_TONE,1/300.)
#define FRAMES_PER_BUFFER (1)

#ifndef M_PI
#define M_PI (3.14159265)
#endif

double randAmp = 0;
double sineFunction(double time, float pulseWidth) {
    return sin((time * M_PI * 2));
}
double sawFunction(double time, float pulseWidth) {
    return 1 - 2 * time;
}
double squareFunction(double time,float pulseWidth) {
    return time < pulseWidth ? 1 : -1;
}
double triFunction(double time, float pulseWidth) {
    return time < 0.5 ? -1 + 2 * time : 1 - 2 * time;
}

double noiseFunction(double time, float pulseWidth) {
    return ( - 1 + (rand() / ((double)RAND_MAX)/2));
}

double modEnvelopeFunction(Note* note, int time, ModEnvelope modEnvelope) {
    double env = 0;
    if (note->pitchEnvelopeStage == ATTACK) {
        env = ((time - note->startTime)) / (double)modEnvelope.attack;
        if (env >= 1)
            note->pitchEnvelopeStage = DECAY;
    }
    else if (note->pitchEnvelopeStage == DECAY) {
        env = 1 - (time - (note->startTime+modEnvelope.attack)) / (double)modEnvelope.decay;
        if (env <= 0)
            note->pitchEnvelopeStage = SUSTAIN;
    }
    return env;

}

double volumeEnvelopeFunction(Note* note, int time, VolumeEnvelope* volEnvelope) {
    double envelopeValue = volEnvelope->sustain;
    if (note->volEnvelopeStage == RELEASE) {
        double noteEnv = (time - note->releaseTime) / (double)volEnvelope->release;
        double peak = volEnvelope->sustain;
        if (note->volEnvelopeStage == ATTACK)
            peak = (note->releaseTime - note->startTime) / (double)volEnvelope->attack;

        else if (note->volEnvelopeStage == DECAY)
            peak = 1 - ((1 - volEnvelope->sustain) * (note->releaseTime - note->startTime - volEnvelope->attack) / (double)volEnvelope->decay);

        envelopeValue = peak * (1 - noteEnv);
        if (noteEnv >= 1)
            note->hasEnded = true;
    }
    else if (note->volEnvelopeStage == ATTACK)
    {
        double noteEnv = (time - note->startTime) / (double)volEnvelope->attack;
        envelopeValue = noteEnv;
        if (noteEnv >= 1) {
            note->volEnvelopeStage = DECAY;
        }
    }
    else if (note->volEnvelopeStage == DECAY) {
        double noteEnv = (time - note->startTime - volEnvelope->attack) / (double)volEnvelope->decay;
        envelopeValue = 1 - (1 - volEnvelope->sustain) * noteEnv;
        if (noteEnv >= 1) {
            note->volEnvelopeStage = SUSTAIN;
        }
    }
    return envelopeValue;
}


double (*waveFunctions[])(double,float) {
    sineFunction,
        sawFunction,
        squareFunction,
        triFunction,
        noiseFunction
};

double lfoFunction(int sampleRate,int sample, Lfo* lfo) {
    double time = sample / (double)sampleRate;
    double interval = time * lfo->frequency;
    if (lfo->waveType == NOISE) {
        if (!(sample % (int)(sampleRate / lfo->frequency)))
            randAmp = waveFunctions[lfo->waveType](interval, 0.5);
        return lfo->amount * randAmp;
    }
    if (interval > 1) {
        interval -= (int)interval;
    }
    return lfo->amount * waveFunctions[lfo->waveType](interval,0.5);
}


float filterFunction(double pitch, Filter filter,  double lfoValue)
{

    double filterRatio = pitch / filter.cutoff;
    filterRatio /= (1 + lfoValue);
    double filterValue = 1;

    if (filter.type == LOWPASS) {
        if (filterRatio > 1) {
            if (filterRatio >= 2)
                filterValue = 0;
            else
                filterValue = 1 - filterRatio / 2;
        }
    }
    else if (filter.type == HIGHPASS) {
        if (filterRatio < 1) {
            if (filterRatio <= 0.5)
                filterValue = 0;
            else
                filterValue = 1 - filterRatio / 0.5;
        }
    }
    return filterValue;
}

double detuneFunction( double pitch
    , Detune detune) {
    double detunedPitch =pitch ;
    for (int i = 0; i < detune.voices; i++) {
        detunedPitch *= SEMI_TONE* detune.amount;
    }
    return detunedPitch;
}
static int
patestCallback(const void* inputBuffer, void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData)
{
    Synthesizer* synthesizerObj = (Synthesizer*)userData;
    float* out = (float*)outputBuffer;
    unsigned long i;

    (void)timeInfo;
    (void)statusFlags;
    (void)inputBuffer;

    for (i = 0; i < framesPerBuffer; i++)
    {   

        double time = ((synthesizerObj->interval % synthesizerObj->sampleRate) / (double)synthesizerObj->sampleRate);
        
        double lfoValue = lfoFunction(synthesizerObj->sampleRate,synthesizerObj->interval, &synthesizerObj->lfo);
        myMutex.lock();
        double osc1Sample = 0;
        double osc2Sample = 0;

        for (auto note : synthesizerObj->playedNotes) {

            double notePitch = pow(2, note->octave) * pow(SEMI_TONE, note->note) * pow(2, 4);
            if (synthesizerObj->modEnvelope.destination == PITCH) {
                double pitchEnv = modEnvelopeFunction(note, synthesizerObj->interval, synthesizerObj->modEnvelope);
                notePitch *= (1 + pitchEnv * synthesizerObj->modEnvelope.amount);
            }

            if (synthesizerObj->lfo.destination == PITCH) 
                notePitch += (100 * lfoValue);

            double noteVol = volumeEnvelopeFunction(note, synthesizerObj->interval, &synthesizerObj->volumeEnvelope);

            float osc1PulseWidth = synthesizerObj->oscillator1.pulseWidth;
            float osc2PulseWidth = synthesizerObj->oscillator2.pulseWidth;

            if (synthesizerObj->lfo.destination == PULSE_WIDTH) {
                osc1PulseWidth *= (1 + lfoValue);
                osc2PulseWidth *= (1 + lfoValue);
            }

            double osc1Pitch = notePitch * pow(SEMI_TONE, synthesizerObj->oscillator1.semitone) * pow(SEMI_TONE, synthesizerObj->oscillator1.detune/100);
            double osc2Pitch = notePitch * pow(SEMI_TONE, synthesizerObj->oscillator2.semitone) * pow(SEMI_TONE, synthesizerObj->oscillator2.detune/100);
            
            double osc1Interval = time * osc1Pitch;
            
            if (osc1Interval > 1)
                osc1Interval -= int(osc1Interval);
            
            double osc2Interval = time * osc2Pitch;
            
            if (osc2Interval > 1)
                osc2Interval -= int(osc2Interval);
            
            osc1Sample += noteVol * waveFunctions[synthesizerObj->oscillator1.waveType](osc1Interval, osc1PulseWidth) * filterFunction(osc1Pitch, synthesizerObj->filter, 0);
            osc2Sample += noteVol * waveFunctions[synthesizerObj->oscillator2.waveType](osc2Interval, osc2PulseWidth) * filterFunction(osc2Pitch, synthesizerObj->filter, 0);

        if (synthesizerObj->detune.amount) {
            double detunedOsc1Pitch = osc1Pitch;
            double detunedOsc2Pitch = osc2Pitch;
            double detunedAmount = pow(SEMI_TONE, synthesizerObj->detune.amount / 100);
            for (int i = 0; i < synthesizerObj->detune.voices-1; i++) {
                
                detunedOsc1Pitch *= detunedAmount;
                detunedOsc2Pitch *= detunedAmount;

                double detunedOsc1Interval = time * detunedOsc1Pitch;
                if (detunedOsc1Interval > 1)
                    detunedOsc1Interval -= int(detunedOsc1Interval);

                double detunedOsc2Interval = time * detunedOsc2Pitch;
                if (detunedOsc2Interval > 1)
                    detunedOsc2Interval -= int(detunedOsc2Interval);

                osc1Sample += noteVol * waveFunctions[synthesizerObj->oscillator1.waveType](detunedOsc1Interval, osc1PulseWidth) * filterFunction(detunedOsc1Pitch, synthesizerObj->filter, 0);
                osc2Sample += noteVol * waveFunctions[synthesizerObj->oscillator2.waveType](detunedOsc2Interval, osc2PulseWidth) * filterFunction(detunedOsc2Pitch, synthesizerObj->filter, 0);
            }
        }
            
        }

        float sample = ((1 - synthesizerObj->mix) * osc1Sample + synthesizerObj->mix * osc2Sample);

        if (synthesizerObj->lfo.destination == VOLUME)
            sample *= (1 + lfoValue);

        sample *= (1 + synthesizerObj->volume);

        *out++ = sample;
        *out++ = sample;


        while (true) {
            auto it = std::find_if(synthesizerObj->playedNotes.begin(), synthesizerObj->playedNotes.end(), [](const Note* playedNote) {
                return (playedNote->hasEnded);
                });
            if (it != synthesizerObj->playedNotes.end())
                synthesizerObj->playedNotes.erase(it);
            else break;
        }
        myMutex.unlock();
        synthesizerObj->interval++;

    }
    return paContinue;
}

static void StreamFinished(void *userData)
{
    Synthesizer *data = (Synthesizer *)userData;
}   

PaError err;

int closeStream(PaStream* stream) {
    err = Pa_StopStream(stream);
    if (err != paNoError)
        return err;
    err = Pa_CloseStream(stream);
    return err;
}

int Synthesizer::startStream(PaStreamParameters* outputParameters)
{
    if (outputParameters->device == paNoDevice)
    {
        return err;
    }
    
    interval = 0;
    sampleRate = devices[currentDeviceIndex]->defaultSampleRate;
    outputParameters->channelCount = 2;
    outputParameters->sampleFormat = paFloat32;

    outputParameters->suggestedLatency = devices[currentDeviceIndex]->defaultLowOutputLatency;
    outputParameters->hostApiSpecificStreamInfo = NULL;
    err = Pa_OpenStream(
        &stream,
        NULL,
        outputParameters,
        sampleRate,
        FRAMES_PER_BUFFER,
        paNoFlag,
        patestCallback,
        this);

    if (err != paNoError)
        return err;

    err = Pa_SetStreamFinishedCallback(stream, &StreamFinished);
    if (err != paNoError)
        return err;

    err = Pa_StartStream(stream);
    if (err != paNoError)
        return err;
    return err;
}

Note::Note(int note, int octave, int startTime) {
    this->note = note;
    this->octave = octave;
    this->startTime = startTime;
    volEnvelopeStage = ATTACK;
    pitchEnvelopeStage = ATTACK;
    hasEnded = false;
    releaseTime = -1;
}

Oscillator::Oscillator() {
    semitone = 0;
    detune = 0;
    pulseWidth = 0.5;
    waveType = SINE;
}

VolumeEnvelope::VolumeEnvelope() {
    attack = 1;
    decay = 1;
    sustain = 0.5;
    release = 1;
}

ModEnvelope::ModEnvelope() {
    destination = PITCH;
    attack = 1;
    decay = 1;
    amount = 0;
}

Lfo::Lfo() {
    destination = CUTOFF;
    frequency = 1;
    waveType = SINE;
    amount = 0;
}

Filter::Filter() {
    type = LOWPASS;
    cutoff = 1000;
    resonance = 100;
}

Delay::Delay() {
    amount = 0;
    interval = 500;
}

Detune::Detune() {
    voices = 2;
    amount = 0;
}

Synthesizer::Synthesizer() {
    octave = 5;
    started = false;
    stream = new PaStream*;
    sampleRate = -1;
    err = 0;
    currentDeviceIndex = -1;
    mix = 0.5;
    volume = 0;
    interval = 0;
}


int Synthesizer::init() {
    err = Pa_Initialize();
    if (err != paNoError)
        return err;
    currentDeviceIndex = Pa_GetDefaultOutputDevice();
    getDevices();
    started = true;
    sampleRate = devices[currentDeviceIndex]->defaultSampleRate;
    PaStreamParameters outputParameters;

    outputParameters.device = currentDeviceIndex;

    
    const char * errorText =Pa_GetErrorText(startStream(&outputParameters));

    return err;
}
void Synthesizer::getDevices() {
    int totalDevices = Pa_GetDeviceCount();
    for (int i = 0;i < totalDevices;i++) {
        const PaDeviceInfo* device = Pa_GetDeviceInfo(i);
        if (device->maxOutputChannels)
            devices.push_back(device);
    }
}
