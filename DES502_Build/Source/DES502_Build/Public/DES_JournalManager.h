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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FDES_JournalStruct Journal;

public:
	void ReadJournalData(FString FilePath);

	void ReadJournalProgress(); // NB: Reference to SaveGame.h?
	void WriteJournalProgress(); // NB: Reference to SaveGame.h?
};
