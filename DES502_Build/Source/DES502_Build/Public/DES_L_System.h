#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DES_L_System.generated.h"

UCLASS()
class DES502_BUILD_API ADES_L_System : public AActor
{
	GENERATED_BODY()

/* USTRUCTS */
protected:
	// Seed vertex information, used for normalising turtle drawings...
	struct SeedVertexType
	{
	public:
		int Parent;
		FTransform Transform;
		FVector Position;
		float Depth;
	};

	// Tree vertex information, used for 'structuring' turtle drawings...
	struct TreeVertexType
	{
		int Parent;
		FTransform Transform;
		FVector Position;
		float Width;
		int Degree;
	};

public:
	struct LModuleType
	{
		std::string Letter;

		float Period;
		float Aperiodicity;
		float Synchronisation;
		float Asynchronicity;

		float StaticLength;
		float RandomStaticLength;
		float PeriodicLength;
		float RandomPeriodicLength;

		float StaticRotation;
		float RandomStaticRotation;
		float PeriodicRotation;
		float RandomPeriodicRotation;

		float StaticWidth;
		float RandomStaticWidth;
		float PeriodicWidth;
		float RandomPeriodicWidth;

		float Asymmetry;
		float StaticAsymmetry;
		float RandomStaticAsymmetry;
		float PeriodicAsymmetry;
		float RandomPeriodicAsymmetry;

		LModuleType();
	};

	struct ProductionRuleType
	{
		std::vector<std::function<LModuleType(LModuleType)>> Productions;
		float Weight;
	};
	
public:
	UPROPERTY()
		TArray<FVector> Vertices;

	UPROPERTY()
		TArray<FVector> Indices;

public:	
	ADES_L_System();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
