#include "SequencerPanel.h"
#include "GroovDeckLookAndFeel.h"

SequencerPanel::SequencerPanel(Sequencer& seq)
    : sequencer(seq)
{
    // Setup control buttons
    startButton.setButtonText("Start");
    stopButton.setButtonText("Stop");
    resetButton.setButtonText("Reset");
    clearButton.setButtonText("Clear");
    randomButton.setButtonText("Random");
    shiftLeftButton.setButtonText("<<");
    shiftRightButton.setButtonText(">>");
    
    // Setup step buttons
    for (int i = 0; i < 16; ++i)
    {
        stepButtons[i].setButtonText(juce::String(i + 1));
        stepButtons[i].setClickingTogglesState(true);
    }
    
    // Setup sliders
    setupSlider(tempoSlider, tempoLabel, "Tempo (BPM)", 60.0, 200.0, 1.0, 120.0);
    setupSlider(stepsSlider, stepsLabel, "Steps", 4.0, 16.0, 1.0, 16.0);
    
    tempoSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 36, 14);
    stepsSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 36, 14);
    tempoSlider.setName("Tempo");
    stepsSlider.setName("Steps");

    for (auto* lb : { &tempoLabel, &stepsLabel })
    {
        lb->setColour(juce::Label::textColourId, GroovDeckLookAndFeel::textMuted());
        lb->setFont(juce::Font(juce::FontOptions(9.0f)));
    }
    
    // Add components
    addAndMakeVisible(startButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(resetButton);
    addAndMakeVisible(clearButton);
    addAndMakeVisible(randomButton);
    addAndMakeVisible(shiftLeftButton);
    addAndMakeVisible(shiftRightButton);
    
    for (auto& button : stepButtons)
    {
        addAndMakeVisible(button);
    }
    
    addAndMakeVisible(tempoSlider);
    addAndMakeVisible(stepsSlider);
    addAndMakeVisible(tempoLabel);
    addAndMakeVisible(stepsLabel);
    
    // Add listeners
    startButton.addListener(this);
    stopButton.addListener(this);
    resetButton.addListener(this);
    clearButton.addListener(this);
    randomButton.addListener(this);
    shiftLeftButton.addListener(this);
    shiftRightButton.addListener(this);
    
    for (auto& button : stepButtons)
    {
        button.addListener(this);
    }
    
    tempoSlider.addListener(this);
    stepsSlider.addListener(this);
    
    updateButtonStates();
    updateStepButtons();
}

SequencerPanel::~SequencerPanel()
{
    // Remove listeners
    startButton.removeListener(this);
    stopButton.removeListener(this);
    resetButton.removeListener(this);
    clearButton.removeListener(this);
    randomButton.removeListener(this);
    shiftLeftButton.removeListener(this);
    shiftRightButton.removeListener(this);
    
    for (auto& button : stepButtons)
    {
        button.removeListener(this);
    }
    
    tempoSlider.removeListener(this);
    stepsSlider.removeListener(this);
}

void SequencerPanel::paint(juce::Graphics& g)
{
    GroovDeckLookAndFeel::drawModulePanel(g, getLocalBounds(), "Step sequencer");
}

void SequencerPanel::resized()
{
    auto a = getLocalBounds().reduced(5, 3);
    a.removeFromTop(17);

    const int gap = 3;
    const int bh = 22;
    int x = a.getX();
    int y = a.getY();
    const int w = a.getWidth();

    const int q = (w - 3 * gap) / 4;
    startButton.setBounds(x, y, q, bh);
    stopButton.setBounds(x + q + gap, y, q, bh);
    resetButton.setBounds(x + 2 * (q + gap), y, q, bh);
    clearButton.setBounds(x + 3 * (q + gap), y, q, bh);
    y += bh + gap;

    const int t = (w - 2 * gap) / 3;
    randomButton.setBounds(x, y, t, bh);
    shiftLeftButton.setBounds(x + t + gap, y, t, bh);
    shiftRightButton.setBounds(x + 2 * (t + gap), y, t, bh);
    y += bh + gap;

    const int lh = 15;
    tempoLabel.setBounds(x, y, 56, lh);
    tempoSlider.setBounds(x + 58, y, w - 60, lh);
    y += lh + gap;
    stepsLabel.setBounds(x, y, 56, lh);
    stepsSlider.setBounds(x + 58, y, w - 60, lh);
    y += lh + gap;

    auto stepArea = juce::Rectangle<int>(x, y, w, a.getBottom() - y);
    const int bw = stepArea.getWidth() / 4;
    const int bhh = juce::jmax(12, stepArea.getHeight() / 4);

    for (int i = 0; i < 16; ++i)
    {
        const int row = i / 4;
        const int col = i % 4;
        stepButtons[i].setBounds(stepArea.getX() + col * bw + 1,
                                 stepArea.getY() + row * bhh + 1,
                                 bw - 2,
                                 bhh - 2);
    }
}

void SequencerPanel::buttonClicked(juce::Button* button)
{
    if (button == &startButton)
    {
        sequencer.start();
    }
    else if (button == &stopButton)
    {
        sequencer.stop();
    }
    else if (button == &resetButton)
    {
        sequencer.reset();
    }
    else if (button == &clearButton)
    {
        sequencer.clearPattern();
        updateStepButtons();
    }
    else if (button == &randomButton)
    {
        sequencer.randomizePattern();
        updateStepButtons();
    }
    else if (button == &shiftLeftButton)
    {
        sequencer.shiftPattern(-1);
        updateStepButtons();
    }
    else if (button == &shiftRightButton)
    {
        sequencer.shiftPattern(1);
        updateStepButtons();
    }
    else
    {
        // Check if it's a step button
        for (int i = 0; i < 16; ++i)
        {
            if (button == &stepButtons[i])
            {
                sequencer.setStepActive(i, button->getToggleState());
                break;
            }
        }
    }
    
    updateButtonStates();
}

void SequencerPanel::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &tempoSlider)
    {
        sequencer.setTempo(tempoSlider.getValue());
    }
    else if (slider == &stepsSlider)
    {
        sequencer.setSteps(static_cast<int>(stepsSlider.getValue()));
        updateStepButtons();
    }
}

void SequencerPanel::updateButtonStates()
{
    startButton.setEnabled(!sequencer.isPlaying());
    stopButton.setEnabled(sequencer.isPlaying());
    resetButton.setEnabled(true);
    clearButton.setEnabled(true);
    randomButton.setEnabled(true);
    shiftLeftButton.setEnabled(true);
    shiftRightButton.setEnabled(true);
}

void SequencerPanel::updateStepButtons()
{
    int numSteps = sequencer.getNumSteps();
    for (int i = 0; i < 16; ++i)
    {
        stepButtons[i].setEnabled(i < numSteps);
        if (i < numSteps)
        {
            stepButtons[i].setToggleState(sequencer.getStepActive(i), juce::dontSendNotification);
        }
    }
}

void SequencerPanel::setupSlider(juce::Slider& slider, juce::Label& label, const juce::String& name,
                                double min, double max, double interval, double defaultValue)
{
    slider.setRange(min, max, interval);
    slider.setValue(defaultValue);
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    label.setText(name, juce::dontSendNotification);
} 