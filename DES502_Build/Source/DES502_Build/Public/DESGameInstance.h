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

	// NB: Settings to be interacted with at high-level
	
	// FIXME: ...Why can't I find documentation for audio modulation??
	// TO TIDY UP AUDIO OUT OF UI...
	// Can't use a blueprint, so all functionality needs contained within C++...
	// To do this, need the right documentation...
	// Necessarily leaving this until I've got a better sense of C++ for UE5...
};
