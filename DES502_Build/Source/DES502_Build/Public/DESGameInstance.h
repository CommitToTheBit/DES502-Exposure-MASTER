#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "DESSaveGame.h"
#include "DES_SaveSettings.h"
#include "Engine.h"

#include "AudioModulationStatics.h"

#include "DESGameInstance.generated.h"

class UDESSaveGame;
class UDES_SaveSettings;

UCLASS()
class DES502_BUILD_API UDESGameInstance : public UGameInstance
{
	GENERATED_BODY()

	void Init() override;

	FString SaveGameSlot = "GameData";
	FString SaveSettingsSlot = "SettingsData";

public:
	// NB: Getting/setting handles most 'saving/loading' functionality...
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Saving / Loading") // NB: BlueprintReadOnly because... Blueprints can't 'rewrite' this C++ code?
		UDESSaveGame* GameData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Saving / Loading") // NB: BlueprintReadOnly because... Blueprints can't 'rewrite' this C++ code?
		UDES_SaveSettings* SettingsData;

	// AUDIO SETTINGS:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
		USoundControlBusMix* MasterMix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
		USoundControlBus* MasterSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
		USoundControlBus* MasterMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
		USoundControlBusMix* SFX_Mix;

public:
	// GAME DATA:
	UFUNCTION(BlueprintCallable, Category="Saving / Loading")
		void SaveGameData();

	UFUNCTION(BlueprintCallable, Category="Saving / Loading")
		void LoadGameData(bool resetGameData = false);

	// SETTINGS DATA:
	UFUNCTION(BlueprintCallable, Category="Saving / Loading")
		void SaveSettingsData();

	UFUNCTION(BlueprintCallable, Category="Saving / Loading")
		void LoadSettingsData(bool resetSettingsData = false);

	// GRAPHICS SETTINGS:
	UFUNCTION(BlueprintCallable, Category="Graphics")
		void UpdateOverallScalability(int overallScalability);

	// AUDIO SETTINGS:
	UFUNCTION(BlueprintCallable, Category="Audio")
		void UpdateMasterVolume(float masterVolume, bool unmute = true);

	UFUNCTION(BlueprintCallable, Category="Audio")
		void UpdateMasterMute(bool masterMute);

	UFUNCTION(BlueprintCallable, Category="Audio")
		void UpdateMusicVolume(float musicVolume, bool unmute = true);

	UFUNCTION(BlueprintCallable, Category="Audio")
		void UpdateMusicMute(bool musicMute);

	UFUNCTION(BlueprintCallable, Category="Audio")
		void UpdateSFX_Volume(float sfxVolume, bool unmute = true);

	UFUNCTION(BlueprintCallable, Category="Audio")
		void UpdateSFX_Mute(bool sfxMute);

	// NB: Settings to be interacted with at high-level
	
	// FIXME: ...Why can't I find documentation for audio modulation??
	// TO TIDY UP AUDIO OUT OF UI...
	// Can't use a blueprint, so all functionality needs contained within C++...
	// To do this, need the right documentation...
	// Necessarily leaving this until I've got a better sense of C++ for UE5...

	
private:
	// AUDIO SETTINGS:
	void UpdateBus(USoundControlBus* bus, USoundControlBusMix* mix, float volume);
	float GetMixWithMasterVolume(float volume, bool mute);
};
