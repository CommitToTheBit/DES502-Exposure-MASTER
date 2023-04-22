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
		Journal.Entries[Entry.Key].RenderTarget->InitCustomFormat(1024, 1024, PF_B8G8R8A8, false);
	}*/
}

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */

void UDES_JournalManager::ReadJournalProgress(UDESSaveGame* GameData)
{
	int index = 0;
	int maxActive = 0;
	for (TPair<FString, FDES_JournalEntryStruct>& Entry : Journal.Entries)
	{
		Journal.Entries[Entry.Key].EntryActive = GameData->EntriesActive[index];

		if (Journal.Entries[Entry.Key].EntryActive >= 0)
		{
			/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
			/* This enclosed section has been adapted from: Kazimieras Mikelis' Game Blog (2020) Saving Screenshots & Byte Data in Unreal Engine. Available at: https://mikelis.net/saving-screenshots-byte-data-in-unreal-engine/ (Accessed: 2 April 2023) */

			// STEP 1: Set up texture...
			UTexture2D* Texture = UTexture2D::CreateTransient(1024, 1024, PF_B8G8R8A8);
			// Get a reference to MIP 0, for convenience.
			FTexture2DMipMap& Mip = Texture->PlatformData->Mips[0];

			// STEP 2: Copy data...
			void* MipBulkData = Mip.BulkData.Lock(LOCK_READ_WRITE);
			Mip.BulkData.Realloc(4 * 1024 * 1024);
			FMemory::Memcpy(MipBulkData, GameData->BinaryTextures.GetData() + index * 4 * 1024 * 1024, 4 * 1024 * 1024);
			Mip.BulkData.Unlock();

			// STEP 3: Update resources...
			Texture->UpdateResource();
			Journal.Entries[Entry.Key].Texture = Texture;

			// FIXME: Could these settings solve our 'stair step' effect?
			/*Journal.Entries[Entry.Key].Texture->MipGenSettings = TextureMipGenSettings::TMGS_FromTextureGroup;
			Journal.Entries[Entry.Key].Texture->LODBias = 0;
			Journal.Entries[Entry.Key].Texture->LODGroup = TextureGroup::TEXTUREGROUP_Pixels2D;//TEXTUREGROUP_UI;
			Journal.Entries[Entry.Key].Texture->NeverStream = true;
			Journal.Entries[Entry.Key].Texture->Filter = TextureFilter::TF_Trilinear;//TF_Nearest;
			Journal.Entries[Entry.Key].Texture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;*/

			/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
		}

		Journal.Entries[Entry.Key].RenderTargetActive = Journal.Entries[Entry.Key].EntryActive < 0;

		Journal.Entries[Entry.Key].RenderTarget = NewObject<UTextureRenderTarget2D>(UTextureRenderTarget2D::StaticClass());
		Journal.Entries[Entry.Key].RenderTarget->InitCustomFormat(1024, 1024, PF_B8G8R8A8, false);

		if (Journal.Entries[Entry.Key].EntryActive >= maxActive)
			maxActive = Journal.Entries[Entry.Key].EntryActive + 1;

		index++;
	}

	// NB: Ordering entries for UI...
	Chronology.Empty();
	for (int i = 0; i < maxActive; i++)
	{
		for (TPair<FString, FDES_JournalEntryStruct>& Entry : Journal.Entries)
		{
			if (Entry.Value.EntryActive == i)
			{
				Chronology.Add(Entry.Key);
				continue;
			}
		}
	}
}

void UDES_JournalManager::WriteJournalProgress(UDESSaveGame* GameData, bool resetJournalData)
{
	// NB: If narrative meanigfully alters the JSON, journal progress will reset.
	// If the total number of entries remains the same, narrative should be able to reset the save file manually without a crash.
	// This error handling should be robust; the main issue in a finished product would be pushing updates to existing players...
	resetJournalData |= GameData->EntriesActive.Num() != Journal.Entries.Num(); 

	if (resetJournalData)
	{
		GameData->EntriesActive.Reserve(Journal.Entries.Num());
		GameData->EntriesActive.Empty();

		// NB: Populating with the exact number of entries...
		if (GameData->EntriesActive.Num() < Journal.Entries.Num())
			GameData->EntriesActive.AddUninitialized(Journal.Entries.Num() - GameData->EntriesActive.Num());
		while (GameData->EntriesActive.Num() > Journal.Entries.Num())
			GameData->EntriesActive.RemoveAt(GameData->EntriesActive.Num() - 1);

		for (int i = 0; i < Journal.Entries.Num(); i++)
			GameData->EntriesActive[i] = -1;

		GameData->BinaryTextures.Reserve(Journal.Entries.Num() * 4 * 1024 * 1024); // NB: 4 values, RGBA, for each pixel of the polaroid...
		GameData->BinaryTextures.Empty();

		// NB: Populating with the exact number of entries...
		if (GameData->BinaryTextures.Num() < Journal.Entries.Num() * 4 * 1024 * 1024)
			GameData->BinaryTextures.AddUninitialized(Journal.Entries.Num() * 4 * 1024 * 1024 - GameData->BinaryTextures.Num());
		while (GameData->BinaryTextures.Num() > Journal.Entries.Num() * 4 * 1024 * 1024)
			GameData->EntriesActive.RemoveAt(GameData->EntriesActive.Num() - 1);
	}
	else
	{
		int index = 0;
		for (TPair<FString, FDES_JournalEntryStruct>& Entry : Journal.Entries)
		{
			// NB: Updating only the entries that have actually changed...
			if (GameData->EntriesActive[index] < 0 && Entry.Value.EntryActive >= 0)
			{
				GameData->EntriesActive[index] = Entry.Value.EntryActive;

				/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
				/* This enclosed section has been adapted from: Kazimieras Mikelis' Game Blog (2020) Saving Screenshots & Byte Data in Unreal Engine. Available at: https://mikelis.net/saving-screenshots-byte-data-in-unreal-engine/ (Accessed: 2 April 2023) */

				// STEP 1: Read RenderTarget's data into an FColor array...
				TArray<FColor> ColorArray;
				ColorArray.Reserve(1024 * 1024);
				Journal.Entries[Entry.Key].RenderTarget->GameThread_GetRenderTargetResource()->ReadPixels(ColorArray);
				ColorArray.Shrink();

				// STEP 2: Copy FColor array into binary texture...
				FMemory::Memcpy(GameData->BinaryTextures.GetData() + index * 4 * 1024 * 1024, ColorArray.GetData(), 4 * 1024 * 1024);

				/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
			}

			index++;
		}
	}
}