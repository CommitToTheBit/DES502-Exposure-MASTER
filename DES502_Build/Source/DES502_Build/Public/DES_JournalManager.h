#pragma once

#include "CoreMinimal.h"

#include "DES_JournalEntry.h" // DELETE THIS!
#include "DES_JournalStruct.h"
#include "Engine.h"

#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"

#include "DES_JournalManager.generated.h"

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
