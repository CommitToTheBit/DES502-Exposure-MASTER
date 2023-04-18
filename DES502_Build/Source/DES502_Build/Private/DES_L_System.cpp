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

ADES_L_System::~ADES_L_System()
{

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
	FRandomStream rng = FRandomStream(Seed);

	Sentence = Axiom;
	for (int i = 0; i < Axiom.Num(); i++)
	{
		// NB: (Rough) initialisation...
		Sentence[i].Asymmetry = Sentence[i].StaticAsymmetry + rng.FRandRange(-1.0, 1.0) * Sentence[i].RandomStaticAsymmetry;
	}

	TArray<FDES_L_Module> IteratedSentence;
	for (int i = 0; i < Iterations; i++)
	{
		IteratedSentence = TArray<FDES_L_Module>();
		for (FDES_L_Module L_Module : Sentence)
		{
			for (TFunction<FDES_L_Module(FDES_L_Module)> Production : GetProductionRule(L_Module.Letter, &rng).Productions)
			{
				IteratedSentence.Add(Production(L_Module));
				IteratedSentence[IteratedSentence.Num() - 1].Asymmetry = IteratedSentence[IteratedSentence.Num() - 1].StaticAsymmetry + rng.FRandRange(-1.0, 1.0) * IteratedSentence[IteratedSentence.Num() - 1].RandomStaticAsymmetry;
			}
		}
		Sentence = IteratedSentence;
	}
}

