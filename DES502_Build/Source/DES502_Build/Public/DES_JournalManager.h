#pragma once

#include "CoreMinimal.h"

#include "DES_JournalEntry.h"
#include "Engine.h"

#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"

#include "DES_JournalManager.generated.h"

USTRUCT(BlueprintType, Category = "JSON")
struct FDES_JournalEntryStruct
{
	GENERATED_BODY()

public:
	UPROPERTY()
		FString ID;

	UPROPERTY()
		FString Title;
};

USTRUCT(BlueprintType, Category = "JSON")
struct FDES_JournalStruct
{
	GENERATED_BODY()

public:
	UPROPERTY()
		TArray<FDES_JournalEntryStruct> JournalEntries;
};

UCLASS()
class DES502_BUILD_API UDES_JournalManager : public UObject
{
	GENERATED_BODY()

public:
	void InitialiseEntries(FString FilePath);

private:
	TArray<FDES_JournalEntryStruct> ReadJsonStructsFromFile(FString FilePath);
};
