#include "PanelLauncherStrip.h"
#include "GroovDeckLookAndFeel.h"

namespace
{
juce::String shortName (ModuleId id)
{
    switch (id)
    {
        case ModuleId::Effects:       return "FX";
        case ModuleId::LiveLoop:      return "Loop";
        case ModuleId::Sequencer:     return "Seq";
        case ModuleId::SampleSlicer:  return "Slice";
        case ModuleId::RackMixer:     return "Mix";
        case ModuleId::RackRouting:   return "I/O";
        default:                      return "?";
    }
}

constexpr auto dragIdPrefix = "gdModule:";
}

ModuleChip::ModuleChip (PanelLauncherStrip& ownerRef, ModuleId module)
    : owner (ownerRef), moduleId (module)
{
}

void ModuleChip::paint (juce::Graphics& g)
{
    auto r = getLocalBounds().toFloat().reduced (1.0f);
    const bool hi = owner.isDropHighlight (this);

    g.setColour (GroovDeckLookAndFeel::surfaceElevated().brighter (hi ? 0.12f : 0.0f));
    g.fillRoundedRectangle (r, 5.0f);
    g.setColour (hi ? GroovDeckLookAndFeel::accent() : GroovDeckLookAndFeel::border());
    g.drawRoundedRectangle (r, 5.0f, hi ? 2.0f : 1.0f);

    g.setColour (GroovDeckLookAndFeel::text());
    g.setFont (juce::Font (juce::FontOptions (10.5f, juce::Font::bold)));
    g.drawFittedText (shortName (moduleId), getLocalBounds().reduced (2), juce::Justification::centred, 1);
}

void ModuleChip::mouseDown (const juce::MouseEvent& e)
{
    juce::ignoreUnused (e);
    if (auto* c = juce::DragAndDropContainer::findParentDragContainerFor (this))
        c->startDragging (dragIdPrefix + juce::String (static_cast<int> (moduleId)),
                          this,
                          juce::ScaledImage(),
                          true,
                          nullptr,
                          &e.source);
}

void ModuleChip::mouseDoubleClick (const juce::MouseEvent& e)
{
    juce::ignoreUnused (e);
    if (owner.onRequestOpenModule)
        owner.onRequestOpenModule (moduleId);
}

juce::Array<ModuleId> PanelLauncherStrip::defaultPanelOrder()
{
    juce::Array<ModuleId> o;
    for (int i = 0; i < static_cast<int> (ModuleId::numModules); ++i)
        o.add (static_cast<ModuleId> (i));
    return o;
}

juce::Array<ModuleId> PanelLauncherStrip::parseOrderString (const juce::String& s)
{
    juce::Array<ModuleId> out;
    if (s.isEmpty())
        return defaultPanelOrder();

    juce::StringArray parts;
    parts.addTokens (s, ",;", "");

    for (int ti = 0; ti < parts.size(); ++ti)
    {
        const int v = parts[ti].getIntValue();
        if (v < 0 || v >= static_cast<int> (ModuleId::numModules))
            return defaultPanelOrder();
        out.add (static_cast<ModuleId> (v));
    }

    if (out.size() != static_cast<int> (ModuleId::numModules))
        return defaultPanelOrder();

    juce::Array<ModuleId> seen;
    for (auto id : out)
    {
        for (auto e : seen)
            if (e == id)
                return defaultPanelOrder();
        seen.add (id);
    }

    return out;
}

juce::String PanelLauncherStrip::orderToString (const juce::Array<ModuleId>& order)
{
    juce::String s;
    for (int i = 0; i < order.size(); ++i)
    {
        if (i > 0)
            s << ",";
        s << static_cast<int> (order.getReference (i));
    }
    return s;
}

PanelLauncherStrip::PanelLauncherStrip()
{
    for (int i = 0; i < static_cast<int> (ModuleId::numModules); ++i)
    {
        const auto id = static_cast<ModuleId> (i);
        chips[(size_t) i] = std::make_unique<ModuleChip> (*this, id);
        addAndMakeVisible (*chips[(size_t) i]);
    }

    panelOrder = defaultPanelOrder();
    layoutChips();
}

