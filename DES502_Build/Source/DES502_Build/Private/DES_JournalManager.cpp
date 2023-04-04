#include "DES_JournalManager.h"

void UDES_JournalManager::InitialiseEntries(FString FilePath)
{
	TArray<FDES_JournalEntryStruct> JsonStructs = ReadJsonStructsFromFile(FilePath);

	// DEBUG:
	//GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, FString::FromInt(JsonStructs.Num()));
}

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
/* This enclosed section has been adapted from: Alex Quevillon (2022) UE5 C++ 2 - How To Read and Write Json Files with C++? - Unreal Engine Tutorial Editor CPP. Available at: https://www.youtube.com/watch?v=4wJ45mrzrkM&t=339s (Accessed: 4 April 2023) */

TArray<FDES_JournalEntryStruct> UDES_JournalManager::ReadJsonStructsFromFile(FString FilePath)
{
	// STEP 1: Check if the file exists...
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
		return TArray<FDES_JournalEntryStruct>();

	// STEP 2: Read the JSON into a string, if possible...
	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
		return TArray<FDES_JournalEntryStruct>();

	// STEP 3: Deserialise string into a JSON object, if possible...
	TSharedPtr<FJsonObject> JsonObject;// = MakeShareable(new FJsonObject());
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), JsonObject))
		return TArray<FDES_JournalEntryStruct>();

	// STEP 4: Convert JSON object into desired structures, if possible...
	FDES_JournalStruct JsonStruct;
	if (!FJsonObjectConverter::JsonObjectToUStruct<FDES_JournalStruct>(JsonObject.ToSharedRef(), &JsonStruct)) // FIXME: Is this actually catching any errors?
		return TArray<FDES_JournalEntryStruct>();

	// DEBUG:
	GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, JsonStruct.JournalEntries[0].Title);

	return TArray<FDES_JournalEntryStruct>();
}

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
