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
		int GraphicsOverallScalability; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
		float AudioMasterVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
		bool AudioMasterMute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
		float AudioMusicVolume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
		bool AudioMusicMute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
		float AudioSFX_Volume;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
		bool AudioSFX_Mute;
};
