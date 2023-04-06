#include "DESGameInstance.h"

void UDESGameInstance::Init()
{
	// BUGFIX: This is what makes the calls that set up widgets!
	Super::Init();

	// FIXME: Fold this into a full 'Update' function...
	DEBUG_JournalEntry = NewObject<UDES_JournalEntry>(UDES_JournalEntry::StaticClass());
	DEBUG_JournalEntry->RenderTarget = NewObject<UTextureRenderTarget2D>(UTextureRenderTarget2D::StaticClass());
	DEBUG_JournalEntry->RenderTarget->InitCustomFormat(900, 1080, PF_B8G8R8A8, false); // NB: This line, previously missing, is what solves the access violation!

	// Create journal manager...
	JournalManager = NewObject<UDES_JournalManager>(UDES_JournalManager::StaticClass());
	JournalManager->ReadJournalData("C:/Users/Sam/Desktop/JournalEntries.json");

	// Load any saved data...
	LoadGameData(); // DEBUG: Set to true to reset GameData on start...
	LoadSettingsData();
}

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */ 
/* This enclosed section has been adapted from: Awesome Tuts (no date) Saving And Loading Game Data With Blueprints And C++ In Unreal Engine. Available at: https://awesometuts.com/blog/save-load-game-data-unreal-engine/ (Accessed: 22 March 2023) */

void UDESGameInstance::SaveGameData()
{
	if (!GameData)
		return;

	// FIXME: A for loop over all of the journal's updated entries will go here...

	/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
	/* This enclosed section has been adapted from: Kazimieras Mikelis' Game Blog (2020) Saving Screenshots & Byte Data in Unreal Engine. Available at: https://mikelis.net/saving-screenshots-byte-data-in-unreal-engine/ (Accessed: 2 April 2023) */

	GameData->EntryActive = DEBUG_JournalEntry->EntryActive;

	if (DEBUG_JournalEntry->EntryActive && DEBUG_JournalEntry->RenderTargetActive)
	{	
		// STEP 1: Read RenderTarget's data into an FColor array...
		TArray<FColor> ColorArray;
		ColorArray.Reserve(900 * 1080);
		DEBUG_JournalEntry->RenderTarget->GameThread_GetRenderTargetResource()->ReadPixels(ColorArray);
		ColorArray.Shrink();

		// STEP 2: Copy FColor array into binary texture...
		GameData->BinaryTexture.Reserve(4 * 900 * 1080);

		GameData->BinaryTexture.Empty();
		if (GameData->BinaryTexture.Num() < 4 * 900 * 1080)
			GameData->BinaryTexture.AddUninitialized(4 * 900 * 1080 - GameData->BinaryTexture.Num()); // NB: 4 values, RGBA, for each pixel of the polaroid...

		FMemory::Memcpy(GameData->BinaryTexture.GetData(), ColorArray.GetData(), 4 * 900 * 1080);
	}

	/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */

	JournalManager->WriteJournalProgress(GameData);

	// FIXME: Why is this not saving properly?
	// - gameData *has* the binary data...
	UGameplayStatics::SaveGameToSlot(GameData, SaveGameSlot, 0);	
}

void UDESGameInstance::LoadGameData(bool resetGameData)
{
	// STEP 1: If not resetting settings, try to load any settings data written to disk...
	if (!resetGameData)
	{
		GameData = Cast<UDESSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSlot, 0));
	}

	// STEP 2: If settings aren't loaded, reset to defaults...
	resetGameData |= !GameData;
	if (resetGameData)
	{
		// Create new game data...
		GameData = Cast<UDESSaveGame>(UGameplayStatics::CreateSaveGameObject(UDESSaveGame::StaticClass()));

		// Progress variables
		GameData->ProgressStarted = false;

		// Player variables...
		GameData->PlayerPosition = FVector(0.0, 0.0, 0.0);
		GameData->PlayerCameraRotation = FRotator(0.0, 0.0, 0.0);

		GameData->bCrawling = false;
		GameData->CrawlDistance = 0.0f;

		GameData->PlayerHandheld = EHandheld::ENone;

		// Polaroid variables...
		GameData->PolaroidInventoried = 0;
		GameData->PolaroidAmmo = 27;

		GameData->CrowbarInventoried = 0;

		GameData->EntryActive = false;
		GameData->BinaryTexture.Init(128, 4 * 900 * 1080);
	}

	DEBUG_JournalEntry->EntryActive = GameData->EntryActive;
	DEBUG_JournalEntry->RenderTargetActive = !GameData->EntryActive;

	if (GameData->BinaryTexture.Num() == 4 * 900 * 1080) // NB: Safety check; prevents crashes!
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
		FMemory::Memcpy(MipBulkData, GameData->BinaryTexture.GetData(), 4 * 900 * 1080);
		Mip.BulkData.Unlock();

		// STEP 3: Update resources...
		Texture->UpdateResource();
		DEBUG_JournalEntry->Texture = Texture;

		// FIXME: No longer crashing, but not doing anything else?
		DEBUG_JournalEntry->RenderTarget->UpdateTexture2D(Texture, TSF_BGRA8);
		DEBUG_JournalEntry->RenderTarget->UpdateResource();

		/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */
	}

	// STEP 3: ...
	JournalManager->WriteJournalProgress(GameData, resetGameData);
	JournalManager->ReadJournalProgress(GameData);

	// NB: Updates will be applied by... the appropriate game mode?
}

