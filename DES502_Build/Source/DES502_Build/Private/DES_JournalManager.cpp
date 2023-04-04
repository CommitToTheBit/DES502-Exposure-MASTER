#include "DES_JournalManager.h"

void UDES_JournalManager::InitialiseEntries(FString FilePath)
{
	FDES_JournalStruct TempJournal = FDES_JournalStruct();

	ReadJournalStructFromFile(FilePath);
	//Journal = ReadJournalStructFromFile(FilePath);

	/*for (TPair<FString, FDES_JournalEntryStruct>& Entry : Journal.Entries)
	{
		//Entry.Value.RenderTarget = NewObject<UTextureRenderTarget2D>(UTextureRenderTarget2D::StaticClass());
		//Entry.Value.RenderTarget->InitCustomFormat(900, 1080, PF_B8G8R8A8, false);
	}*/

	// DEBUG:
	//GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, FString::FromInt(JsonStructs.Num()));
}

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
/* This enclosed section has been adapted from: Alex Quevillon (2022) UE5 C++ 2 - How To Read and Write Json Files with C++? - Unreal Engine Tutorial Editor CPP. Available at: https://www.youtube.com/watch?v=4wJ45mrzrkM&t=339s (Accessed: 4 April 2023) */

FDES_JournalStruct UDES_JournalManager::ReadJournalStructFromFile(FString FilePath)
{
	// STEP 1: Check if the file exists...
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
		return FDES_JournalStruct();

	// STEP 2: Read the JSON into a string, if possible...
	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
		return FDES_JournalStruct();

	// STEP 3: Deserialise string into a JSON object, if possible...
	TSharedPtr<FJsonObject> JsonObject;// = MakeShareable(new FJsonObject());
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), JsonObject))
		return FDES_JournalStruct();

	// STEP 4: Convert JSON object into desired structures, if possible...
	FDES_JournalStruct JournalStruct;
	if (!FJsonObjectConverter::JsonObjectToUStruct<FDES_JournalStruct>(JsonObject.ToSharedRef(), &JournalStruct)) // FIXME: Is this actually catching any errors?
		return FDES_JournalStruct();

	// DEBUG:
	GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, JournalStruct.Entries["Goblet"].Title);

	return JournalStruct;
}

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
