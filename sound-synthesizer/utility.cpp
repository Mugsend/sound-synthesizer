#include "utility.h"
#include "ui.h"
#include "thread"
#include "string"
#include "mutex"
Synthesizer* synthesizerObj = new Synthesizer;
std::mutex myMutex;
bool MyApp::OnInit() {
    this->Bind(wxEVT_KEY_DOWN, &MyApp::keyPress,this);
    this->Bind(wxEVT_KEY_UP, &MyApp::keyRelease, this);
	return startSynth();
}
void MyApp::keyPress(wxKeyEvent& event) {
    char keycode = event.GetKeyCode();
    int note = -1;
    if (keycode == '1')
    {
        synthesizerObj->octave = 1;
    }
    if (keycode == '2')
    {
        synthesizerObj->octave = 1;
    }
    if (keycode == '3')
    {
        synthesizerObj->octave = 3;
    }
    if (keycode == '4')
    {
        synthesizerObj->octave = 4;
    }
    if (keycode == '5')
    {
        synthesizerObj->octave = 5;
    }
    if (keycode == '6')
    {
        synthesizerObj->octave = 6;
    }
    if (keycode == '7')
    {
        synthesizerObj->octave = 7;
    }
    if (keycode == '8')
    {
        synthesizerObj->octave = 8;
    }
    if (keycode == '9')
    {
        synthesizerObj->octave = 9;
    }
    if (keycode == 'A')
    {
        note = 0;
    }
    if (keycode == 'W')
    {
        note = 1;
    }
    if (keycode == 'S')
    {
        note = 2;
    }
    if (keycode == 'E')
    {
        note = 3;
    }
    if (keycode == 'D')
    {
        note = 4;
    }
    if (keycode == 'F')
    {
        note = 5;
    }
    if (keycode == 'T')
    {
        note = 6;
    }
    if (keycode == 'G')
    {
        note = 7;
    }
    if (keycode == 'Y')
    {
        note = 8;
    }
    if (keycode == 'H')
    {
        note = 9;
    }
    if (keycode == 'U')
    {
        note = 10;
    }
    if (keycode == 'J')
    {
        note = 11;
    }
    if (keycode == 'K')
    {
        note = 12;
    }
    if (note != -1 && !event.IsAutoRepeat()) {
        myMutex.lock();
        synthesizerObj->playedNotes.push_back(new Note(note, synthesizerObj->octave,synthesizerObj->interval));
        myMutex.unlock();
    }
    event.Skip();
}
void MyApp::keyRelease(wxKeyEvent& event) {
    char keycode = event.GetKeyCode();
    int note = -1;
    if (keycode == 'A')
    {
        note = 0;
    }
    if (keycode == 'W')
    {
        note = 1;
    }
    if (keycode == 'S')
    {
        note = 2;
    }
    if (keycode == 'E')
    {
        note = 3;
    }
    if (keycode == 'D')
    {
        note = 4;
    }
    if (keycode == 'F')
    {
        note = 5;
    }
    if (keycode == 'T')
    {
        note = 6;
    }
    if (keycode == 'G')
    {
        note = 7;
    }
    if (keycode == 'Y')
    {
        note = 8;
    }
    if (keycode == 'H')
    {
        note = 9;
    }
    if (keycode == 'U')
    {
        note = 10;
    }
    if (keycode == 'J')
    {
        note = 11;
    }
    if (keycode == 'K')
    {
        note = 12;
    }
    if (note != -1 ) {
        myMutex.lock();
        auto it = std::find_if(synthesizerObj->playedNotes.begin(), synthesizerObj->playedNotes.end(), [note](const Note* playedNote) {
            return (playedNote->note == note && playedNote->volEnvelopeStage != RELEASE);
            });

        if (it != synthesizerObj->playedNotes.end()) {
            (*it)->releaseTime = synthesizerObj->interval;
            (*it)->volEnvelopeStage = RELEASE;
        }
        myMutex.unlock();
    }
    event.Skip();
}
bool startSynth() {
    synthesizerObj->init();

	MyFrame* frame = new MyFrame();
	frame->Show(true);
	return true;
}
wxMenu* getDeviceMenu() {
	wxMenu* deviceMenu = new wxMenu;
	
	if (!synthesizerObj->devices.size())
		deviceMenu->Append(wxID_ANY, "No device");
	else {
		int deviceIndex = 0;
		for (const PaDeviceInfo* device : synthesizerObj->devices) {
			std::string deviceName = device->name;
			if (deviceIndex == synthesizerObj->currentDeviceIndex) {
				deviceName += "(Current)";
			}
			
			deviceMenu->Append(deviceIndex++, deviceName);
		}
	}
	
	return deviceMenu;
}
void changeDevice(int id) {
	synthesizerObj->currentDeviceIndex = id;
}
void changeVolume(int value) {
	synthesizerObj->volume = value/10.;
}
void changeMix(int value) {
	synthesizerObj->mix = value / 100.;
}
void changeSemitone(int value,int oscillator){
	if (oscillator == 0)
		synthesizerObj->oscillator1.semitone = value;
	else 
		synthesizerObj->oscillator2.semitone = value;
}
void changeDetune(int value, int oscillator) {
    if (oscillator == 0)
        synthesizerObj->oscillator1.detune = value;
    else
        synthesizerObj->oscillator2.detune = value;
}
void changeWave(int value,int oscillator){
	if (oscillator == 0)
		synthesizerObj->oscillator1.waveType = (wave)value;
	else
		synthesizerObj->oscillator2.waveType = (wave)value;
}
void changePulseWidth(int value, int oscillator) {
    if (oscillator == 0)
        synthesizerObj->oscillator1.pulseWidth = value/20.;
    else
        synthesizerObj->oscillator2.pulseWidth = value/20.;
}
void changeAttack(int value) {
    synthesizerObj->volumeEnvelope.attack = value * (synthesizerObj->sampleRate/1000);
}
void changeDecay(int value) {
    synthesizerObj->volumeEnvelope.decay = value * (synthesizerObj->sampleRate / 1000);
}
void changeSustain(int value) {
    synthesizerObj->volumeEnvelope.sustain = value/10.;
}
void changeRelease(int value) {
    synthesizerObj->volumeEnvelope.release = value * (synthesizerObj->sampleRate / 1000);
}
void changeLfoDestination(int value) {
    destination selection = (destination)value;
    if (value == 3)
        selection = PULSE_WIDTH;

    synthesizerObj->lfo.destination = selection;
}
void changeLfoWave(int value) {
    synthesizerObj->lfo.waveType = (wave)value;
}
void changeLfoFrequency(int value) {
    synthesizerObj->lfo.frequency = value/10.;
}
void changeLfoAmount(int value) {
    synthesizerObj->lfo.amount = value/100.;
}
void changeDetuneVoice(int value) {
    synthesizerObj->detune.voices = value;
}
void changeDetuneAmount(int value) {
    synthesizerObj->detune.amount = value;
}
void changeFilterType(int value) {
    synthesizerObj->filter.type = (filterType)value;
}
void changeFilterCutoff(int value) {
    synthesizerObj->filter.cutoff = value;
}
void changeFilterResonance(int value) {
    synthesizerObj->filter.resonance = value / 10.;
}

void changeModEnvelopeDestination(int value) {
    destination selection = PITCH;
    if (value == 1)
        selection = OSC2_PITCH;
    synthesizerObj->modEnvelope.destination = selection;
}

void changeModEnvelopeAttack(int value) {
    synthesizerObj->modEnvelope.attack = value *(synthesizerObj->sampleRate / 1000.);
}
void changeModEnvelopeDecay(int value) {
    synthesizerObj->modEnvelope.decay = value * (synthesizerObj->sampleRate / 1000.);
}
void changeModEnvelopeAmount(int value) {
    synthesizerObj->modEnvelope.amount = value/100.;
}