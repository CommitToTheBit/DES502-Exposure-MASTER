#pragma once

#include "CoreMinimal.h"
#include "Engine.h"

#include "DES_JournalEntry.generated.h"

class UTextureRenderTarget2D;

UCLASS()
class DES502_BUILD_API UDES_JournalEntry : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		UTextureRenderTarget2D* EntryRenderTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		int test = 20;
};
