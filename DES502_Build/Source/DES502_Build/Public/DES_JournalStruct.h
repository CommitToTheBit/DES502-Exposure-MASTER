#pragma once

#include "CoreMinimal.h"
#include "Engine.h"

#include "DES_JournalEntryStruct.h"

#include "DES_JournalStruct.generated.h"

USTRUCT(BlueprintType, Category = "Journal")
struct DES502_BUILD_API FDES_JournalStruct
{
	GENERATED_BODY()

public:
	UPROPERTY()
		TMap<FString, FDES_JournalEntryStruct> Entries;

	FDES_JournalStruct();
	~FDES_JournalStruct();
};