void ADES_L_System::InitializeTree(float Seed, float Rotation, FVector2D Anchoring)
{
	LSeed = Seed;
	LRotation = Rotation;
	LScale = 1.0f;
	LDepth = 0.0f;

	Time = 0.0f;
	Intensity = 0.0f;

	SeedVertices = TArray<FDES_SeedVertex>();
	SeedVertices.Add(FDES_SeedVertex());
	SeedVertices[0].Parent = 0;
	SeedVertices[0].Transform = FTransform(FRotator(0.0f, Rotation, 0.0f));
	SeedVertices[0].Position = FVector(0.0f, 0.0f, 0.0f);
	SeedVertices[0].Depth = 0.0f;

	int ParentIndex = 0;
	TArray<int> ParentIndices = TArray<int>();

	FTransform LocalTransform = FTransform::Identity;

	// STEP 1: Create branching structure...
	int ChildIndex = 1;
	for (FDES_L_Module L_Module : Sentence)
	{
		if (L_Module.Letter == "[")
		{
			ParentIndices.Add(ParentIndex);
		}
		else if (L_Module.Letter == "]" && ParentIndices.Num() > 0)
		{
			ParentIndex = ParentIndices[ParentIndices.Num() - 1];
			ParentIndices.RemoveAt(ParentIndices.Num() - 1);
		}
		else if (L_Module.Letter != "]")
		{
			LocalTransform = FTransform(FRotator(0.0f, L_Module.StaticRotation, 0.0f)) * LocalTransform; // NB: Note the order of operations!

			if (L_Module.StaticLength == 0.0f)
				continue;

			LocalTransform = FTransform(FVector(L_Module.StaticLength, 0.0f, 0.0f)) * LocalTransform;

			SeedVertices.Add(FDES_SeedVertex());
			SeedVertices[ChildIndex].Parent = ParentIndex;
			SeedVertices[ChildIndex].Transform = LocalTransform * SeedVertices[ParentIndex].Transform ;
			SeedVertices[ChildIndex].Position = SeedVertices[ChildIndex].Transform.TransformPosition(FVector(0.0f, 0.0f, 0.0f));
			SeedVertices[ChildIndex].Depth = SeedVertices[ParentIndex].Depth + L_Module.StaticLength;

			ChildIndex = SeedVertices.Num();
			ParentIndex = ChildIndex - 1;
			LocalTransform = FTransform::Identity;

			//GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, FTransform(FVector(L_Module.StaticLength, 0.0f, 0.0f)), FVector(0.0f, 0.0f, 0.0f)).ToString());
			//GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, SeedVertices[ParentIndex].Position.ToString());
			//GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, (FTransform(FVector(L_Module.StaticLength, 0.0f, 0.0f)).TransformPosition(FVector(0.0f, 0.0f, 0.0f))).ToString());
			//GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, SeedVertices[ParentIndex].Position.ToString());
		}
	}

	// DEBUG:
	/*FString Positions = SeedVertices[0].Transform.ToString() + "\n";
	Positions += (SeedVertices[0].Transform * FTransform(FVector(1.0f, 0.0f, 0.0f))).ToString() + "\n";
	Positions += (FTransform(FVector(1.0f, 0.0f, 0.0f)) * SeedVertices[0].Transform).ToString() + "\n";
	Positions += SeedVertices[0].Transform.TransformPosition(FVector(1.0f, 0.0f, 0.0f)).ToString() + "\n";
	for (FDES_SeedVertex SeedVertex : SeedVertices)
	{
		Positions += SeedVertex.Position.ToString() + "\n";
	}
	GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, Positions);*/

	// STEP 2: Calculate bounds...
	FVector2D Minima = FVector2D(0.0f, 0.0f);
	FVector2D Maxima = FVector2D(0.0f, 0.0f);
	for (int i = 0; i < SeedVertices.Num(); i++)
	{
		if (SeedVertices[i].Position.X < Minima.X)
			Minima.X = SeedVertices[i].Position.X;
		else if (SeedVertices[i].Position.X > Maxima.X)
			Maxima.X = SeedVertices[i].Position.X;

		if (SeedVertices[i].Position.Y < Minima.Y)
			Minima.Y = SeedVertices[i].Position.Y;
		else if (SeedVertices[i].Position.Y > Maxima.Y)
			Maxima.Y = SeedVertices[i].Position.Y;
	}

	FVector2D Delta = Maxima - Minima;
	float MaxDelta = std::max(Delta.X, Delta.Y);
	if (MaxDelta > 0.0f) // NB: In the case of a point, scaling won't matter!
		LScale /= MaxDelta;

	// STEP 3: Rescale... 
	float XBorder = Anchoring.X * (1.0f - Delta.X / MaxDelta);
	float YBorder = Anchoring.Y * (1.0f - Delta.Y / MaxDelta);

	for (int i = 0; i < SeedVertices.Num(); i++)
	{
		SeedVertices[i].Position.X = XBorder + (SeedVertices[i].Position.X - Minima.X) / MaxDelta;
		SeedVertices[i].Position.Y = YBorder + (SeedVertices[i].Position.Y - Minima.Y) / MaxDelta;

		SeedVertices[i].Depth *= LScale;
		LDepth = std::max(SeedVertices[i].Depth, LDepth);
	}

	// DEBUG:
	FString Positions = FString::SanitizeFloat(LScale)+"\n";
	for (FDES_SeedVertex SeedVertex : SeedVertices)
	{
		Positions += SeedVertex.Position.ToString() + "\n";
	}
	GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, Positions);

	// STEP 4: Instantaneously 'update' TreeVertices...
	UpdateTree(0.0f, 0.0f);
}
void ADES_L_System::InitializeMesh(UProceduralMeshComponent* Mesh)
{
	LMesh = Mesh;

	Vertices = TArray<FVector>();
	Vertices.Init(FVector(0.0f, 0.0f, 0.0f), 4 * TreeVertices.Num()); // NB: Use 16-gon to approximate a circle at joints?

	Indices = TArray<int32>();
	Indices.Init(0, 24 * TreeVertices.Num());

	// Initialize the index to the vertex buffer.
	FDES_TreeVertex TreeVertex, ParentVertex;
	FVector Orthogonal;
	for (int i = 0; i < TreeVertices.Num(); i++)
	{
		TreeVertex = TreeVertices[i];
		ParentVertex = TreeVertices[TreeVertex.Parent];

		Orthogonal = FTransform(FRotator(0.0f, 90.0f, 0.0f)).TransformPosition(TreeVertex.Position - ParentVertex.Position);
		Orthogonal.Normalize();

		Vertices[4 * i] = ParentVertex.Position + ParentVertex.Width * Orthogonal;
		Vertices[4 * i + 1] = ParentVertex.Position - ParentVertex.Width * Orthogonal;
		Vertices[4 * i + 2] = TreeVertex.Position - TreeVertex.Width * Orthogonal;
		Vertices[4 * i + 3] = TreeVertex.Position + TreeVertex.Width * Orthogonal;

		for (int j = 0; j < 2; j++)
		{
			Indices[24 * i + 12 * j] = 4 * TreeVertex.Parent + 2;
			Indices[24 * i + 12 * j + 1 + j] = 4 * TreeVertex.Parent + 3;
			Indices[24 * i + 12 * j + 2 - j] = 4 * i;

			Indices[24 * i + 12 * j + 3] = 4 * TreeVertex.Parent + 2;
			Indices[24 * i + 12 * j + 4 + j] = 4 * i;
			Indices[24 * i + 12 * j + 5 - j] = 4 * i + 1;

			Indices[24 * i + 12 * j + 6] = 4 * i;
			Indices[24 * i + 12 * j + 7 + j] = 4 * i + 1;
			Indices[24 * i + 12 * j + 8 - j] = 4 * i + 2;

			Indices[24 * i + 12 * j + 9] = 4 * i;
			Indices[24 * i + 12 * j + 10 + j] = 4 * i + 2;
			Indices[24 * i + 12 * j + 11 - j] = 4 * i + 3;
		}
	}

	LMesh->CreateMeshSection_LinearColor(0, Vertices, Indices, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	LMesh->SetMeshSectionVisible(0, true);
}

void ADES_L_System::Update(float DeltaTime, float DeltaIntensity)
{
	UpdateTree(DeltaTime, DeltaIntensity);

	InitializeMesh(LMesh);
}

void ADES_L_System::UpdateTree(float DeltaTime, float DeltaIntensity)
{
	FRandomStream rng = FRandomStream(LSeed);

	Time += DeltaTime;
	//Intensity += DeltaIntensity;
	//Intensity = std::max(0.0f, std::min(Intensity, 1.0f));

	// DEBUG: 
	Intensity = 0.5f + 0.5f * cos(0.08 * (2.0f * PI) * Time);

	TreeVertices = TArray<FDES_TreeVertex>();
	TreeVertices.Add(FDES_TreeVertex());
	TreeVertices[0].Parent = 0;
	TreeVertices[0].Transform = FTransform(FRotator(0.0f, LRotation, 0.0f)) * FTransform(SeedVertices[0].Position); // NB: Assumes we've initialised m_seedVertices!
	TreeVertices[0].Position = TreeVertices[0].Transform.TransformPosition(FVector(0.0f, 0.0f, 0.0f));
	TreeVertices[0].Width = 0.0f;
	TreeVertices[0].Degree = 0;

	int ParentIndex = 0;
	TArray<int> ParentIndices = TArray<int>();

	FTransform LocalTransform = FTransform::Identity;

	// STEP 1: Create branching structure...
	int ChildIndex = 1;
	float StaticLength, PeriodicLength, StaticRotation, PeriodicRotation, StaticWidth, PeriodicWidth, Period, Oscillation, Scale;
	for (FDES_L_Module L_Module : Sentence)
	{
		if (L_Module.Letter == "[")
		{
			ParentIndices.Add(ParentIndex);
		}
		else if (L_Module.Letter == "]" && ParentIndices.Num() > 0)
		{
			ParentIndex = ParentIndices[ParentIndices.Num() - 1];
			ParentIndices.RemoveAt(ParentIndices.Num() - 1);
		}
		else if (L_Module.Letter != "]")
		{
			Period = L_Module.Period + rng.FRandRange(0.0f, std::max(L_Module.Aperiodicity, 0.0f));
			Oscillation = (L_Module.Period > 0.0f) ? cos(2.0f * PI * (Time / Period) + (L_Module.Synchronisation + rng.FRandRange(0.0f, L_Module.Asynchronicity))) : 0.0f;
			StaticRotation = L_Module.StaticRotation + rng.FRandRange(-1.0f, 1.0f) * L_Module.RandomStaticRotation;
			PeriodicRotation = Oscillation * (L_Module.PeriodicRotation + rng.FRandRange(-1.0f, 1.0f) * L_Module.RandomPeriodicRotation);
			LocalTransform = FTransform(FRotator(0.0f, StaticRotation + PeriodicRotation, 0.0f)) * LocalTransform;

			if (L_Module.StaticLength == 0.0f)
				continue;

			Scale = (SeedVertices[ChildIndex].Depth > 0.0f) ? LScale * std::max(0.0f, std::min(/*(1.0f - LDepth / (LDepth - SeedVertices[ChildIndex].Depth) + */ (LDepth / SeedVertices[ChildIndex].Depth) * Intensity, 1.0f)) : 0.0f;

			// DEBUG:
			GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, FString::SanitizeFloat(LDepth / SeedVertices[ChildIndex].Depth));

			Period = L_Module.Period + rng.FRandRange(0.0f, std::max(L_Module.Aperiodicity, 0.0f));
			Oscillation = (L_Module.Period > 0.0f) ? cos(2.0f * PI * (Time / Period) + (L_Module.Synchronisation + rng.FRandRange(0.0f, L_Module.Asynchronicity))) : 0.0f;
			StaticLength = L_Module.StaticLength + rng.FRandRange(-1.0f, 1.0f) * L_Module.RandomStaticLength;
			PeriodicLength = Oscillation * (L_Module.PeriodicLength + rng.FRandRange(-1.0f, 1.0f) * L_Module.RandomPeriodicLength);
			LocalTransform = FTransform(FVector(Scale * (StaticLength + PeriodicLength), 0.0f, 0.0f)) * LocalTransform;

			Period = L_Module.Period + rng.FRandRange(0.0f, std::max(L_Module.Aperiodicity, 0.0f));
			Oscillation = (L_Module.Period > 0.0f) ? cos(2.0f * PI * (Time / Period) + (L_Module.Synchronisation + rng.FRandRange(0.0f, L_Module.Asynchronicity))) : 0.0f;
			StaticWidth = L_Module.StaticWidth + rng.FRandRange(-1.0f, 1.0f) * L_Module.RandomStaticWidth;
			PeriodicWidth = Oscillation * (L_Module.PeriodicWidth + rng.FRandRange(-1.0f, 1.0f) * L_Module.RandomPeriodicWidth);

			TreeVertices.Add(FDES_TreeVertex());
			TreeVertices[ChildIndex].Parent = ParentIndex;
			TreeVertices[ChildIndex].Transform = LocalTransform * TreeVertices[ParentIndex].Transform;
			TreeVertices[ChildIndex].Position = TreeVertices[ChildIndex].Transform.TransformPosition(FVector(0.0f, 0.0f, 0.0f));
			TreeVertices[ChildIndex].Width = Scale * (StaticWidth + PeriodicWidth);
			TreeVertices[ChildIndex].Degree = 1;

			TreeVertices[ParentIndex].Width = std::max(TreeVertices[ChildIndex].Width, TreeVertices[ParentIndex].Width);
			TreeVertices[ParentIndex].Degree++;

			ChildIndex = TreeVertices.Num();
			ParentIndex = ChildIndex - 1;
			LocalTransform = FTransform::Identity;
		}
	}
}

