#include "DES_L_System.h"

FDES_L_Module::FDES_L_Module()
{
	Letter = "";

	Period = 0.0f;
	Aperiodicity = 0.0f;
	Synchronisation = 0.0f;
	Asynchronicity = 0.0f;

	StaticLength = 0.0f;
	RandomStaticLength = 0.0f;
	PeriodicLength = 0.0f;
	RandomPeriodicLength = 0.0f;

	StaticRotation = 0.0f;
	RandomStaticRotation = 0.0f;
	PeriodicRotation = 0.0f;
	RandomPeriodicRotation = 0.0f;

	StaticWidth = 0.0f;
	RandomStaticWidth = 0.0f;
	PeriodicWidth = 0.0f;
	RandomPeriodicWidth = 0.0f;

	Asymmetry = 0.0f;
	StaticAsymmetry = 0.0f;
	RandomStaticAsymmetry = 0.0f;
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

void ADES_L_System::Initialize(UProceduralMeshComponent* Mesh, TArray<FDES_L_Module> Axiom, int Iterations, float Seed, float Rotation, FVector2D Anchoring)
{
	// STEP 1: Initialize sentence, using grammar...
	InitializeSentence(Seed, Axiom, Iterations);

	// STEP 2: Initialize tree, to scale...
	InitializeTree(Seed, Rotation, Anchoring);

	// STEP 3: Prepare vertex/index arrays, and feed these into the procedural mesh component...
	InitializeMesh(Mesh);
}

void ADES_L_System::InitializeSentence(float Seed, TArray<FDES_L_Module> Axiom, int Iterations)
{
	srand(Seed);

	Sentence = Axiom;
	for (int i = 0; i < Axiom.Num(); i++)
	{
		// NB: (Rough) initialisation...
		Sentence[i].Asymmetry = Sentence[i].StaticAsymmetry + GetRNGRange() * Sentence[i].RandomStaticAsymmetry;
	}

	TArray<FDES_L_Module> IteratedSentence;
	FDES_L_Module ProductionModule;
	for (int i = 0; i < Iterations; i++)
	{
		IteratedSentence = TArray<FDES_L_Module>();

		// FIXME: Why won't this loop compile?
		for (FDES_L_Module L_Module : Sentence)
		{
			ProductionModule = L_Module;

			if (GetProductionRule(L_Module.Letter).Productions.Num() == 0)
				continue;
			
			for (TFunction<FDES_L_Module(FDES_L_Module)> Production : GetProductionRule(L_Module.Letter).Productions)
			{
				IteratedSentence.Add(Production(L_Module));
				IteratedSentence[IteratedSentence.Num() - 1].Asymmetry = IteratedSentence[IteratedSentence.Num() - 1].StaticAsymmetry + GetRNGRange() * IteratedSentence[IteratedSentence.Num() - 1].RandomStaticAsymmetry;
			}
		}

		Sentence = IteratedSentence;
	}
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