#pragma once

#include "CoreMinimal.h"

#include "DES_JournalEntry.h" // DELETE THIS!
#include "DES_JournalStruct.h"
#include "Engine.h"

#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"

#include "DES_JournalManager.generated.h"

//Use USTRUCT(BlueprintType) if you would like to include your Struct in Blueprints
USTRUCT(BlueprintType, Category = "Journal")
struct DES502_BUILD_API FPlayerStats
{
	GENERATED_USTRUCT_BODY()
		// Use UPROPERTY() to decorate member variables as they allow for easier integration with network replication as well as potential garbage collection processing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		FString Running;
	FPlayerStats()
	{
		Running = "Yes!";
	}
};
// ...

UCLASS()
class DES502_BUILD_API UDES_JournalManager : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		FDES_JournalStruct Journal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
		FPlayerStats dummy;

public:
	void InitialiseEntries(FString FilePath);

private:
	FDES_JournalStruct ReadJournalStructFromFile(FString FilePath);
};