void ADES_L_System::AddProductionRule(FString Letter, FDES_ProductionRule ProductionRule)
{
	if (!ProductionRules.Contains(Letter))
	{
		ProductionRules.Add(TPair<FString, TArray<FDES_ProductionRule>>(Letter, TArray<FDES_ProductionRule>{ ProductionRule }));
	}
	else
	{
		ProductionRules[Letter].Add(ProductionRule);
	}
}

FDES_ProductionRule ADES_L_System::GetProductionRule(FString Letter, FRandomStream* rng)
{
	// If no production rules exists, treat the letter as a terminal...
	if (!ProductionRules.Contains(Letter))
	{
		FDES_ProductionRule Identity;
		Identity.Productions.Add([this](FDES_L_Module L_Module) { return L_Module; });
		Identity.Weight = 1.0f;
		return Identity;
	}

	// If production rules exist, choose one at random...
	float TotalWeight = 0.0f;
	for (FDES_ProductionRule ProductionRule : ProductionRules[Letter])
		TotalWeight += ProductionRule.Weight;

	int Index = 0;
	float Weight = rng->FRandRange(0.0f, TotalWeight);
	float SummedWeight = 0.0f;
	for (FDES_ProductionRule ProductionRule : ProductionRules[Letter])
	{
		if (Weight < SummedWeight + ProductionRule.Weight || Index == ProductionRules[Letter].Num() - 1)
		{
			break;
		}
		else
		{
			SummedWeight += ProductionRule.Weight;
			Index++;
		}
	}

	return ProductionRules[Letter][Index];
}

FString ADES_L_System::GetSentence()
{
	FString Text = "";
	for (FDES_L_Module L_Module : Sentence)
		Text += L_Module.Letter;

	return Text;
}