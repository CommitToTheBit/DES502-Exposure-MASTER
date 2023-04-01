#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "DES_JournalEntry.h"

#include "DESSaveGame.generated.h"

class UDES_JournalEntry;

UENUM(BlueprintType)
enum class EHandheld : uint8 {
	EPolaroid	UMETA(DisplayName = "Polaroid"),
	ECrowbar	UMETA(DisplayName = "Crowbar"),
	ENone		UMETA(Displayname = "None")
};

UCLASS()
class DES502_BUILD_API UDESSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		bool ProgressStarted;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		EHandheld PlayerHandheld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polaroid")
		int PolaroidInventoried;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polaroid")
		int PolaroidAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crowbar")
		int CrowbarInventoried;

	// DEBUG:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Journal")
		UDES_JournalEntry* DEBUG_JournalEntry;

};