void UDESGameInstance::SaveSettingsData() 
{
	if (!SettingsData)
		return;

	UGameplayStatics::SaveGameToSlot(SettingsData, SaveSettingsSlot, 0);	
}

void UDESGameInstance::LoadSettingsData(bool resetSettingsData) 
{
	// STEP 1: If not resetting settings, try to load any settings data written to disk...
	if (!resetSettingsData)
	{
		SettingsData = Cast<UDES_SaveSettings>(UGameplayStatics::LoadGameFromSlot(SaveSettingsSlot, 0));
	}

	// STEP 2: If settings aren't loaded, reset to defaults...
	resetSettingsData |= !SettingsData;
	if (resetSettingsData)
	{
		// Create new settings data...
		SettingsData = Cast<UDES_SaveSettings>(UGameplayStatics::CreateSaveGameObject(UDES_SaveSettings::StaticClass()));

		// Graphics settings...
		SettingsData->OverallScalability = 0;

		// Audio settings...
		SettingsData->MasterVolume = 1.0f;
		SettingsData->MasterMute = false;

		SettingsData->MusicVolume = 1.0f;
		SettingsData->MusicMute = false;

		SettingsData->SFX_Volume = 1.0f;
		SettingsData->SFX_Mute = false;

		SaveSettingsData();
	}

	// STEP 3: Update all settings according to SettingsData...
	UpdateOverallScalability(SettingsData->OverallScalability);

	UpdateMasterVolume(SettingsData->MasterVolume, false);
}

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */ 

void UDESGameInstance::UpdateOverallScalability(int overallScalability)
{
	SettingsData->OverallScalability = overallScalability;

	UGameUserSettings::GetGameUserSettings()->SetOverallScalabilityLevel(SettingsData->OverallScalability);
}

void UDESGameInstance::UpdateMasterVolume(float masterVolume, bool unmute)
{
	SettingsData->MasterVolume = masterVolume;
	if (unmute)
	{
		SettingsData->MasterMute = false;
	}

	UpdateBus(MasterMusic, MasterMix, GetMixWithMasterVolume(SettingsData->MusicVolume, SettingsData->MusicMute));
	UpdateBus(MasterSFX, MasterMix, GetMixWithMasterVolume(SettingsData->SFX_Volume, SettingsData->SFX_Mute));
}

void UDESGameInstance::UpdateMasterMute(bool masterMute)
{
	SettingsData->MasterMute = masterMute;

	UpdateBus(MasterMusic, MasterMix, GetMixWithMasterVolume(SettingsData->MusicVolume, SettingsData->MusicMute));
	UpdateBus(MasterSFX, MasterMix, GetMixWithMasterVolume(SettingsData->SFX_Volume, SettingsData->SFX_Mute));
}

void UDESGameInstance::UpdateMusicVolume(float musicVolume, bool unmute)
{
	SettingsData->MusicVolume = musicVolume;
	if (unmute)
	{
		SettingsData->MusicMute = false;
		SettingsData->MasterMute = false;
	}

	UpdateBus(MasterMusic, MasterMix, GetMixWithMasterVolume(SettingsData->MusicVolume, SettingsData->MusicMute));
}

void UDESGameInstance::UpdateMusicMute(bool musicMute)
{
	SettingsData->MusicMute = musicMute;

	UpdateBus(MasterMusic, MasterMix, GetMixWithMasterVolume(SettingsData->MusicVolume, SettingsData->MusicMute));
}

void UDESGameInstance::UpdateSFX_Volume(float SFX_Volume, bool unmute)
{
	SettingsData->SFX_Volume = SFX_Volume;
	if (unmute)
	{
		SettingsData->SFX_Mute = false;
		SettingsData->MasterMute = false;
	}

	UpdateBus(MasterSFX, MasterMix, GetMixWithMasterVolume(SettingsData->SFX_Volume, SettingsData->SFX_Mute));
}

void UDESGameInstance::UpdateSFX_Mute(bool SFX_Mute)
{	
	SettingsData->SFX_Mute = SFX_Mute;

	UpdateBus(MasterSFX, MasterMix, GetMixWithMasterVolume(SettingsData->SFX_Volume, SettingsData->SFX_Mute));
}

void UDESGameInstance::UpdateBus(USoundControlBus* bus, USoundControlBusMix* mix, float volume)
{
	if (!bus || !mix)
		return;

	FSoundControlBusMixStage ControlBusMixStage = UAudioModulationStatics::CreateBusMixStage(GetWorld(), bus, volume);

	TArray<FSoundControlBusMixStage> ControlBusMixStageArray;
	ControlBusMixStageArray.Add(ControlBusMixStage);

	UAudioModulationStatics::UpdateMix(GetWorld(), mix, ControlBusMixStageArray);
}

float UDESGameInstance::GetMixWithMasterVolume(float volume, bool mute)
{
	return (mute || SettingsData->MasterMute) ? 0.0f : volume*SettingsData->MasterVolume;
}