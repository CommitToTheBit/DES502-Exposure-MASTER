// CC: AwesomeTuts...

#include "DESGameInstance.h"

void UDESGameInstance::Init()
{
	// Try loading data...
	GameData = Cast<UDESSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, 0));
	if (GameData)
		return;

	// If this fails, create new game data...
	GameData = Cast<UDESSaveGame>(UGameplayStatics::CreateSaveGameObject(UDESSaveGame::StaticClass()));

	GameData->PlayerPosition = FVector(0.0, 0.0, 0.0);
	GameData->CameraAmmo = 27;

	UGameplayStatics::SaveGameToSlot(GameData, SaveSlot, 0);
}

void UDESGameInstance::SaveGameData(
	FVector PlayerPosition,
	int CameraAmmo
) 
{
	if (!GameData)
		return;

	GameData->PlayerPosition = PlayerPosition;
	GameData->CameraAmmo = CameraAmmo;

	UGameplayStatics::SaveGameToSlot(GameData, SaveSlot, 0);
}

UDESSaveGame* UDESGameInstance::LoadGameData()
{
	return (GameData) ? GameData : nullptr;
}