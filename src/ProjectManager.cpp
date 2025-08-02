#include "ProjectManager.h"

ProjectManager::ProjectManager()
    : autoSaveEnabled(false), autoSaveInterval(5)
{
}

ProjectManager::~ProjectManager()
{
    if (autoSaveEnabled)
    {
        performAutoSave();
    }
}

bool ProjectManager::createProject(const juce::String& name, const juce::File& directory)
{
    if (!directory.exists())
    {
        directory.createDirectory();
    }
    
    currentProject = std::make_unique<ProjectData>();
    currentProject->projectName = name;
    currentProject->projectPath = directory.getFullPathName();
    
    projectFile = directory.getChildFile(name + ".groovdeck");
    return true;
}

bool ProjectManager::saveProject(const juce::File& file)
{
    if (!currentProject)
        return false;
    
    return saveToFile(file, *currentProject);
}

bool ProjectManager::loadProject(const juce::File& file)
{
    if (!file.existsAsFile())
        return false;
    
    auto newProject = std::make_unique<ProjectData>();
    if (loadFromFile(file, *newProject))
    {
        currentProject = std::move(newProject);
        projectFile = file;
        return true;
    }
    return false;
}

bool ProjectManager::closeProject()
{
    if (autoSaveEnabled)
    {
        performAutoSave();
    }
    
    currentProject.reset();
    projectFile = juce::File();
    return true;
}

juce::String ProjectManager::getProjectName() const
{
    if (currentProject)
        return currentProject->projectName;
    return "";
}

juce::File ProjectManager::getProjectFile() const
{
    return projectFile;
}

void ProjectManager::exportProjectData(ProjectData& data)
{
    if (currentProject)
    {
        data = *currentProject;
    }
}

void ProjectManager::importProjectData(const ProjectData& data)
{
    currentProject = std::make_unique<ProjectData>(data);
}

void ProjectManager::enableAutoSave(bool enable)
{
    autoSaveEnabled = enable;
    if (enable)
    {
        autoSaveTimer.startTimer(autoSaveInterval * 60 * 1000); // Convert to milliseconds
    }
    else
    {
        autoSaveTimer.stopTimer();
    }
}

void ProjectManager::setAutoSaveInterval(int minutes)
{
    autoSaveInterval = minutes;
    if (autoSaveEnabled)
    {
        autoSaveTimer.startTimer(autoSaveInterval * 60 * 1000);
    }
}

void ProjectManager::performAutoSave()
{
    if (currentProject && !projectFile.getFullPathName().isEmpty())
    {
        saveToFile(projectFile, *currentProject);
    }
}

void ProjectManager::saveToFile(const juce::File& file, const ProjectData& data)
{
    juce::var projectVar = projectDataToVar(data);
    juce::FileOutputStream stream(file);
    if (stream.openedOk())
    {
        stream.setPosition(0);
        stream.truncate();
        stream.writeText(projectVar.toString(), false, false, "\n");
    }
}

bool ProjectManager::loadFromFile(const juce::File& file, ProjectData& data)
{
    juce::String jsonString = file.loadFileAsString();
    juce::var projectVar = juce::JSON::parse(jsonString);
    
    if (projectVar.isObject())
    {
        return varToProjectData(projectVar, data);
    }
    return false;
}

