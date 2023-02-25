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
	UPROPERTY()
		FVector PlayerPosition;

	UPROPERTY()
		int CameraAmmo;
};
