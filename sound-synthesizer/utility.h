#ifndef UTILITY_H
#define UTILITY_H
#include <wx/wx.h>
#include "portaudio.h"
#include <vector>
#include "synth.h"

wxMenu* getDeviceMenu();
bool startSynth();
void changeDevice(int id);
void changeVolume(int value);
void changeMix(int value);
void changeSemitone(int value, int oscillator);
void changeDetune(int value, int oscillator);
void changeWave(int value,int oscillator);
void changePulseWidth(int value, int oscillator);
void changeAttack(int value);
void changeDecay(int value);
void changeSustain(int value);
void changeRelease(int value);
void changeLfoDestination(int value);
void changeLfoWave(int value);
void changeLfoFrequency(int value);
void changeLfoAmount(int value);
void changeDetuneVoice(int value);
void changeDetuneAmount(int value);
void changeFilterType(int value);
void changeFilterCutoff(int value);
void changeFilterResonance(int value);
void changeModEnvelopeDestination(int value);
void changeModEnvelopeAttack(int value);
void changeModEnvelopeDecay(int value);
void changeModEnvelopeAmount(int value);
class MyApp : public wxApp
{
public:
	bool OnInit() override;
	void keyPress(wxKeyEvent& event);
	void keyRelease(wxKeyEvent& event);
};

#endif 



