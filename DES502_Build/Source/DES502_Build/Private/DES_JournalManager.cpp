#include "DES_JournalManager.h"

void UDES_JournalManager::InitialiseEntries(FString FilePath)
{
	// DEBUG:
	GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, TEXT("Journal manager running..."));

	ReadJSONFromFile(FilePath);
}

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
/* This enclosed section has been adapted from: Alex Quevillon (2022) UE5 C++ 2 - How To Read and Write Json Files with C++? - Unreal Engine Tutorial Editor CPP. Available at: https://www.youtube.com/watch?v=4wJ45mrzrkM&t=339s (Accessed: 4 April 2023) */

TSharedPtr<FJsonObject> UDES_JournalManager::ReadJSONFromFile(FString FilePath)
{
	// STEP 1: Check if the file exists...
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
		return nullptr;

	// STEP 2: Read the JSON into a string, if possible...
	FString jsonString = "";
	if (!FFileHelper::LoadFileToString(jsonString, *FilePath))
		return nullptr;

	// DEBUG:
	GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, jsonString);

	return nullptr;
}

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
