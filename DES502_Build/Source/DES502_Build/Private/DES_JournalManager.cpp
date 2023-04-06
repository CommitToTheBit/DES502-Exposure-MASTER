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

	// STEP 5: [SAFETY]
	/*for (TPair<FString, FDES_JournalEntryStruct>& Entry : Journal.Entries)
	{
		Journal.Entries[Entry.Key].RenderTarget = NewObject<UTextureRenderTarget2D>(UTextureRenderTarget2D::StaticClass());
		Journal.Entries[Entry.Key].RenderTarget->InitCustomFormat(900, 1080, PF_B8G8R8A8, false);
	}*/
}

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */

void UDES_JournalManager::ReadJournalProgress(UDESSaveGame* GameData)
{
	int index = 0;
	for (TPair<FString, FDES_JournalEntryStruct>& Entry : Journal.Entries)
	{
		Journal.Entries[Entry.Key].EntryActive = GameData->EntriesActive[index];
		if (Journal.Entries[Entry.Key].EntryActive)
		{
			/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
			/* This enclosed section has been adapted from: Kazimieras Mikelis' Game Blog (2020) Saving Screenshots & Byte Data in Unreal Engine. Available at: https://mikelis.net/saving-screenshots-byte-data-in-unreal-engine/ (Accessed: 2 April 2023) */

			// STEP 1: Set up texture...
			UTexture2D* Texture = UTexture2D::CreateTransient(900, 1080, PF_B8G8R8A8);
			// Get a reference to MIP 0, for convenience.
			FTexture2DMipMap& Mip = Texture->PlatformData->Mips[0];

			// STEP 2: Copy data...
			void* MipBulkData = Mip.BulkData.Lock(LOCK_READ_WRITE);
			Mip.BulkData.Realloc(4 * 900 * 1080);
			FMemory::Memcpy(MipBulkData, GameData->BinaryTextures.GetData() + index * 4 * 900 * 1080, 4 * 900 * 1080);
			Mip.BulkData.Unlock();

			// STEP 3: Update resources...
			Texture->UpdateResource();
			Journal.Entries[Entry.Key].Texture = Texture;

			/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
		}

		Journal.Entries[Entry.Key].RenderTargetActive = !Journal.Entries[Entry.Key].EntryActive;
		//{
		Journal.Entries[Entry.Key].RenderTarget = NewObject<UTextureRenderTarget2D>(UTextureRenderTarget2D::StaticClass());
		Journal.Entries[Entry.Key].RenderTarget->InitCustomFormat(900, 1080, PF_B8G8R8A8, false);
		//}

		index++;
	}
}

void UDES_JournalManager::WriteJournalProgress(UDESSaveGame* GameData)
{
	// NB: 'Re-saving' everything, every time, might not be efficient, but useful for JSON changes...?
	GameData->EntriesActive.Reserve(Journal.Entries.Num());
	GameData->EntriesActive.Empty();
	if (GameData->EntriesActive.Num() < Journal.Entries.Num())
		GameData->EntriesActive.AddUninitialized(Journal.Entries.Num() - GameData->EntriesActive.Num());

	GameData->BinaryTextures.Reserve(Journal.Entries.Num() * 4 * 900 * 1080); // NB: 4 values, RGBA, for each pixel of the polaroid...
	GameData->BinaryTextures.Empty();
	if (GameData->BinaryTextures.Num() < Journal.Entries.Num() * 4 * 900 * 1080)
		GameData->BinaryTextures.AddUninitialized(Journal.Entries.Num() * 4 * 900 * 1080 - GameData->BinaryTextures.Num());

	int index = 0;
	for (TPair<FString, FDES_JournalEntryStruct>& Entry : Journal.Entries)
	{
		// NB: This would prevent issues with re-saving...
		//if (Entry.Value.EntryActive)
		//	continue;

		GameData->EntriesActive[index] = Entry.Value.EntryActive;

		//for (int i = 0; i < 4 * 900 * 1080; i++)
		//	GameData->BinaryTextures[index * 4 * 900 * 1080 + i] = 0;



		//if (!GameData->EntriesActive.Contains(Entry.Key))
		//	GameData->EntriesActive.Add(Entry.Key, Journal.Entries[Entry.Key].EntryActive);
		//else
		//	GameData->EntriesActive[Entry.Key] = Journal.Entries[Entry.Key].EntryActive;

		//if (Journal.Entries[Entry.Key].EntryActive&& Journal.Entries[Entry.Key].RenderTargetActive)
		//{
			/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
			/* This enclosed section has been adapted from: Kazimieras Mikelis' Game Blog (2020) Saving Screenshots & Byte Data in Unreal Engine. Available at: https://mikelis.net/saving-screenshots-byte-data-in-unreal-engine/ (Accessed: 2 April 2023) */

			// STEP 1: Read RenderTarget's data into an FColor array...
			TArray<FColor> ColorArray;
			ColorArray.Reserve(900 * 1080);
			Journal.Entries[Entry.Key].RenderTarget->GameThread_GetRenderTargetResource()->ReadPixels(ColorArray);
			ColorArray.Shrink();

			// STEP 2: Copy FColor array into binary texture...
			/*if (!GameData->BinaryTextures.Contains(Entry.Key))
				GameData->BinaryTextures.Add(Entry.Key, TArray<uint8>());
			GameData->BinaryTexture.Reserve(4 * 900 * 1080);

			GameData->BinaryTexture.Empty();
			if (GameData->BinaryTexture.Num() < 4 * 900 * 1080)
				GameData->BinaryTexture.AddUninitialized(4 * 900 * 1080 - GameData->BinaryTexture.Num());*/ //

			FMemory::Memcpy(GameData->BinaryTextures.GetData() + index * 4 * 900 * 1080, ColorArray.GetData(), 4 * 900 * 1080);

			/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
		//}

		index++;
	}
}