#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DES_SaveSettings.generated.h"

UCLASS()
class DES502_BUILD_API UDES_SaveSettings : public USaveGame
{
	GENERATED_BODY()
	
public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Graphics")
		int OverallScalability; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
		float MasterVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
		bool MasterMute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
		float MusicVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
		bool MusicMute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
		float SFX_Volume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
		bool SFX_Mute;
};
