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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Saving / Loading")
		FVector PlayerPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Saving / Loading")
		FRotator PlayerCameraRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Saving / Loading")
		int PolaroidAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Saving / Loading")
		bool bCrawling;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Saving / Loading")
		float CrawlDistance;
};
