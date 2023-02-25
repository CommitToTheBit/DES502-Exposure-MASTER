// CC: AwesomeTuts...

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "DESSaveGame.h"
#include "Engine.h"

#include "DESGameInstance.generated.h"

UCLASS()
class DES502_BUILD_API UDESGameInstance : public UGameInstance
{
	GENERATED_BODY()

	void Init() override;

	FString SaveSlot = "GameData";

	class UDESSaveGame* GameData;

public:
	void SaveGameData(
		FVector PlayerPosition,
		int CameraAmmo
	);
	class UDESSaveGame* LoadGameData();
};
