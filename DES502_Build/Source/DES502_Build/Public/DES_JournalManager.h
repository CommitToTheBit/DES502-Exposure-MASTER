#pragma once

#include "CoreMinimal.h"

#include "DES_JournalEntry.h"
#include "Engine.h"

#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"

#include "DES_JournalManager.generated.h"

USTRUCT(BlueprintType, Category = "Journal")
struct FDES_JournalEntryStruct
{
	GENERATED_BODY()

public:
	// DATA: Variables read from the JSON...
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
		FString ID = "N/A";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
		FString Title = "N/A";

	// PROGRESS: Variables determined by the player...
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		bool EntryActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		bool RenderTargetActive = true; // NB: Not saved to texture yet!

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		UTextureRenderTarget2D* RenderTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		UTexture2D* Texture;
};

USTRUCT(BlueprintType, Category = "Journal")
struct FDES_JournalStruct
{
	GENERATED_BODY()

public:
	UPROPERTY()
		TMap<FString, FDES_JournalEntryStruct> Entries;

	// Copy constructor
	FDES_JournalStruct()
	{
		Entries = TMap<FString, FDES_JournalEntryStruct>();
	}
};

UCLASS()
class DES502_BUILD_API UDES_JournalManager : public UObject
{
	GENERATED_BODY()

public:
	FDES_JournalStruct Journal;
	FDES_JournalEntryStruct dummy;

public:
	void InitialiseEntries(FString FilePath);

private:
	FDES_JournalStruct ReadJournalStructFromFile(FString FilePath);
};