juce::var ProjectManager::projectDataToVar(const ProjectData& data)
{
    juce::DynamicObject::Ptr obj = new juce::DynamicObject();
    
    obj->setProperty("projectName", data.projectName);
    obj->setProperty("projectPath", data.projectPath);
    obj->setProperty("tempo", data.tempo);
    obj->setProperty("masterGain", data.masterGain);
    
    // Audio files
    juce::var audioFilesArray;
    for (const auto& file : data.audioFiles)
    {
        audioFilesArray.append(file);
    }
    obj->setProperty("audioFiles", audioFilesArray);
    
    // Effects settings
    juce::DynamicObject::Ptr effectsObj = new juce::DynamicObject();
    effectsObj->setProperty("reverbEnabled", data.effects.reverbEnabled);
    effectsObj->setProperty("reverbRoomSize", data.effects.reverbRoomSize);
    effectsObj->setProperty("reverbDamping", data.effects.reverbDamping);
    effectsObj->setProperty("reverbWetLevel", data.effects.reverbWetLevel);
    effectsObj->setProperty("reverbDryLevel", data.effects.reverbDryLevel);
    
    effectsObj->setProperty("delayEnabled", data.effects.delayEnabled);
    effectsObj->setProperty("delayTime", data.effects.delayTime);
    effectsObj->setProperty("delayFeedback", data.effects.delayFeedback);
    effectsObj->setProperty("delayMix", data.effects.delayMix);
    
    effectsObj->setProperty("filterEnabled", data.effects.filterEnabled);
    effectsObj->setProperty("filterCutoff", data.effects.filterCutoff);
    effectsObj->setProperty("filterResonance", data.effects.filterResonance);
    
    effectsObj->setProperty("distortionEnabled", data.effects.distortionEnabled);
    effectsObj->setProperty("distortionDrive", data.effects.distortionDrive);
    effectsObj->setProperty("distortionMix", data.effects.distortionMix);
    
    obj->setProperty("effects", effectsObj);
    
    // Sequencer data
    juce::DynamicObject::Ptr sequencerObj = new juce::DynamicObject();
    sequencerObj->setProperty("numSteps", data.sequencer.numSteps);
    sequencerObj->setProperty("tempo", data.sequencer.tempo);
    
    juce::var stepStatesArray;
    for (bool state : data.sequencer.stepStates)
    {
        stepStatesArray.append(state);
    }
    sequencerObj->setProperty("stepStates", stepStatesArray);
    
    juce::var stepVelocitiesArray;
    for (float velocity : data.sequencer.stepVelocities)
    {
        stepVelocitiesArray.append(velocity);
    }
    sequencerObj->setProperty("stepVelocities", stepVelocitiesArray);
    
    obj->setProperty("sequencer", sequencerObj);
    
    // Looper data
    juce::DynamicObject::Ptr looperObj = new juce::DynamicObject();
    looperObj->setProperty("loopLength", data.looper.loopLength);
    looperObj->setProperty("loopGain", data.looper.loopGain);
    looperObj->setProperty("loopStart", data.looper.loopStart);
    looperObj->setProperty("loopEnd", data.looper.loopEnd);
    looperObj->setProperty("hasLoop", data.looper.hasLoop);
    
    obj->setProperty("looper", looperObj);
    
    // Slicer data
    juce::DynamicObject::Ptr slicerObj = new juce::DynamicObject();
    slicerObj->setProperty("sampleFile", data.slicer.sampleFile);
    
    juce::var sliceNamesArray;
    for (const auto& name : data.slicer.sliceNames)
    {
        sliceNamesArray.append(name);
    }
    slicerObj->setProperty("sliceNames", sliceNamesArray);
    
    juce::var sliceStartTimesArray;
    for (double time : data.slicer.sliceStartTimes)
    {
        sliceStartTimesArray.append(time);
    }
    slicerObj->setProperty("sliceStartTimes", sliceStartTimesArray);
    
    juce::var sliceEndTimesArray;
    for (double time : data.slicer.sliceEndTimes)
    {
        sliceEndTimesArray.append(time);
    }
    slicerObj->setProperty("sliceEndTimes", sliceEndTimesArray);
    
    obj->setProperty("slicer", slicerObj);
    
    return obj;
}

