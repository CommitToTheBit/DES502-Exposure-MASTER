// CC: AwesomeTuts...

#include "DESGameInstance.h"

void UDESGameInstance::Init()
{
	// BUGFIX: This is what makes the calls that set up widgets!
	Super::Init();

	// Load any saved data...
	LoadGameData(true); // DEBUG: Set to true to reset GameData on start...
}

void UDESGameInstance::SaveGameData() 
{
	if (!GameData)
		return;

	UGameplayStatics::SaveGameToSlot(GameData, SaveSlot, 0);
}

void UDESGameInstance::LoadGameData(bool resetGameData)
{
	// Try loading data...
	if (!resetGameData)
	{
		GameData = Cast<UDESSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, 0));
		if (GameData)
			return;
	}

	// If this fails, create new game data...
	GameData = Cast<UDESSaveGame>(UGameplayStatics::CreateSaveGameObject(UDESSaveGame::StaticClass()));

	// Settings Variables...
	GameData->SettingsMasterVolume = 1.0f;
	GameData->SettingsMasterMute = false;

	GameData->SettingsMusicVolume = 1.0f;
	GameData->SettingsMusicMute = false;

	GameData->SettingsSFX_Volume = 1.0f;
	GameData->SettingsSFX_Mute = false;

	// Player Variables...
	GameData->PlayerPosition = FVector(0.0, 0.0, 0.0);
	GameData->PlayerCameraRotation = FRotator(0.0, 0.0, 0.0);

	GameData->bCrawling = false;
	GameData->CrawlDistance = 0.0f;

	// Polaroid Variables...
	GameData->PolaroidAmmo = 27;

	UGameplayStatics::SaveGameToSlot(GameData, SaveSlot, 0);
}