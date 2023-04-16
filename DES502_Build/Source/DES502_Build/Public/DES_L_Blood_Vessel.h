#pragma once

#include "CoreMinimal.h"
#include "DES_L_System.h"
#include "DES_L_Blood_Vessel.generated.h"

UCLASS()
class DES502_BUILD_API ADES_L_Blood_Vessel : public ADES_L_System
{
	GENERATED_BODY()
	
public:
	ADES_L_Blood_Vessel();
	~ADES_L_Blood_Vessel();

	virtual void Tick(float DeltaTime) override;

	void Initialize(UProceduralMeshComponent* Mesh, float Width, int Iterations, float Seed = 0.0f);

protected:
	virtual void BeginPlay() override;
};
