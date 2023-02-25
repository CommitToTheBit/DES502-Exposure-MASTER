// CC: AwesomeTuts...

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "DESSaveGame.h"
#include "Engine.h"

#include "DESGameInstance.generated.h"

class UDESSaveGame;

UCLASS()
class DES502_BUILD_API UDESGameInstance : public UGameInstance
{
	GENERATED_BODY()

	void Init() override;

	FString SaveSlot = "GameData";

	UDESSaveGame* GameData;

public:

	UFUNCTION(BlueprintCallable, Category="Saving / Loading")
		void SaveGameData(
			FVector PlayerPosition,
			int CameraAmmo
		);

	UFUNCTION(BlueprintCallable, Category="Saving / Loading")
		UDESSaveGame* LoadGameData();
};
