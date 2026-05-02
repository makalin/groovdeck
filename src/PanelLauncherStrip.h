#pragma once

#include <JuceHeader.h>
#include <array>
#include <memory>
#include "ModuleId.h"

class PanelLauncherStrip;

/** One draggable chip in the launcher strip (drag onto another chip to swap order). */
class ModuleChip final : public juce::Component
{
public:
    ModuleChip (PanelLauncherStrip& ownerRef, ModuleId module);

    void paint (juce::Graphics& g) override;
    void mouseDown (const juce::MouseEvent& e) override;
    void mouseDoubleClick (const juce::MouseEvent& e) override;

    ModuleId getModuleId() const { return moduleId; }

private:
    PanelLauncherStrip& owner;
    ModuleId moduleId;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModuleChip)
};

/** Bottom strip: ordered module chips, drag–drop to reorder, double-click to show window. */
class PanelLauncherStrip final : public juce::Component,
                                 public juce::DragAndDropContainer,
                                 public juce::DragAndDropTarget
{
public:
    PanelLauncherStrip();

    void setPanelOrder (const juce::Array<ModuleId>& order);
    juce::Array<ModuleId> getPanelOrder() const { return panelOrder; }

    void resized() override;
    void paint (juce::Graphics& g) override;

    bool isInterestedInDragSource (const SourceDetails& dragSourceDetails) override;
    void itemDragEnter (const SourceDetails& dragSourceDetails) override;
    void itemDragMove (const SourceDetails& dragSourceDetails) override;
    void itemDragExit (const SourceDetails& dragSourceDetails) override;
    void itemDropped (const SourceDetails& dragSourceDetails) override;

    std::function<void (ModuleId)> onRequestOpenModule;
    std::function<void (const juce::Array<ModuleId>&)> onOrderChanged;

    bool isDropHighlight (const ModuleChip* chip) const noexcept { return chip != nullptr && chip == hoverTargetChip; }

    static juce::Array<ModuleId> defaultPanelOrder();
    static juce::Array<ModuleId> parseOrderString (const juce::String& s);
    static juce::String orderToString (const juce::Array<ModuleId>& order);

private:
    juce::Array<ModuleId> panelOrder;
    std::array<std::unique_ptr<ModuleChip>, static_cast<size_t> (ModuleId::numModules)> chips {};
    ModuleChip* hoverTargetChip = nullptr;

    ModuleChip* findChipAt (juce::Point<int> localPoint) const;
    void layoutChips();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PanelLauncherStrip)
};
