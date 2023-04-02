#include "DESGameInstance.h"

void UDESGameInstance::Init()
{
	// BUGFIX: This is what makes the calls that set up widgets!
	Super::Init();

	// Load any saved data...
	LoadGameData(true); // DEBUG: Set to true to reset GameData on start...
	LoadSettingsData();

	// Create journal manager...
	// FIXME: Fold this into a full 'Update' function...
	DEBUG_JournalEntry = NewObject<UDES_JournalEntry>(UDES_JournalEntry::StaticClass());
	DEBUG_JournalEntry->RenderTarget = NewObject<UTextureRenderTarget2D>(UTextureRenderTarget2D::StaticClass());
	DEBUG_JournalEntry->RenderTarget->InitAutoFormat(1080, 900); // NB: This line, previously missing, is what solves the access violation!

	//TArray<FColor> colors;
	//colors.Init(FColor::White, 1080 * 900);

	//uint8* check = new uint8[40000];
	//for (int i = 0; i < 40000; i++)
	//	check[i] = 200;

	//DEBUG_JournalEntry->RenderTarget->Source.Init(1080,900,0,0,TSF_RGBA16F,check);//; GetRenderTargetResource()->Set
}

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */ 
/* This enclosed section has been adapted from: Awesome Tuts (no date) Saving And Loading Game Data With Blueprints And C++ In Unreal Engine. Available at: https://awesometuts.com/blog/save-load-game-data-unreal-engine/ (Accessed: 22 March 2022) */

void UDESGameInstance::SaveGameData() 
{
	if (!GameData)
		return;

	UGameplayStatics::SaveGameToSlot(GameData, SaveGameSlot, 0);	

	// DEBUG:
	// FIXME: Crashes if RenderTarget is empty... so need to initialise it on creation!
	//colors.Init(FColor::White, 1080 * 900);
	DEBUG_JournalEntry->RenderTarget->GetRenderTargetResource()->ReadPixels(GameData->DEBUG_Pixels);
}

void UDESGameInstance::LoadGameData(bool resetGameData)
{
	// STEP 1: If not resetting settings, try to load any settings data written to disk...
	if (!resetGameData)
	{
		GameData = Cast<UDESSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSlot, 0));
	}

	// STEP 2: If settings aren't loaded, reset to defaults...
	if (resetGameData || !GameData)
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

		// DEBUG:
		GameData->DEBUG_Pixels.Init(FColor::Red, 1080 * 900);

		//GameData->DEBUG_JournalEntry->EntryRenderTarget = NewObject<UTextureRenderTarget2D>(UTextureRenderTarget2D::StaticClass());
		
		//GameData->DEBUG_JournalEntry->EntryRenderTarget->SizeX = 1080;
		//GameData->DEBUG_JournalEntry->EntryRenderTarget->SizeY = 1080;

		//UTextureRenderTarget2D::UTextureRenderTarget2D()

		//if (GameData->DEBUG_JournalEntry->EntryRenderTarget)
		//	GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, TEXT("Success?"));
	}

	// NB: Updates will be applied by... the appropriate game mode?
	GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, GameData->DEBUG_Pixels[0].ToString());

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
	if (resetSettingsData || !SettingsData)
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