#include "SequencerPanel.h"

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
    
    // Setup labels
    sequencerLabel.setText("Sequencer", juce::dontSendNotification);
    sequencerLabel.setJustificationType(juce::Justification::centred);
    
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
    addAndMakeVisible(sequencerLabel);
    
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
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void SequencerPanel::resized()
{
    auto area = getLocalBounds();
    auto buttonHeight = 30;
    auto margin = 10;
    
    // Title
    sequencerLabel.setBounds(area.removeFromTop(30).reduced(margin));
    
    // Control buttons
    auto controlArea = area.removeFromTop(buttonHeight * 2).reduced(margin);
    startButton.setBounds(controlArea.removeFromTop(buttonHeight).removeFromLeft(controlArea.getWidth() / 4).reduced(5));
    stopButton.setBounds(controlArea.removeFromLeft(controlArea.getWidth() / 3).reduced(5));
    resetButton.setBounds(controlArea.removeFromLeft(controlArea.getWidth() / 2).reduced(5));
    clearButton.setBounds(controlArea.reduced(5));
    
    auto secondRow = controlArea.removeFromTop(buttonHeight);
    randomButton.setBounds(secondRow.removeFromLeft(secondRow.getWidth() / 3).reduced(5));
    shiftLeftButton.setBounds(secondRow.removeFromLeft(secondRow.getWidth() / 2).reduced(5));
    shiftRightButton.setBounds(secondRow.reduced(5));
    
    // Parameters
    auto paramArea = area.removeFromTop(60).reduced(margin);
    tempoSlider.setBounds(paramArea.removeFromTop(30).reduced(5));
    stepsSlider.setBounds(paramArea.removeFromTop(30).reduced(5));
    
    // Step buttons (4x4 grid)
    auto stepArea = area.reduced(margin);
    int buttonWidth = stepArea.getWidth() / 4;
    int buttonHeight2 = stepArea.getHeight() / 4;
    
    for (int i = 0; i < 16; ++i)
    {
        int row = i / 4;
        int col = i % 4;
        stepButtons[i].setBounds(stepArea.getX() + col * buttonWidth,
                                stepArea.getY() + row * buttonHeight2,
                                buttonWidth - 2, buttonHeight2 - 2);
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