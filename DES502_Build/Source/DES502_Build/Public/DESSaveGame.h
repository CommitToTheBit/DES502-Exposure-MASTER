#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DESSaveGame.generated.h"

UENUM(BlueprintType)
enum class EHandheld : uint8 {
	ENone		UMETA(Displayname = "None"),
	EPolaroid	UMETA(DisplayName = "Polaroid"),
	ECrowbar	UMETA(DisplayName = "Crowbar"),
	ETorch		UMETA(DisplayName = "Torch")
};

UCLASS()
class DES502_BUILD_API UDESSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		bool ProgressStarted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		bool ProgressSplashRead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		bool ProgressSawneyWallCrawled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		FVector PlayerPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		FRotator PlayerCameraRotation;

	// FIXME: CrawlSpaces need refactored...
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		bool bCrawling;

	// FIXME: CrawlSpaces need refactored...
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		float CrawlDistance;

	// HANDHELD SAVE DATA:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		EHandheld PlayerHandheld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polaroid")
		int PolaroidInventoried;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polaroid")
		int PolaroidAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crowbar")
		int CrowbarInventoried;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torch")
		int TorchInventoried;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torch")
		bool TorchBreak;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torch")
		bool TorchBroken;

	// JOURNAL SAVE DATA:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Journal")
		TArray<int> EntriesActive; // Store IDs in... ??

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Journal")
		TArray<uint8> BinaryTextures;

	// TUTORIAL SAVE DATA:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
		bool TutorialisedRaise;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
		bool TutorialisedRaiseCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
		bool TutorialisedPressCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
		bool TutorialisedOpenJournal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
		bool TutorialisedRaisePickaxe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
		bool TutorialisedSwingPickaxe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
		bool TutorialisedScrollInventory;
};