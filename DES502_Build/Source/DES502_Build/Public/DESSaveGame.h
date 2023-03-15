// CC: AwesomeTuts...

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DESSaveGame.generated.h"

UCLASS()
class DES502_BUILD_API UDESSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
		float SettingsMasterVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
		bool SettingsMasterMute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
		float SettingsMusicVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
		bool SettingsMusicMute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
		float SettingsSFX_Volume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
		bool SettingsSFX_Mute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
		FVector PlayerPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
		FRotator PlayerCameraRotation;

	// FIXME: CrawlSpaces need refactored...
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
		bool bCrawling;

	// FIXME: CrawlSpaces need refactored...
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
		float CrawlDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Polaroid")
		int PolaroidAmmo;
};
