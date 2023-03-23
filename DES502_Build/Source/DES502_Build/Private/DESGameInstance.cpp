#include "DESGameInstance.h"

void UDESGameInstance::Init()
{
	// BUGFIX: This is what makes the calls that set up widgets!
	Super::Init();

	// Load any saved data...
	LoadGameData(true); // DEBUG: Set to true to reset GameData on start...

	// Set all graphics to low quality, as default...
	UGameUserSettings* gameUserSettings = UGameUserSettings::GetGameUserSettings();
	gameUserSettings->SetOverallScalabilityLevel(0);

	/* FIXME: Components to incorporate as individual graphics settings...?
	gameUserSettings->SetResolutionScaleNormalized(0.25f);
	gameUserSettings->SetViewDistanceQuality(0);
	gameUserSettings->SetAntiAliasingQuality(0);
	gameUserSettings->SetPostProcessingQuality(0);
	gameUserSettings->SetShadowQuality(0);
	gameUserSettings->SetGlobalIlluminationQuality(0);
	gameUserSettings->SetReflectionQuality(0);
	gameUserSettings->SetTextureQuality(0);
	gameUserSettings->SetVisualEffectQuality(0);
	gameUserSettings->SetFoliageQuality(0);
	gameUserSettings->SetShadingQuality(0); */

}

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */ 
/* This enclosed section has been adapted from: Awesome Tuts (no date) Saving And Loading Game Data With Blueprints And C++ In Unreal Engine. Available at: https://awesometuts.com/blog/save-load-game-data-unreal-engine/ (Accessed: 22 March 2022) */

void UDESGameInstance::SaveGameData() 
{
	if (!GameData)
		return;

	UGameplayStatics::SaveGameToSlot(GameData, SaveGameSlot, 0);	
}

void UDESGameInstance::LoadGameData(bool resetGameData)
{
	// Try loading data...
	if (!resetGameData)
	{
		GameData = Cast<UDESSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSlot, 0));
		if (GameData)
			return;
	}

	// If this fails, create new game data...
	GameData = Cast<UDESSaveGame>(UGameplayStatics::CreateSaveGameObject(UDESSaveGame::StaticClass()));

	// Player Variables...
	GameData->PlayerPosition = FVector(0.0, 0.0, 0.0);
	GameData->PlayerCameraRotation = FRotator(0.0, 0.0, 0.0);

	GameData->bCrawling = false;
	GameData->CrawlDistance = 0.0f;

	// Polaroid Variables...
	GameData->PolaroidAmmo = 27;

	UGameplayStatics::SaveGameToSlot(GameData, SaveGameSlot, 0);
}

void UDESGameInstance::SaveSettingsData() 
{

}

void UDESGameInstance::LoadSettingsData(bool resetSettingsData) 
{
	// STEP 1: If not resetting settings, try to load any settings data written to disk...
	if (!resetSettingsData)
	{
		SettingsData = Cast<UDES_SaveSettings>(UGameplayStatics::LoadGameFromSlot(SaveSettingsSlot, 0));
	}

	// STEP 2: If settings aren't loaded, reset to defaults...
	if (!SettingsData)
	{
		// Graphics
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
}

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */ 

void UDESGameInstance::UpdateMasterVolume(float masterVolume)
{
	SettingsData->MasterVolume = masterVolume;

	// FIXME: Update both buses here...
	// Create music bus...
	GetMixWithMasterVolume(SettingsData->MusicVolume, SettingsData->MusicMute);

	// Create SFX bus...
	GetMixWithMasterVolume(SettingsData->SFX_Volume, SettingsData->SFX_Mute);
}

void UDESGameInstance::UpdateMasterMute(bool masterMute)
{
	SettingsData->MasterMute = masterMute;
}

void UDESGameInstance::UpdateMusicVolume(float musicVolume)
{
	SettingsData->MusicVolume = musicVolume;
}

void UDESGameInstance::UpdateMusicMute(bool musicMute)
{
	SettingsData->MusicMute = musicMute;
}

void UDESGameInstance::UpdateSFX_Volume(float sfxVolume)
{
	SettingsData->SFX_Volume = sfxVolume;
}

void UDESGameInstance::UpdateSFX_Mute(bool sfxMute)
{
	SettingsData->SFX_Mute = sfxMute;
}

float UDESGameInstance::GetMixWithMasterVolume(float volume, bool mute)
{
	return (mute || SettingsData->MasterMute) ? 0.0f : volume*SettingsData->MasterVolume;
}