void PanelLauncherStrip::setPanelOrder (const juce::Array<ModuleId>& order)
{
    if (order.size() != static_cast<int> (ModuleId::numModules))
        return;

    juce::Array<ModuleId> seen;
    for (auto id : order)
    {
        const int ii = static_cast<int> (id);
        if (ii < 0 || ii >= static_cast<int> (ModuleId::numModules))
            return;
        for (auto e : seen)
            if (e == id)
                return;
        seen.add (id);
    }

    panelOrder = order;
    layoutChips();
    repaint();
}

void PanelLauncherStrip::resized()
{
    layoutChips();
}

void PanelLauncherStrip::paint (juce::Graphics& g)
{
    auto r = getLocalBounds().toFloat();
    g.setColour (GroovDeckLookAndFeel::surface().withAlpha (0.92f));
    g.fillRoundedRectangle (r, 6.0f);
    g.setColour (GroovDeckLookAndFeel::border());
    g.drawRoundedRectangle (r, 6.0f, 1.0f);

    g.setColour (GroovDeckLookAndFeel::textMuted());
    g.setFont (juce::Font (juce::FontOptions (9.0f)));
    g.drawText ("Drag chips to reorder layout · Double-click brings panel to front",
                getLocalBounds().removeFromTop (14).reduced (8, 0),
                juce::Justification::centredLeft,
                true);
}

void PanelLauncherStrip::layoutChips()
{
    auto r = getLocalBounds().reduced (6, 4).withTrimmedTop (14);
    const int n = panelOrder.size();
    if (n <= 0)
        return;

    const int gap = 4;
    const int chipW = juce::jmax (44, (r.getWidth() - (n - 1) * gap) / n);
    int x = r.getX();

    for (auto id : panelOrder)
    {
        const int idx = moduleIdToIndex (id);
        if (chips[(size_t) idx] != nullptr)
        {
            chips[(size_t) idx]->setBounds (x, r.getY(), chipW, r.getHeight());
            x += chipW + gap;
        }
    }
}

ModuleChip* PanelLauncherStrip::findChipAt (juce::Point<int> localPoint) const
{
    for (auto& c : chips)
        if (c != nullptr && c->getBounds().contains (localPoint))
            return c.get();

    return nullptr;
}

bool PanelLauncherStrip::isInterestedInDragSource (const SourceDetails& dragSourceDetails)
{
    return dragSourceDetails.description.toString().startsWith (dragIdPrefix);
}

void PanelLauncherStrip::itemDragEnter (const SourceDetails& dragSourceDetails)
{
    juce::ignoreUnused (dragSourceDetails);
}

void PanelLauncherStrip::itemDragMove (const SourceDetails& dragSourceDetails)
{
    if (! isInterestedInDragSource (dragSourceDetails))
        return;

    auto* ch = findChipAt (dragSourceDetails.localPosition);
    if (ch != hoverTargetChip)
    {
        hoverTargetChip = ch;
        repaint();
    }
}

void PanelLauncherStrip::itemDragExit (const SourceDetails& dragSourceDetails)
{
    juce::ignoreUnused (dragSourceDetails);
    hoverTargetChip = nullptr;
    repaint();
}

void PanelLauncherStrip::itemDropped (const SourceDetails& dragSourceDetails)
{
    auto* srcChip = dynamic_cast<ModuleChip*> (dragSourceDetails.sourceComponent.get());
    auto* dstChip = findChipAt (dragSourceDetails.localPosition);

    hoverTargetChip = nullptr;
    repaint();

    if (srcChip == nullptr || dstChip == nullptr || srcChip == dstChip)
        return;

    const ModuleId a = srcChip->getModuleId();
    const ModuleId b = dstChip->getModuleId();
    const int ia = panelOrder.indexOf (a);
    const int ib = panelOrder.indexOf (b);

    if (ia < 0 || ib < 0)
        return;

    panelOrder.swap (ia, ib);
    layoutChips();

    if (onOrderChanged)
        onOrderChanged (panelOrder);
}
