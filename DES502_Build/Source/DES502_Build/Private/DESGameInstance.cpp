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

	// Settings Variables...
	SettingsData->AudioMasterVolume = 1.0f;
	SettingsData->AudioMasterMute = false;

	SettingsData->AudioMusicVolume = 1.0f;
	SettingsData->AudioMusicMute = false;

	SettingsData->AudioSFX_Volume = 1.0f;
	SettingsData->AudioSFX_Mute = false;

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
	
}

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */ 