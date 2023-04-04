#pragma once

#include "CoreMinimal.h"

#include "DES_JournalEntry.h"
#include "Engine.h"

#include "DES_JournalManager.generated.h"

UCLASS()
class DES502_BUILD_API UDES_JournalManager : public UObject
{
	GENERATED_BODY()

public:
	void InitialiseEntries(FString FilePath);

private:
	TSharedPtr<FJsonObject> ReadJSONFromFile(FString FilePath);
};
