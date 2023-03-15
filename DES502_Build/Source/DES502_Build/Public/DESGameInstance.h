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

public:
	// NB: Getting/setting handles most 'saving/loading' functionality...
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Saving / Loading") // NB: BlueprintReadOnly because... Blueprints can't 'rewrite' this C++ code?
		UDESSaveGame* GameData;

public:
	// NB: Saving/loading are purely interactions with the disk...
	UFUNCTION(BlueprintCallable, Category="Saving / Loading")
		void SaveGameData();

	UFUNCTION(BlueprintCallable, Category="Saving / Loading")
		void LoadGameData(bool resetGameData = false);
};
