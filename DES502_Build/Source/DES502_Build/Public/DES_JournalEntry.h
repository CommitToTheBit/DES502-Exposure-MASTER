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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FString Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
		bool EntryActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
		bool RenderTargetActive = true; // NB: Not saved to texture yet!

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
		UTextureRenderTarget2D* RenderTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
		UTexture2D* Texture;

private:

};
