#include <wx/wx.h>
#include <string>
#include "ui.h"
#include "utility.h"
#include "cmath"


enum
{
    ID_OSC1_SLIDER = 0,
    ID_OSC2_SLIDER = 1,
};
MyWindow::MyWindow(wxWindow* parent, wxWindowID id)
    : wxScrolled(parent, id) {


    wxPanel* osc1Panel = new wxPanel(this, wxID_ANY, wxPoint(0, 100), wxDefaultSize, wxBORDER);

    wxPanel* osc2Panel = new wxPanel(this, wxID_ANY, wxPoint(10, 10), wxDefaultSize, wxBORDER);


    wxPanel* volumeEnvelopePanel = new wxPanel(this, wxID_ANY, wxPoint(10, 10), wxDefaultSize, wxBORDER);
    wxPanel* modEnvelopePanel = new wxPanel(this, wxID_ANY, wxPoint(10, 10), wxDefaultSize, wxBORDER);
    wxPanel* lfoPanel = new wxPanel(this, wxID_ANY, wxPoint(10, 10), wxDefaultSize, wxBORDER);

    wxPanel* detunePanel = new wxPanel(this, wxID_ANY, wxPoint(10, 10), wxDefaultSize, wxBORDER);
    wxPanel* filterPanel = new wxPanel(this, wxID_ANY, wxPoint(10, 10), wxDefaultSize, wxBORDER);

    wxPanel* volumePanel = new wxPanel(this, wxID_ANY, wxPoint(10, 10), wxDefaultSize, wxBORDER);

    auto mainSizer = new wxGridSizer(2,4,0,0);
    wxBoxSizer* osc1Sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* osc2Sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* volumeEnvelopeSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* modEnvelopeSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* lfoSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* detuneSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* filterSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* volumeSizer = new wxBoxSizer(wxVERTICAL);

    wxArrayString waveTypes;
    waveTypes.Add("Sine");
    waveTypes.Add("Saw");
    waveTypes.Add("Square");
    waveTypes.Add("Tri");


    wxStaticText* osc1_wave_type_label = new wxStaticText(osc1Panel, wxID_ANY, "Wave1 :");
    wxChoice* osc1_wave_type_choice = new wxChoice(osc1Panel, ID_OSC1_SLIDER, wxDefaultPosition, wxDefaultSize, waveTypes);
    osc1_wave_type_choice->Select(0);
    osc1_wave_type_choice->Bind(wxEVT_CHOICE, &MyWindow::OnOscWaveTypeChoiceSelect, this);

    wxStaticText* osc1_pulse_width_label = new wxStaticText(osc1Panel, wxID_ANY, "Pulse Width1:");
    wxSlider* osc1_pulse_width_slider = new wxSlider(osc1Panel, ID_OSC1_SLIDER, 10, 1, 10, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    osc1_pulse_width_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnOscPulseWidthSliderScroll, this);

    wxStaticText* osc1_semitone_label = new wxStaticText(osc1Panel, wxID_ANY, "Semitone1:");
    wxSlider* osc1_semitone_slider = new wxSlider(osc1Panel, ID_OSC1_SLIDER, 0, -12, 12, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    osc1_semitone_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnOscSemitoneSliderScroll, this);

    wxStaticText* osc1_detune_label = new wxStaticText(osc1Panel, wxID_ANY, "Detune1:");
    wxSlider* osc1_detune_slider = new wxSlider(osc1Panel, ID_OSC1_SLIDER, 0, -300, 300, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    osc1_detune_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnOscDetuneSliderScroll, this);

    wxStaticText* osc2_wave_type_label = new wxStaticText(osc2Panel, wxID_ANY, "Wave2 :");
    wxChoice* osc2_wave_type_choice = new wxChoice(osc2Panel, ID_OSC2_SLIDER, wxDefaultPosition, wxDefaultSize, waveTypes);
    osc2_wave_type_choice->Select(0);
    osc2_wave_type_choice->Bind(wxEVT_CHOICE, &MyWindow::OnOscWaveTypeChoiceSelect, this);

    wxStaticText* osc2_pulse_width_label = new wxStaticText(osc2Panel, wxID_ANY, "Pulse Width2:");
    wxSlider* osc2_pulse_width_slider = new wxSlider(osc2Panel, ID_OSC2_SLIDER, 10, 1, 10, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    osc2_pulse_width_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnOscPulseWidthSliderScroll, this);

    wxStaticText* osc2_semitone_label = new wxStaticText(osc2Panel, wxID_ANY, "Semitone2:");
    wxSlider* osc2_semitone_slider = new wxSlider(osc2Panel, ID_OSC2_SLIDER, 0, -12, 12, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    osc2_semitone_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnOscSemitoneSliderScroll, this);

    wxStaticText* osc2_detune_label = new wxStaticText(osc2Panel, wxID_ANY, "Detune2:");
    wxSlider* osc2_detune_slider = new wxSlider(osc2Panel, ID_OSC2_SLIDER, 0, -300, 300, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    osc2_detune_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnOscDetuneSliderScroll, this);

    wxArrayString filterTypes;

    filterTypes.Add("Lowpass");
    filterTypes.Add("Highpass");

    wxStaticText* filter_type_label = new wxStaticText(filterPanel, wxID_ANY, "Type:");
    wxChoice* filter_type_choice = new wxChoice(filterPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, filterTypes);
    filter_type_choice->Select(0);
    filter_type_choice->Bind(wxEVT_CHOICE, &MyWindow::OnFilterTypeChoiceSelect, this);

    wxStaticText* filter_cutoff_label = new wxStaticText(filterPanel, wxID_ANY, "Cutoff:");
    wxSlider* filter_cutoff_slider = new wxSlider(filterPanel, wxID_ANY, 1000, 0, 22000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    filter_cutoff_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnFilterCutoffSliderScroll, this);

    wxStaticText* filter_resonance_label = new wxStaticText(filterPanel, wxID_ANY, "Resonance:");
    wxSlider* filter_resonance_slider = new wxSlider(filterPanel, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    filter_resonance_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnFilterResonanceSliderScroll, this);


    wxStaticText* mix_label = new wxStaticText(volumePanel, wxID_ANY, "Mix:");
    wxSlider* mix_slider = new wxSlider(volumePanel, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    mix_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnMixSliderScroll, this);

    wxStaticText* volume_label = new wxStaticText(volumePanel, wxID_ANY, "Volume:");
    wxSlider* volume_slider = new wxSlider(volumePanel, wxID_ANY, 0, -10, 10, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    volume_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnVolumeSliderScroll, this);

    wxStaticText* attack_label = new wxStaticText(volumeEnvelopePanel, wxID_ANY, "Attack:");
    wxSlider* attack_slider = new wxSlider(volumeEnvelopePanel, wxID_ANY, 1, 1, 2000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    attack_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnAttackSliderScroll, this);

    wxStaticText* decay_label = new wxStaticText(volumeEnvelopePanel, wxID_ANY, "Decay:");
    wxSlider* decay_slider = new wxSlider(volumeEnvelopePanel, wxID_ANY, 1, 1, 1000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    decay_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnDecaySliderScroll, this);

    wxStaticText* sustain_label = new wxStaticText(volumeEnvelopePanel, wxID_ANY, "Sustain:");
    wxSlider* sustain_slider = new wxSlider(volumeEnvelopePanel, wxID_ANY, 5, 0, 10, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    sustain_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnSustainSliderScroll, this);

    wxStaticText* release_label = new wxStaticText(volumeEnvelopePanel, wxID_ANY, "Release:");
    wxSlider* release_slider = new wxSlider(volumeEnvelopePanel, wxID_ANY, 1, 1, 15000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    release_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnReleaseSliderScroll, this);

    wxArrayString modEnvelopeDestinations;
    modEnvelopeDestinations.Add("Pitch");
    modEnvelopeDestinations.Add("Osc2 Pitch");

    wxStaticText* modEnvelope_destination_label = new wxStaticText(modEnvelopePanel, wxID_ANY, "Destination:");
    wxChoice* modEnvelope_destination_choice = new wxChoice(modEnvelopePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, modEnvelopeDestinations);
    modEnvelope_destination_choice->Select(0);
    modEnvelope_destination_choice->Bind(wxEVT_CHOICE, &MyWindow::OnModEnvelopeDestinationChoiceSelect, this);

    wxStaticText* modEnvelope_attack_label = new wxStaticText(modEnvelopePanel, wxID_ANY, "Attack:");
    wxSlider* modEnvelope_attack_slider = new wxSlider(modEnvelopePanel, wxID_ANY, 1, 1, 2000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    modEnvelope_attack_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnModEnvelopeAttackSliderScroll, this);

    wxStaticText* modEnvelope_decay_label = new wxStaticText(modEnvelopePanel, wxID_ANY, "Decay:");
    wxSlider* modEnvelope_decay_slider = new wxSlider(modEnvelopePanel, wxID_ANY, 1, 1, 1000, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    modEnvelope_decay_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnModEnvelopeDecaySliderScroll, this);

    wxStaticText* modEnvelope_amount_label = new wxStaticText(modEnvelopePanel, wxID_ANY, "Amount:");
    wxSlider* modEnvelope_amount_slider = new wxSlider(modEnvelopePanel, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    modEnvelope_amount_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnModEnvelopeAmountSliderScroll, this);

    wxArrayString lfoDestinations;
    lfoDestinations.Add("Cutoff");
    lfoDestinations.Add("Volume");
    lfoDestinations.Add("Pitch");
    lfoDestinations.Add("Pulse Width");

    wxStaticText* lfo_destination_label = new wxStaticText(lfoPanel, wxID_ANY, "Destination:");
    wxChoice* lfo_destination_choice = new wxChoice(lfoPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, lfoDestinations);
    lfo_destination_choice->Select(0);
    lfo_destination_choice->Bind(wxEVT_CHOICE, &MyWindow::OnLfoDestinationChoiceSelect, this);

    wxStaticText* frequency_label = new wxStaticText(lfoPanel, wxID_ANY, "Frequency:");
    wxSlider* frequency_slider = new wxSlider(lfoPanel, wxID_ANY, 1, 1, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    frequency_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnLfoFrequencySliderScroll, this);

    wxArrayString lfoWaves;
    lfoWaves.Add("SINE");
    lfoWaves.Add("SAW");
    lfoWaves.Add("SQUARE");
    lfoWaves.Add("TRI");
    lfoWaves.Add("NOISE");

    wxStaticText* lfo_wave_label = new wxStaticText(lfoPanel, wxID_ANY, "Wave:");
    wxChoice* lfo_wave_choice = new wxChoice(lfoPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, lfoWaves);
    lfo_wave_choice->Select(0);
    lfo_wave_choice->Bind(wxEVT_CHOICE, &MyWindow::OnLfoWaveChoiceSelect, this);

    wxStaticText* amount_label = new wxStaticText(lfoPanel, wxID_ANY, "Amount:");
    wxSlider* amount_slider = new wxSlider(lfoPanel, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    amount_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnLfoAmountSliderScroll, this);

    wxStaticText* detune_voice_label = new wxStaticText(detunePanel, wxID_ANY, "Voices:");
    wxSlider* detune_voice_slider = new wxSlider(detunePanel, wxID_ANY, 2, 2, 4, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    detune_voice_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnDetuneVoiceSliderScroll, this);

    wxStaticText* detune_amount_label = new wxStaticText(detunePanel, wxID_ANY, "Amount:");
    wxSlider* detune_amount_slider = new wxSlider(detunePanel, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
    detune_amount_slider->Bind(wxEVT_SCROLL_CHANGED, &MyWindow::OnDetuneAmountSliderScroll, this);

    osc1Sizer->Add(osc1_wave_type_label, 0, wxEXPAND | wxALL, 0);
    osc1Sizer->Add(osc1_wave_type_choice, 0, wxEXPAND | wxALL, 0);
    osc1Sizer->Add(osc1_pulse_width_label, 0, wxEXPAND | wxALL, 0);
    osc1Sizer->Add(osc1_pulse_width_slider, 0, wxEXPAND | wxALL, 0);
    osc1Sizer->Add(osc1_semitone_label, 0, wxEXPAND | wxALL, 0);
    osc1Sizer->Add(osc1_semitone_slider, 0, wxEXPAND | wxALL, 0);
    osc1Sizer->Add(osc1_detune_label, 0, wxEXPAND | wxALL, 0);
    osc1Sizer->Add(osc1_detune_slider, 0, wxEXPAND | wxALL, 0);

    osc2Sizer->Add(osc2_wave_type_label, 0, wxEXPAND | wxALL, 0);
    osc2Sizer->Add(osc2_wave_type_choice, 0, wxEXPAND | wxALL, 0);
    osc2Sizer->Add(osc2_pulse_width_label, 0, wxEXPAND | wxALL, 0);
    osc2Sizer->Add(osc2_pulse_width_slider, 0, wxEXPAND | wxALL, 0);
    osc2Sizer->Add(osc2_semitone_label, 0, wxEXPAND | wxALL, 0);
    osc2Sizer->Add(osc2_semitone_slider, 0, wxEXPAND | wxALL, 0);
    osc2Sizer->Add(osc2_detune_label, 0, wxEXPAND | wxALL, 0);
    osc2Sizer->Add(osc2_detune_slider, 0, wxEXPAND | wxALL, 0);

    volumeEnvelopeSizer->Add(attack_label, 0, wxEXPAND | wxALL, 0);
    volumeEnvelopeSizer->Add(attack_slider, 0, wxEXPAND | wxALL, 0);
    volumeEnvelopeSizer->Add(decay_label, 0, wxEXPAND | wxALL, 0);
    volumeEnvelopeSizer->Add(decay_slider, 0, wxEXPAND | wxALL, 0);
    volumeEnvelopeSizer->Add(sustain_label, 0, wxEXPAND | wxALL, 0);
    volumeEnvelopeSizer->Add(sustain_slider, 0, wxEXPAND | wxALL, 0);
    volumeEnvelopeSizer->Add(release_label, 0, wxEXPAND | wxALL, 0);
    volumeEnvelopeSizer->Add(release_slider, 0, wxEXPAND | wxALL, 0);

    modEnvelopeSizer->Add(modEnvelope_destination_label);
    modEnvelopeSizer->Add(modEnvelope_destination_choice);
    modEnvelopeSizer->Add(modEnvelope_attack_label);
    modEnvelopeSizer->Add(modEnvelope_attack_slider);
    modEnvelopeSizer->Add(modEnvelope_decay_label);
    modEnvelopeSizer->Add(modEnvelope_decay_slider);
    modEnvelopeSizer->Add(modEnvelope_amount_label);
    modEnvelopeSizer->Add(modEnvelope_amount_slider);

    lfoSizer->Add(lfo_destination_label, 0, wxEXPAND | wxALL, 0);
    lfoSizer->Add(lfo_destination_choice, 0, wxALL, 0);
    lfoSizer->Add(frequency_label, 0, wxEXPAND | wxALL, 0);
    lfoSizer->Add(frequency_slider, 0, wxEXPAND | wxALL, 0);
    lfoSizer->Add(lfo_wave_label, 0, wxEXPAND | wxALL, 0);
    lfoSizer->Add(lfo_wave_choice, 0, wxALL, 0);
    lfoSizer->Add(amount_label, 0, wxEXPAND | wxALL, 0);
    lfoSizer->Add(amount_slider, 0, wxEXPAND | wxALL, 0);

    detuneSizer->Add(detune_voice_label, 0, wxEXPAND | wxALL, 0);
    detuneSizer->Add(detune_voice_slider, 0, wxEXPAND | wxALL, 0);
    detuneSizer->Add(detune_amount_label, 0, wxEXPAND | wxALL, 0);
    detuneSizer->Add(detune_amount_slider, 0, wxEXPAND | wxALL, 0);

    filterSizer->Add(filter_type_label);
    filterSizer->Add(filter_type_choice);
    filterSizer->Add(filter_cutoff_label);
    filterSizer->Add(filter_cutoff_slider);
    filterSizer->Add(filter_resonance_label);
    filterSizer->Add(filter_resonance_slider);


    volumeSizer->Add(volume_label, 0, wxEXPAND | wxALL, 0);
    volumeSizer->Add(volume_slider, 0, wxEXPAND | wxALL, 0);
    volumeSizer->Add(mix_label, 0, wxEXPAND | wxALL, 0);
    volumeSizer->Add(mix_slider, 0, wxEXPAND | wxALL, 0);

    osc1Panel->SetSizer(osc1Sizer);
    volumeEnvelopePanel->SetSizer(volumeEnvelopeSizer);
    modEnvelopePanel->SetSizer(modEnvelopeSizer);
    osc2Panel->SetSizer(osc2Sizer);
    lfoPanel->SetSizer(lfoSizer);
    detunePanel->SetSizer(detuneSizer);
    filterPanel->SetSizer(filterSizer);
    volumePanel->SetSizer(volumeSizer);
    mainSizer->Add(osc1Panel);
    mainSizer->Add(osc2Panel);
    mainSizer->Add(volumeEnvelopePanel);
    mainSizer->Add(modEnvelopePanel);
    mainSizer->Add(lfoPanel);
    mainSizer->Add(detunePanel);
    mainSizer->Add(filterPanel);
    mainSizer->Add(volumePanel);


    this->SetSizer(mainSizer);
}

MyFrame::MyFrame()
    : wxFrame(nullptr, wxID_ANY, "Synthesizer",wxDefaultPosition, wxSize(450,500), wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)
{   
    wxMenu* deviceMenu = getDeviceMenu();
    deviceMenu->Bind(wxEVT_MENU, &MyFrame::OnDeviceSelect, this);
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(deviceMenu, "Devices");
    SetMenuBar(menuBar);
    MyWindow* scrolledWindow = new MyWindow(this);
}

void MyWindow::OnOscWaveTypeChoiceSelect(wxCommandEvent& event) {
    changeWave(event.GetInt(), event.GetId());
    wxClientData *nig = event.GetClientObject();
}

void MyWindow::OnOscPulseWidthSliderScroll(wxScrollEvent& event) {
    changePulseWidth(event.GetInt(), event.GetId());
}

void MyWindow::OnOscSemitoneSliderScroll(wxScrollEvent& event) {
    changeSemitone(event.GetInt(),event.GetId());
}

void MyWindow::OnOscDetuneSliderScroll(wxScrollEvent& event) {
    changeDetune(event.GetInt(), event.GetId());
}
void MyWindow::OnMixSliderScroll(wxScrollEvent& event) {
    changeMix(event.GetInt());
}
void MyWindow::OnVolumeSliderScroll(wxScrollEvent& event) {
    changeVolume(event.GetInt());
}
void MyFrame::OnDeviceSelect(wxCommandEvent& event) {
    changeDevice(event.GetId());
}
void MyWindow::OnAttackSliderScroll(wxScrollEvent& event) {
    changeAttack(event.GetInt());
}
void MyWindow::OnDecaySliderScroll(wxScrollEvent& event) {
    changeDecay(event.GetInt());
}
void MyWindow::OnSustainSliderScroll(wxScrollEvent& event) {
    changeSustain(event.GetInt());
}
void MyWindow::OnReleaseSliderScroll(wxScrollEvent& event) {
    changeRelease(event.GetInt());
}
void MyWindow::OnLfoDestinationChoiceSelect(wxCommandEvent& event) {
    changeLfoDestination(event.GetInt());
}
void MyWindow::OnLfoFrequencySliderScroll(wxScrollEvent& event) {
    changeLfoFrequency(event.GetInt());
}
void MyWindow::OnLfoWaveChoiceSelect(wxCommandEvent& event) {
    changeLfoWave(event.GetInt());
}
void MyWindow::OnLfoAmountSliderScroll(wxScrollEvent& event) {
    changeLfoAmount(event.GetInt());
}
void MyWindow::OnDetuneVoiceSliderScroll(wxScrollEvent& event) {
    changeDetuneVoice(event.GetInt());
}
void MyWindow::OnDetuneAmountSliderScroll(wxScrollEvent& event) {
    changeDetuneAmount(event.GetInt());
}
void MyWindow::OnFilterTypeChoiceSelect(wxCommandEvent& event) {
    changeFilterType(event.GetInt());
}
void MyWindow::OnFilterCutoffSliderScroll(wxScrollEvent& event) {
    changeFilterCutoff(event.GetInt());
}
void MyWindow::OnFilterResonanceSliderScroll(wxScrollEvent& event) {
    changeFilterResonance(event.GetInt());
}
void MyWindow::OnModEnvelopeDestinationChoiceSelect(wxCommandEvent& event) {
    changeModEnvelopeDestination(event.GetInt());
}
void MyWindow::OnModEnvelopeAttackSliderScroll(wxScrollEvent& event) {
    changeModEnvelopeAttack(event.GetInt());
}
void MyWindow::OnModEnvelopeDecaySliderScroll(wxScrollEvent& event) {
    changeModEnvelopeDecay(event.GetInt());
}
void MyWindow::OnModEnvelopeAmountSliderScroll(wxScrollEvent& event) {
    changeModEnvelopeAmount(event.GetInt());
}