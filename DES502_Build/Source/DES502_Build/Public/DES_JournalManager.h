#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DES_JournalManager.generated.h"

UCLASS()
class DES502_BUILD_API UDES_JournalManager : public UObject
{
	GENERATED_BODY()

public:
	void InitEntriesFromJSON(FString path);
};
