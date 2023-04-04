#pragma once

#include "CoreMinimal.h"
#include "Engine.h"

#include "DES_JournalEntryStruct.generated.h"

USTRUCT(BlueprintType, Category = "Journal")
struct FDES_JournalEntryStruct
{
	GENERATED_BODY()

public:
	// DATA: Variables read from the JSON...
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
		FString ID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
		FString Title;

	// PROGRESS: Variables determined by the player...
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		bool EntryActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		bool RenderTargetActive = true; // NB: Not saved to texture yet!

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		UTextureRenderTarget2D* RenderTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		UTexture2D* Texture;

	FDES_JournalEntryStruct();
	~FDES_JournalEntryStruct();
};
