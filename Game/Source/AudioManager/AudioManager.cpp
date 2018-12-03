#include "GamePCH.h"
#include <fstream>
#include "AudioManager.h"

std::vector<WaveData*>* AudioManager::LoadedWaveData = nullptr;
std::vector<Audio*>* AudioManager::LoadedSounds = nullptr;
AudioEngine* AudioManager::Engine = nullptr;

void AudioManager::Initialize()
{
	if (Engine == nullptr)
		Engine = new AudioEngine();

	if (LoadedWaveData == nullptr)
		LoadedWaveData = new std::vector<WaveData*>;

	if (LoadedSounds == nullptr)
		LoadedSounds = new std::vector<Audio*>;

}

void AudioManager::Reserve(int value /*= 10*/)
{
	LoadedWaveData->reserve(value);
	LoadedSounds->reserve(value);
}

void AudioManager::Release()
{
	for (unsigned int i = 0; i < LoadedSounds->size(); i++)
	{
		LoadedSounds->at(i)->Stop();
		delete LoadedSounds->at(i);
	}

	delete LoadedSounds;
	LoadedSounds = nullptr;

	for (unsigned int i = 0; i < LoadedWaveData->size(); i++)
		delete LoadedWaveData->at(i);

	delete LoadedWaveData;
	LoadedWaveData = nullptr;

	delete Engine;
	Engine = nullptr;
}

bool AudioManager::LoadFromPath(const char* path)
{

	//filename pathing
	std::string aPath = "Data/Audio/" + (std::string)path + ".wav";

	//Open the input file
	std::ifstream inFile(aPath.c_str(), std::ios::binary | std::ios::in);
	if (inFile.bad())
	{
		return false;
	}

	unsigned long dwChunkId = 0;
	unsigned long dwFileSize = 0;
	unsigned long dwChunkSize = 0;
	unsigned long dwExtra = 0;

	//look for 'RIFF' chunk identifier
	inFile.seekg(0, std::ios::beg);
	inFile.read(reinterpret_cast<char*>(&dwChunkId), sizeof(dwChunkId));
	if (dwChunkId != 'FFIR')
	{
		inFile.close();
		return false;
	}
	inFile.seekg(4, std::ios::beg); //get file size
	inFile.read(reinterpret_cast<char*>(&dwFileSize), sizeof(dwFileSize));
	if (dwFileSize <= 16)
	{
		inFile.close();
		return false;
	}
	inFile.seekg(8, std::ios::beg); //get file format
	inFile.read(reinterpret_cast<char*>(&dwExtra), sizeof(dwExtra));
	if (dwExtra != 'EVAW')
	{
		inFile.close();
		return false;
	}

	//Create the WaveData object
	WaveData* waveData = new WaveData();
	waveData->name = path;

	//look for 'fmt ' chunk id
	bool bFilledFormat = false;
	for (unsigned int i = 12; i < dwFileSize; )
	{
		inFile.seekg(i, std::ios::beg);
		inFile.read(reinterpret_cast<char*>(&dwChunkId), sizeof(dwChunkId));
		inFile.seekg(i + 4, std::ios::beg);
		inFile.read(reinterpret_cast<char*>(&dwChunkSize), sizeof(dwChunkSize));
		if (dwChunkId == ' tmf')
		{
			inFile.seekg(i + 8, std::ios::beg);
			inFile.read(reinterpret_cast<char*>(&waveData->waveFormat), sizeof(waveData->waveFormat));
			bFilledFormat = true;
			break;
		}
		dwChunkSize += 8; //add offsets of the chunk id, and chunk size data entries
		dwChunkSize += 1;
		dwChunkSize &= 0xfffffffe; //guarantees WORD padding alignment
		i += dwChunkSize;
	}

	if (bFilledFormat == false)
	{
		delete waveData;
		inFile.close();
		return false;
	}

	//look for 'data' chunk id
	bool bFilledData = false;
	for (unsigned int i = 12; i < dwFileSize; )
	{
		inFile.seekg(i, std::ios::beg);
		inFile.read(reinterpret_cast<char*>(&dwChunkId), sizeof(dwChunkId));
		inFile.seekg(i + 4, std::ios::beg);
		inFile.read(reinterpret_cast<char*>(&dwChunkSize), sizeof(dwChunkSize));
		if (dwChunkId == 'atad')
		{
			waveData->data = new unsigned char[dwChunkSize];
			inFile.seekg(i + 8, std::ios::beg);
			inFile.read(reinterpret_cast<char*>(waveData->data), dwChunkSize);
			waveData->buffer.AudioBytes = dwChunkSize;
			waveData->buffer.pAudioData = (BYTE*)waveData->data;
			waveData->buffer.PlayBegin = 0;
			waveData->buffer.PlayLength = 0;
			bFilledData = true;
			break;
		}
		dwChunkSize += 8; //add offsets of the chunk id, and chunk size data entries
		dwChunkSize += 1;
		dwChunkSize &= 0xfffffffe; //guarantees WORD padding alignment
		i += dwChunkSize;
	}
	if (!bFilledData)
	{
		delete waveData;
		inFile.close();
		return false;
	}


	//Close the input file
	inFile.close();

	LoadedWaveData->push_back(waveData);

	//Return true
	return true;
}

Audio* AudioManager::CreateAudio(const char ** name)
{
	LoadedSounds->push_back(new Audio(name));
	return GetAudio(name);
}

Audio * AudioManager::GetAudio(const char ** name)
{
	for (unsigned int i = 0; i < LoadedSounds->size(); i++)
	{
		if (*name == *LoadedSounds->at(i)->GetName())
			return LoadedSounds->at(i);
	}

	//This should never get here
	assert(false);
	return new Audio(&AUDIO_NAMES::FLOOR_1);
}

WaveData* AudioManager::GetWaveData(const char* path)
{
	for (unsigned int i = 0; i < LoadedWaveData->size(); i++)
		if (LoadedWaveData->at(i)->name == path)
			return LoadedWaveData->at(i);

	//This should never get here
	assert(false);
	return new WaveData();
}
