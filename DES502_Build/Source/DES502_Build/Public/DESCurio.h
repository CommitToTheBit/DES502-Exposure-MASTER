#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DESCurio.generated.h"

UCLASS()
class DES502_BUILD_API ADESCurio : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recognition")
		FString ID;

	// NB: Will we assign a 'value' to each photo composition? Could get out of hand...
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recognition")
	//	float Score;
};
