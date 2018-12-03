#pragma once

struct WaveData;
class AudioEngine;


//A class that provides conveniance methods to load PNG image data
class AudioManager
{
public:

	static void Initialize();
	static void Reserve(int value = 10);
	static void Release();

	static bool LoadFromPath(const char* path);

	static Audio* CreateAudio(const char** name);
	static Audio* GetAudio(const char** name);

	static WaveData* GetWaveData(const char* path);
	static AudioEngine* GetEngine() { return Engine; }

private:

	static std::vector<WaveData*>* LoadedWaveData;
	static std::vector<Audio*>* LoadedSounds;

	static AudioEngine* Engine;
};