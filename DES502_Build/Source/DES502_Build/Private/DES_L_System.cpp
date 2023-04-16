#include "DES_L_System.h"

FDES_LModule::FDES_LModule()
{

}

ADES_L_System::ADES_L_System()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADES_L_System::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADES_L_System::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADES_L_System::Initialize(/*UProceduralMeshComponent* Mesh,*/ TArray<FDES_LModule> Axiom, int Iterations, float Seed, float Rotation, FVector2D Anchoring)
{

}

void ADES_L_System::InitializeSentence(float Seed, TArray<FDES_LModule> Axiom, int Iterations)
{

}

void ADES_L_System::InitializeTree(float Seed, float Rotation, FVector2D Anchoring)
{

}
void ADES_L_System::InitializeMesh(UProceduralMeshComponent* Mesh)
{

}

void ADES_L_System::Update(float DeltaTime, float DeltaIntensity)
{

}

void ADES_L_System::UpdateTree(float DeltaTime, float DeltaIntensity)
{

}

void ADES_L_System::AddProductionRule(FString Letter, FDES_ProductionRule ProductionRule)
{

}

FDES_ProductionRule ADES_L_System::GetProductionRule(FString Letter)
{
	return FDES_ProductionRule();
}

float ADES_L_System::GetRNGRange(float A, float B)
{
	return 0.0f;
}