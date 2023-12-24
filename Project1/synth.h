#ifndef SYNTH_H
#define SYNTH_H
#include "utility.h"
enum wave
{
    SINE,
    SAW,
    SQUARE,
    TRI,
    NOISE
};
enum destination {
    CUTOFF,
    VOLUME,
    PITCH,
    OSC1_PITCH,
    OSC2_PITCH,
    PULSE_WIDTH
};

enum filterType {
    LOWPASS,
    HIGHPASS
};

enum envelopeStages {
    ATTACK,
    DECAY,
    SUSTAIN,
    RELEASE
};
class Note {
public:
    int note;
    int octave;
    int startTime;
    int releaseTime;
    envelopeStages volEnvelopeStage;
    envelopeStages pitchEnvelopeStage;
    bool hasEnded;
    Note(int note, int octave, int startTime);
};

class Oscillator
{
public:
    int semitone;
    float detune;
    wave waveType;
    float pulseWidth;
    Oscillator();
};

class VolumeEnvelope {
public:
    int attack;
    int decay;
    float sustain;
    int release;
    VolumeEnvelope();
};

class ModEnvelope {
public:
    destination destination;
    double attack;
    double decay;
    double amount;
    ModEnvelope();
};
class Lfo {
public:
    destination destination;
    double frequency;
    wave waveType;
    float amount;
    Lfo();
};

class Filter {

public:
    filterType type;
    int cutoff;
    int resonance;
    Filter();
};

class Delay {
public:
    int amount;
    int interval;
    Delay();
};

class Detune {
public:
    int voices;
    float amount;
    Detune();
};

class Synthesizer {
public:
    Oscillator oscillator1;
    Oscillator oscillator2;
    VolumeEnvelope volumeEnvelope;
    ModEnvelope modEnvelope;
    Lfo lfo;
    Filter filter;
    Delay delay;
    Detune detune;
    int octave;
    bool started;
    int interval;
    float mix;
    float volume;
    int currentDeviceIndex;
    int sampleRate;
    PaStream* stream;
    PaError err;
    std::vector<const PaDeviceInfo*> devices;
    std::vector<Note*> playedNotes;
    Synthesizer();
    int init();
    int startStream(PaStreamParameters* outputParameters);
    void getDevices();
};

#endif	
