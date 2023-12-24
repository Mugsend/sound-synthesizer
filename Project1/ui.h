#ifndef UI_H
#define UI_H

class MyWindow : public wxScrolled<wxWindow>
{
public:
    MyWindow(wxWindow* parent, wxWindowID id = wxID_ANY);
private:
    void OnOscWaveTypeChoiceSelect(wxCommandEvent& event);
    void OnOscPulseWidthSliderScroll(wxScrollEvent& event);
    void OnOscSemitoneSliderScroll(wxScrollEvent& event);
    void OnOscDetuneSliderScroll(wxScrollEvent& event);
    void OnMixSliderScroll(wxScrollEvent& event);
    void OnVolumeSliderScroll(wxScrollEvent& event);
    void OnAttackSliderScroll(wxScrollEvent& event);
    void OnDecaySliderScroll(wxScrollEvent& event);
    void OnSustainSliderScroll(wxScrollEvent& event);
    void OnReleaseSliderScroll(wxScrollEvent& event);
    void OnModEnvelopeDestinationChoiceSelect(wxCommandEvent& event);
    void OnModEnvelopeAttackSliderScroll(wxScrollEvent& event);
    void OnModEnvelopeDecaySliderScroll(wxScrollEvent& event);
    void OnModEnvelopeAmountSliderScroll(wxScrollEvent& event);
    void OnLfoDestinationChoiceSelect(wxCommandEvent& event);
    void OnLfoFrequencySliderScroll(wxScrollEvent& event);
    void OnLfoWaveChoiceSelect(wxCommandEvent& event);
    void OnLfoAmountSliderScroll(wxScrollEvent& event);
    void OnDetuneVoiceSliderScroll(wxScrollEvent& event);
    void OnDetuneAmountSliderScroll(wxScrollEvent& event);
    void OnFilterTypeChoiceSelect(wxCommandEvent& event);
    void OnFilterCutoffSliderScroll(wxScrollEvent& event);
    void OnFilterResonanceSliderScroll(wxScrollEvent& event);
};

class MyFrame : public wxFrame
{
public:
    MyFrame();
private:
    void OnDeviceSelect(wxCommandEvent& event);
};
#endif