bool ProjectManager::varToProjectData(const juce::var& var, ProjectData& data)
{
    if (!var.isObject())
        return false;
    
    juce::DynamicObject::Ptr obj = var.getDynamicObject();
    if (!obj)
        return false;
    
    data.projectName = obj->getProperty("projectName").toString();
    data.projectPath = obj->getProperty("projectPath").toString();
    data.tempo = obj->getProperty("tempo");
    data.masterGain = obj->getProperty("masterGain");
    
    // Audio files
    juce::var audioFilesVar = obj->getProperty("audioFiles");
    if (audioFilesVar.isArray())
    {
        data.audioFiles.clear();
        for (int i = 0; i < audioFilesVar.size(); ++i)
        {
            data.audioFiles.add(audioFilesVar[i].toString());
        }
    }
    
    // Effects settings
    juce::var effectsVar = obj->getProperty("effects");
    if (effectsVar.isObject())
    {
        juce::DynamicObject::Ptr effectsObj = effectsVar.getDynamicObject();
        data.effects.reverbEnabled = effectsObj->getProperty("reverbEnabled");
        data.effects.reverbRoomSize = effectsObj->getProperty("reverbRoomSize");
        data.effects.reverbDamping = effectsObj->getProperty("reverbDamping");
        data.effects.reverbWetLevel = effectsObj->getProperty("reverbWetLevel");
        data.effects.reverbDryLevel = effectsObj->getProperty("reverbDryLevel");
        
        data.effects.delayEnabled = effectsObj->getProperty("delayEnabled");
        data.effects.delayTime = effectsObj->getProperty("delayTime");
        data.effects.delayFeedback = effectsObj->getProperty("delayFeedback");
        data.effects.delayMix = effectsObj->getProperty("delayMix");
        
        data.effects.filterEnabled = effectsObj->getProperty("filterEnabled");
        data.effects.filterCutoff = effectsObj->getProperty("filterCutoff");
        data.effects.filterResonance = effectsObj->getProperty("filterResonance");
        
        data.effects.distortionEnabled = effectsObj->getProperty("distortionEnabled");
        data.effects.distortionDrive = effectsObj->getProperty("distortionDrive");
        data.effects.distortionMix = effectsObj->getProperty("distortionMix");
    }
    
    // Sequencer data
    juce::var sequencerVar = obj->getProperty("sequencer");
    if (sequencerVar.isObject())
    {
        juce::DynamicObject::Ptr sequencerObj = sequencerVar.getDynamicObject();
        data.sequencer.numSteps = sequencerObj->getProperty("numSteps");
        data.sequencer.tempo = sequencerObj->getProperty("tempo");
        
        juce::var stepStatesVar = sequencerObj->getProperty("stepStates");
        if (stepStatesVar.isArray())
        {
            data.sequencer.stepStates.clear();
            for (int i = 0; i < stepStatesVar.size(); ++i)
            {
                data.sequencer.stepStates.push_back(stepStatesVar[i]);
            }
        }
        
        juce::var stepVelocitiesVar = sequencerObj->getProperty("stepVelocities");
        if (stepVelocitiesVar.isArray())
        {
            data.sequencer.stepVelocities.clear();
            for (int i = 0; i < stepVelocitiesVar.size(); ++i)
            {
                data.sequencer.stepVelocities.push_back(stepVelocitiesVar[i]);
            }
        }
    }
    
    // Looper data
    juce::var looperVar = obj->getProperty("looper");
    if (looperVar.isObject())
    {
        juce::DynamicObject::Ptr looperObj = looperVar.getDynamicObject();
        data.looper.loopLength = looperObj->getProperty("loopLength");
        data.looper.loopGain = looperObj->getProperty("loopGain");
        data.looper.loopStart = looperObj->getProperty("loopStart");
        data.looper.loopEnd = looperObj->getProperty("loopEnd");
        data.looper.hasLoop = looperObj->getProperty("hasLoop");
    }
    
    // Slicer data
    juce::var slicerVar = obj->getProperty("slicer");
    if (slicerVar.isObject())
    {
        juce::DynamicObject::Ptr slicerObj = slicerVar.getDynamicObject();
        data.slicer.sampleFile = slicerObj->getProperty("sampleFile").toString();
        
        juce::var sliceNamesVar = slicerObj->getProperty("sliceNames");
        if (sliceNamesVar.isArray())
        {
            data.slicer.sliceNames.clear();
            for (int i = 0; i < sliceNamesVar.size(); ++i)
            {
                data.slicer.sliceNames.push_back(sliceNamesVar[i].toString());
            }
        }
        
        juce::var sliceStartTimesVar = slicerObj->getProperty("sliceStartTimes");
        if (sliceStartTimesVar.isArray())
        {
            data.slicer.sliceStartTimes.clear();
            for (int i = 0; i < sliceStartTimesVar.size(); ++i)
            {
                data.slicer.sliceStartTimes.push_back(sliceStartTimesVar[i]);
            }
        }
        
        juce::var sliceEndTimesVar = slicerObj->getProperty("sliceEndTimes");
        if (sliceEndTimesVar.isArray())
        {
            data.slicer.sliceEndTimes.clear();
            for (int i = 0; i < sliceEndTimesVar.size(); ++i)
            {
                data.slicer.sliceEndTimes.push_back(sliceEndTimesVar[i]);
            }
        }
    }
    
    return true;
} 