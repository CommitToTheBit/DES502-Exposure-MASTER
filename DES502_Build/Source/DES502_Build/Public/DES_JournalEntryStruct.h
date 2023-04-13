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
		FString CaptionText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
		FString ExtractType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
		FString ExtractText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
		FString HeadlineText;

	// PROGRESS: Variables determined by the player...
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		int EntryActive = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		bool RenderTargetActive = true; // NB: Not saved to texture yet!

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		UTextureRenderTarget2D* RenderTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		UTexture2D* Texture;

	FDES_JournalEntryStruct();
	~FDES_JournalEntryStruct();
};
