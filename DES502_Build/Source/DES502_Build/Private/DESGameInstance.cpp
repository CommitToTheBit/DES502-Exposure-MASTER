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
	GameData->PolaroidAmmo = 27;

	// Crawl Space Variables...
	GameData->bCrawling = false;
	GameData->CrawlDistance = 0.0f;

	UGameplayStatics::SaveGameToSlot(GameData, SaveSlot, 0);
}

void UDESGameInstance::SaveGameData(
	FVector PlayerPosition,
	int PolaroidAmmo,
	bool bCrawling,
	float CrawlDistance
) 
{
	if (!GameData)
		return;

	GameData->PlayerPosition = PlayerPosition;
	GameData->PolaroidAmmo = PolaroidAmmo;

	// Crawl Space Variables...
	GameData->bCrawling = bCrawling;
	GameData->CrawlDistance = CrawlDistance;

	UGameplayStatics::SaveGameToSlot(GameData, SaveSlot, 0);
}

UDESSaveGame* UDESGameInstance::LoadGameData()
{
	return (GameData) ? GameData : nullptr;
}