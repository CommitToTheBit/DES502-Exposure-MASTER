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

		// FIXME: Why won't this loop compile?
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
			ParentIndex = ParentIndices.Pop();
		}
		else if (L_Module.Letter != "]")
		{
			LocalTransform *= FTransform(FRotator(0.0f, L_Module.StaticRotation, 0.0f)); // NB: Note the order of operations!

			if (L_Module.StaticLength == 0.0f)
				continue;

			LocalTransform *= FTransform(FVector(L_Module.StaticLength, 0.0f, 0.0f));

			SeedVertices.Add(FDES_SeedVertex());
			SeedVertices[ChildIndex].Parent = ParentIndex;
			SeedVertices[ChildIndex].Transform = SeedVertices[ParentIndex].Transform * LocalTransform;
			SeedVertices[ChildIndex].Position = SeedVertices[ChildIndex].Transform.TransformVector(FVector(0.0f, 0.0f, 0.0f));
			SeedVertices[ChildIndex].Depth = SeedVertices[ParentIndex].Depth + L_Module.StaticLength;

			ChildIndex = SeedVertices.Num();
			ParentIndex = ChildIndex - 1;
			LocalTransform = FTransform::Identity;

			//GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, FTransform(FVector(L_Module.StaticLength, 0.0f, 0.0f)), FVector(0.0f, 0.0f, 0.0f)).ToString());
			GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, (FTransform(FVector(L_Module.StaticLength, 0.0f, 0.0f)).TransformVectorNoScale(FVector(1.0f, 0.0f, 0.0f))).ToString());
			//GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, SeedVertices[ParentIndex].Position.ToString());
		}
	}

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

	// STEP 4: Instantaneously 'update' TreeVertices...
	UpdateTree(0.0f, 0.0f);
}
void ADES_L_System::InitializeMesh(UProceduralMeshComponent* Mesh)
{
	LMesh = Mesh;

	Vertices = TArray<FVector>();
	Vertices.Init(FVector(0.0f, 0.0f, 0.0f), 4 * TreeVertices.Num()); // NB: Use 16-gon to approximate a circle at joints?

	Indices = TArray<int32>();
	Indices.Init(0, 12 * TreeVertices.Num());

	// Initialize the index to the vertex buffer.
	int Index = 0;
	FDES_TreeVertex TreeVertex, ParentVertex;
	FVector Orthogonal;
	for (int i = 0; i < TreeVertices.Num(); i++)
	{
		TreeVertex = TreeVertices[i];
		ParentVertex = TreeVertices[TreeVertex.Parent];

		Orthogonal = FTransform(FRotator(0.0f, PI/2.0f, 0.0f)).TransformVector(TreeVertex.Position - ParentVertex.Position);
		Orthogonal.Normalize();

		Vertices[4 * i] = ParentVertex.Position + ParentVertex.Width * Orthogonal;
		Vertices[4 * i + 1] = ParentVertex.Position - ParentVertex.Width * Orthogonal;
		Vertices[4 * i + 2] = TreeVertex.Position - TreeVertex.Width * Orthogonal;
		Vertices[4 * i + 3] = TreeVertex.Position + TreeVertex.Width * Orthogonal;

		Indices[12 * i] = 4 * TreeVertex.Parent + 2;
		Indices[12 * i + 1] = 4 * TreeVertex.Parent + 3;
		Indices[12 * i + 2] = 4 * i;

		Indices[12 * i + 3] = 4 * TreeVertex.Parent + 2;
		Indices[12 * i + 4] = 4 * i;
		Indices[12 * i + 5] = 4 * i + 1;

		Indices[12 * i + 6] = 4 * i;
		Indices[12 * i + 7] = 4 * i + 1;
		Indices[12 * i + 8] = 4 * i + 2;

		Indices[12 * i + 9] = 4 * i;
		Indices[12 * i + 10] = 4 * i + 2;
		Indices[12 * i + 11] = 4 * i + 3;

		//GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Magenta, FString::FromInt(i));
	}

	/*Vertices[0] = FVector(1.0f, 1.0f, 0.0f);
	Vertices[1] = FVector(1.0f, -1.0f, 0.0f);
	Vertices[2] = FVector(-1.0f, -1.0f, 0.0f);

	Indices[0] = 0;
	Indices[1] = 1;
	Indices[2] = 2;*/

	LMesh->CreateMeshSection_LinearColor(0, Vertices, Indices, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	LMesh->SetMeshSectionVisible(0, true);
}

void ADES_L_System::Update(float DeltaTime, float DeltaIntensity)
{

}

void ADES_L_System::UpdateTree(float DeltaTime, float DeltaIntensity)
{
	FRandomStream rng = FRandomStream(LSeed);

	Time += DeltaTime;
	Intensity += DeltaIntensity;
	Intensity = std::max(0.0f, std::min(Intensity, 1.0f));

	TreeVertices = TArray<FDES_TreeVertex>();
	TreeVertices.Add(FDES_TreeVertex());
	TreeVertices[0].Parent = 0;
	TreeVertices[0].Transform = FTransform(SeedVertices[0].Position) * FTransform(FRotator(0.0f, LRotation, 0.0f)); // NB: Assumes we've initialised m_seedVertices!
	TreeVertices[0].Position = TreeVertices[0].Transform.TransformVector(FVector(0.0f, 0.0f, 0.0f));
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
			ParentIndex = ParentIndices.Pop();
		}
		else if (L_Module.Letter != "]")
		{
			Period = L_Module.Period + rng.FRandRange(0.0f, std::max(L_Module.Aperiodicity, 0.0f));
			Oscillation = (L_Module.Period > 0.0f) ? cos(2.0f * PI * (Time / Period) + (L_Module.Synchronisation + rng.FRandRange(0.0f, L_Module.Asynchronicity))) : 0.0f;
			StaticRotation = L_Module.StaticRotation + rng.FRandRange(-1.0f, 1.0f) * L_Module.RandomStaticRotation;
			PeriodicRotation = Oscillation * (L_Module.PeriodicRotation + rng.FRandRange(-1.0f, 1.0f) * L_Module.RandomPeriodicRotation);
			LocalTransform *= FTransform(FRotator(0.0f, StaticRotation + PeriodicRotation, 0.0f));

			if (L_Module.StaticLength == 0.0f)
				continue;

			Scale = (SeedVertices[ChildIndex].Depth > 0.0f) ? LScale * std::max(0.0f, std::min((1.0f - LDepth / SeedVertices[ChildIndex].Depth) + (LDepth / SeedVertices[ChildIndex].Depth) * Intensity, 1.0f)) : 0.0f;

			Period = L_Module.Period + rng.FRandRange(0.0f, std::max(L_Module.Aperiodicity, 0.0f));
			Oscillation = (L_Module.Period > 0.0f) ? cos(2.0f * PI * (Time / Period) + (L_Module.Synchronisation + rng.FRandRange(0.0f, L_Module.Asynchronicity))) : 0.0f;
			StaticLength = L_Module.StaticLength + rng.FRandRange(-1.0f, 1.0f) * L_Module.RandomStaticLength;
			PeriodicLength = Oscillation * (L_Module.PeriodicLength + rng.FRandRange(-1.0f, 1.0f) * L_Module.RandomPeriodicLength);
			LocalTransform *= FTransform(FVector(Scale * (StaticLength + PeriodicLength), 0.0f, 0.0f));

			Period = L_Module.Period + rng.FRandRange(0.0f, std::max(L_Module.Aperiodicity, 0.0f));
			Oscillation = (L_Module.Period > 0.0f) ? cos(2.0f * PI * (Time / Period) + (L_Module.Synchronisation + rng.FRandRange(0.0f, L_Module.Asynchronicity))) : 0.0f;
			StaticWidth = L_Module.StaticWidth + rng.FRandRange(-1.0f, 1.0f) * L_Module.RandomStaticWidth;
			PeriodicWidth = Oscillation * (L_Module.PeriodicWidth + rng.FRandRange(-1.0f, 1.0f) * L_Module.RandomPeriodicWidth);

			TreeVertices.Add(FDES_TreeVertex());
			TreeVertices[ChildIndex].Parent = ParentIndex;
			TreeVertices[ChildIndex].Transform = TreeVertices[ParentIndex].Transform * LocalTransform;
			TreeVertices[ChildIndex].Position = TreeVertices[ChildIndex].Transform.TransformVector(FVector(0.0f, 0.0f, 0.0f));
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
		if (Weight >= SummedWeight + ProductionRule.Weight)
		{
			SummedWeight += ProductionRule.Weight;
			Index++;
		}
		else
		{
			break;
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