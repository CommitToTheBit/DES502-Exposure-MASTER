#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DESCurio.generated.h"

UCLASS()
class DES502_BUILD_API ADESCurio : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Scoring")
		float CurioScore;
};
