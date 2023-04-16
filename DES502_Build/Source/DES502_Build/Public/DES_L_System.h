#pragma once

#include <functional>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DES_L_System.generated.h"

class UProceduralMeshComponent;

// Seed vertex information, used for normalising turtle drawings...
USTRUCT(BlueprintType)
struct DES502_BUILD_API FDES_SeedVertex
{
	GENERATED_BODY()

		int Parent;
	FTransform Transform;
	FVector Position;
	float Depth;
};

// Tree vertex information, used for 'structuring' turtle drawings...
USTRUCT(BlueprintType)
struct DES502_BUILD_API FDES_TreeVertex
{
	GENERATED_BODY()

		int Parent;
	FTransform Transform;
	FVector Position;
	float Width;
	int Degree;
};

USTRUCT(BlueprintType)
struct DES502_BUILD_API FDES_LModule
{
	GENERATED_BODY()

	FString Letter;

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

	FDES_LModule();
};

USTRUCT(BlueprintType)
struct DES502_BUILD_API  FDES_ProductionRule
{
	GENERATED_BODY()

	TArray<std::function<FDES_LModule(FDES_LModule)>> Productions;
	float Weight;
};

UCLASS()
class DES502_BUILD_API ADES_L_System : public AActor
{
	GENERATED_BODY()

public:
	ADES_L_System();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Generation")
		virtual void Initialize(/*UProceduralMeshComponent* Mesh,*/ TArray<FDES_LModule> Axiom, int Iterations, float Seed = 0.0f, float Rotation = 0.0f, FVector2D Anchoring = FVector2D(0.5f, 0.5f));

	UFUNCTION(BlueprintCallable, Category = "Generation")
		void Update(float DeltaTime, float DeltaIntensity);

	// DEBUG:
	//float* GetIntensity();
	//std::string GetSentence();

protected:
	virtual void BeginPlay() override;

	void AddProductionRule(FString Letter, FDES_ProductionRule ProductionRule);

private:
	void InitializeSentence(float Seed, TArray<FDES_LModule> Axiom, int Iterations);
	void InitializeTree(float Seed, float Rotation, FVector2D Anchoring);
	void InitializeMesh(UProceduralMeshComponent* Mesh);

	void UpdateTree(float DeltaTime, float DeltaIntensity);
	//void UpdateMesh(); // FIXME: Add this!

	FDES_ProductionRule GetProductionRule(FString Letter);
	float GetRNGRange(float a = -1.0f, float b = 1.0f);

/*public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* ProceduralMesh;*/

/*private:
	TMap<FString, TArray<FDES_ProductionRule>> ProductionRules;
	TArray<FDES_LModule> Sentence;

	float LSeed, LRotation, LScale, LDepth;
	TArray<FDES_SeedVertex> SeedVertices;

	float Time, Intensity;
	TArray<FDES_TreeVertex> TreeVertices;

	TArray<FVector> Vertices, Indices;*/
	//UProceduralMeshComponent* LMesh;
};