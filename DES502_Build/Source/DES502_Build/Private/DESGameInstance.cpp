#include "DESGameInstance.h"

void UDESGameInstance::Init()
{
	// BUGFIX: This is what makes the calls that set up widgets!
	Super::Init();

	// Load any saved data...
	LoadGameData(); // DEBUG: Set to true to reset GameData on start...
	LoadSettingsData();

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

	//FSoundControlBusMixStage* test;
	//static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("Material'/Game/Materials/YourMaterialName.YourMaterialName'")); // "Material'/DES502_Build/Content/Materials/YourMaterialName.YourMaterialName'"
	//static ConstructorHelpers::FObjectFinder<UMaterial>
	//Material(TEXT("/Game/VFX/EDITOR_Vignette_Trigger.EDITOR_Vignette_Trigger'"));
	//static ConstructorHelpers::FObjectFinder<USoundControlBus>MasterBus(TEXT("Material'/Game/Materials/YourMaterialName.YourMaterialName'"));

	/*loadedMaterial = Material.Object;

	//GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, TEXT("DEBUG..."));

	if (Material.Succeeded())//Material.Object != NULL)
		GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, TEXT("Material loaded..."));
	else
		GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, TEXT("Material not loaded..."));*/
}

/* -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- */ 
/* This enclosed section has been adapted from: Awesome Tuts (no date) Saving And Loading Game Data With Blueprints And C++ In Unreal Engine. Available at: https://awesometuts.com/blog/save-load-game-data-unreal-engine/ (Accessed: 22 March 2022) */

void UDESGameInstance::SaveGameData() 
{
	if (GameData)
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
	if (!GameData)
	{
		// Create new game data...
		GameData = Cast<UDESSaveGame>(UGameplayStatics::CreateSaveGameObject(UDESSaveGame::StaticClass()));

		// Player variables...
		GameData->PlayerPosition = FVector(0.0, 0.0, 0.0);
		GameData->PlayerCameraRotation = FRotator(0.0, 0.0, 0.0);

		GameData->bCrawling = false;
		GameData->CrawlDistance = 0.0f;

		// Polaroid variables...
		GameData->PolaroidAmmo = 27;
	}

	// NB: Updates will be applied by... the appropriate game mode?
}

void UDESGameInstance::SaveSettingsData() 
{
	if (SettingsData)
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
	if (!SettingsData)
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

	UpdateMasterVolume(SettingsData->MasterVolume, true);
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

void UDESGameInstance::UpdateMusicVolume(float musicVolume, bool unmute)
{
	SettingsData->MusicVolume = musicVolume;
	if (unmute)
	{
		SettingsData->MusicMute = false;
		SettingsData->MasterMute = false;
	}
}

void UDESGameInstance::UpdateMusicMute(bool musicMute)
{
	SettingsData->MusicMute = musicMute;
}

void UDESGameInstance::UpdateSFX_Volume(float SFX_Volume, bool unmute)
{
	SettingsData->SFX_Volume = SFX_Volume;
	if (unmute)
	{
		SettingsData->SFX_Mute = false;
		SettingsData->MasterMute = false;
	}
}

void UDESGameInstance::UpdateSFX_Mute(bool SFX_Mute)
{
	SettingsData->SFX_Mute = SFX_Mute;
}

float UDESGameInstance::GetMixWithMasterVolume(float volume, bool mute)
{
	return (mute || SettingsData->MasterMute) ? 0.0f : volume*SettingsData->MasterVolume;
}