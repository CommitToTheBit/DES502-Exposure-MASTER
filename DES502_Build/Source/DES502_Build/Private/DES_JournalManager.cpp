#include "DES_JournalManager.h"

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
/* This enclosed section has been adapted from: Alex Quevillon (2022) UE5 C++ 2 - How To Read and Write Json Files with C++? - Unreal Engine Tutorial Editor CPP. Available at: https://www.youtube.com/watch?v=4wJ45mrzrkM&t=339s (Accessed: 4 April 2023) */

void UDES_JournalManager::ReadJournalData(FString FilePath)
{
	// STEP 1: Check if the file exists...
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
		return;

	// STEP 2: Read the JSON into a string, if possible...
	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
		return;

	// STEP 3: Deserialise string into a JSON object, if possible...
	TSharedPtr<FJsonObject> JsonObject;// = MakeShareable(new FJsonObject());
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), JsonObject))
		return;

	// STEP 4: Convert JSON object into desired structure, if possible...
	FJsonObjectConverter::JsonObjectToUStruct<FDES_JournalStruct>(JsonObject.ToSharedRef(), &Journal);
}

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */

void UDES_JournalManager::ReadJournalProgress()
{
	for (TPair<FString, FDES_JournalEntryStruct>& Entry : Journal.Entries)
	{
		Entry.Value.RenderTarget = NewObject<UTextureRenderTarget2D>(UTextureRenderTarget2D::StaticClass());
		Entry.Value.RenderTarget->InitCustomFormat(900, 1080, PF_B8G8R8A8, false);
	}
}

void UDES_JournalManager::WriteJournalProgress()
{

}