// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Chunk.h"
#include "DrawDebugHelpers.h"
#include "utils/Utils.h"

// Sets default values
AChunk::AChunk()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BlocksMeshes = CreateDefaultSubobject<UProceduralMeshComponent>("Blocks");

	RootComponent = BlocksMeshes;

	ChunkSize = 500;
	Snap = 200.f;

	ChunkHeight = 45;
}


// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();
}

void AChunk::GenerateChunk(UFastNoiseWrapper* NoiseGenerator)
{
	GenerateBaseTerrain(NoiseGenerator);
	UpdateMesh();
}

void AChunk::UpdateMesh()
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector2D> UV0;
	TArray<FVector> Normals;
	TArray<FColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	for (const TPair<int32, TMap<int32, TMap<int32, EBlockType>>> XMap : Blocks)
	{
		for (const TPair<int32, TMap<int32, EBlockType>> YMap : XMap.Value)
		{
			for (const TPair<int32, EBlockType> ZMap : YMap.Value)
			{
				FBlockTextureData TextureData = BlockTextureManager->GetBlock(ZMap.Value);
				if (ShouldBuild(XMap.Key, YMap.Key, ZMap.Key))
				{
					bool XBottomYBottomZBottom_0 = false;
					bool XBottomYBottomZTop_1 = false;
					bool XBottomYTopZBottom_2 = false;
					bool XBottomYTopZTop_3 = false;
					bool XTopYBottomZBottom_4 = false;
					bool XTopYBottomZTop_5 = false;
					bool XTopYTopZTop_6 = false;
					bool XTopYTopZBottom_7 = false;
					if (TextureData.IsTransparent || !BuildingExist(XMap.Key, YMap.Key, ZMap.Key + Snap))
					{
						XTopYBottomZTop_5 = true;
						XTopYTopZTop_6 = true;
						XBottomYBottomZTop_1 = true;
						XBottomYTopZTop_3 = true;
					}
					if (TextureData.IsTransparent || !BuildingExist(XMap.Key, YMap.Key, ZMap.Key - Snap))
					{
						XTopYBottomZBottom_4 = true;
						XTopYTopZBottom_7 = true;
						XBottomYBottomZBottom_0 = true;
						XBottomYTopZBottom_2 = true;
					}
					if (TextureData.IsTransparent || !BuildingExist(XMap.Key, YMap.Key + Snap, ZMap.Key))
					{
						XTopYTopZTop_6 = true;
						XTopYTopZBottom_7 = true;
						XBottomYTopZBottom_2 = true;
						XBottomYTopZTop_3 = true;
					}
					if (TextureData.IsTransparent || !BuildingExist(XMap.Key, YMap.Key - Snap, ZMap.Key))
					{
						XTopYBottomZTop_5 = true;
						XTopYBottomZBottom_4 = true;
						XBottomYBottomZBottom_0 = true;
						XBottomYBottomZTop_1 = true;
					}
					if (TextureData.IsTransparent || !BuildingExist(XMap.Key + Snap, YMap.Key, ZMap.Key))
					{
						XTopYBottomZBottom_4 = true;
						XTopYTopZBottom_7 = true;
						XTopYBottomZTop_5 = true;
						XTopYTopZTop_6 = true;
					}
					if (TextureData.IsTransparent || !BuildingExist(XMap.Key - Snap, YMap.Key, ZMap.Key))
					{
						XBottomYBottomZBottom_0 = true;
						XBottomYTopZBottom_2 = true;
						XBottomYBottomZTop_1 = true;
						XBottomYTopZTop_3 = true;
					}
					if (XBottomYBottomZBottom_0 && XBottomYTopZBottom_2 && XBottomYTopZTop_3 && XBottomYBottomZTop_1)
					{
						int XBottomYBottomZBottomVector_0 = Vertices.Add(
							FVector(XMap.Key, YMap.Key, ZMap.Key));
						int XBottomYTopZBottomVector_2 = Vertices.Add(
							FVector(XMap.Key, YMap.Key + Snap, ZMap.Key));
						int XBottomYBottomZTopVector_1 = Vertices.Add(
							FVector(XMap.Key, YMap.Key, ZMap.Key + Snap));
						int XBottomYTopZTopVector_3 = Vertices.Add(
							FVector(XMap.Key, YMap.Key + Snap, ZMap.Key + Snap));
						Triangles.Add(XBottomYBottomZBottomVector_0);
						Triangles.Add(XBottomYTopZBottomVector_2);
						Triangles.Add(XBottomYTopZTopVector_3);
						Triangles.Add(XBottomYTopZTopVector_3);
						Triangles.Add(XBottomYBottomZTopVector_1);
						Triangles.Add(XBottomYBottomZBottomVector_0);
						UV0.Append(TextureData.AroundBlocksUV);
					}
					if (XTopYBottomZBottom_4 && XBottomYBottomZBottom_0 && XBottomYBottomZTop_1 && XTopYBottomZTop_5)
					{
						int XBottomYBottomZBottomVector_0 = Vertices.Add(
							FVector(XMap.Key, YMap.Key, ZMap.Key));
						int XTopYBottomZBottomVector_4 = Vertices.Add(
							FVector(XMap.Key + Snap, YMap.Key, ZMap.Key));
						int XBottomYBottomZTopVector_1 = Vertices.Add(
							FVector(XMap.Key, YMap.Key, ZMap.Key + Snap));
						int XTopYBottomZTopVector_5 = Vertices.Add(
							FVector(XMap.Key + Snap, YMap.Key, ZMap.Key + Snap));
						Triangles.Add(XBottomYBottomZBottomVector_0);
						Triangles.Add(XBottomYBottomZTopVector_1);
						Triangles.Add(XTopYBottomZBottomVector_4);
						Triangles.Add(XTopYBottomZBottomVector_4);
						Triangles.Add(XBottomYBottomZTopVector_1);
						Triangles.Add(XTopYBottomZTopVector_5);
						UV0.Append(TextureData.AroundBlocksUV);
					}
					if (XTopYBottomZBottom_4 && XTopYBottomZTop_5 && XTopYTopZBottom_7 && XTopYTopZTop_6)
					{
						int XTopYBottomZBottomVector_4 = Vertices.Add(
							FVector(XMap.Key + Snap, YMap.Key, ZMap.Key));
						int XTopYTopZBottomVector_7 = Vertices.Add(
							FVector(XMap.Key + Snap, YMap.Key + Snap, ZMap.Key));
						int XTopYBottomZTopVector_5 = Vertices.Add(
							FVector(XMap.Key + Snap, YMap.Key, ZMap.Key + Snap));
						int XTopYTopZTopVector_6 = Vertices.Add(
							FVector(XMap.Key + Snap, YMap.Key + Snap, ZMap.Key + Snap));
						Triangles.Add(XTopYBottomZBottomVector_4);
						Triangles.Add(XTopYBottomZTopVector_5);
						Triangles.Add(XTopYTopZBottomVector_7);
						Triangles.Add(XTopYTopZBottomVector_7);
						Triangles.Add(XTopYBottomZTopVector_5);
						Triangles.Add(XTopYTopZTopVector_6);
						UV0.Append(TextureData.AroundBlocksUV);
					}
					if (XTopYTopZBottom_7 && XTopYTopZTop_6 && XBottomYTopZBottom_2 && XBottomYTopZTop_3)
					{
						int XBottomYTopZBottomVector_2 = Vertices.Add(
							FVector(XMap.Key, YMap.Key + Snap, ZMap.Key));
						int XTopYTopZBottomVector_7 = Vertices.Add(
							FVector(XMap.Key + Snap, YMap.Key + Snap, ZMap.Key));
						Triangles.Add(XTopYTopZBottomVector_7);
						int XBottomYTopZTopVector_3 = Vertices.Add(
							FVector(XMap.Key, YMap.Key + Snap, ZMap.Key + Snap));
						int XTopYTopZTopVector_6 = Vertices.Add(
							FVector(XMap.Key + Snap, YMap.Key + Snap, ZMap.Key + Snap));
						Triangles.Add(XTopYTopZTopVector_6);
						Triangles.Add(XBottomYTopZTopVector_3);
						Triangles.Add(XBottomYTopZTopVector_3);
						Triangles.Add(XBottomYTopZBottomVector_2);
						Triangles.Add(XTopYTopZBottomVector_7);
						UV0.Append(TextureData.AroundBlocksUV);
					}
					if (XBottomYTopZTop_3 && XTopYTopZTop_6 && XTopYBottomZTop_5 && XBottomYBottomZTop_1)
					{
						int XBottomYBottomZTopVector_1 = Vertices.Add(
							FVector(XMap.Key, YMap.Key, ZMap.Key + Snap));
						int XBottomYTopZTopVector_3 = Vertices.Add(
							FVector(XMap.Key, YMap.Key + Snap, ZMap.Key + Snap));
						int XTopYBottomZTopVector_5 = Vertices.Add(
							FVector(XMap.Key + Snap, YMap.Key, ZMap.Key + Snap));
						int XTopYTopZTopVector_6 = Vertices.Add(
							FVector(XMap.Key + Snap, YMap.Key + Snap, ZMap.Key + Snap));
						Triangles.Add(XBottomYBottomZTopVector_1);
						Triangles.Add(XBottomYTopZTopVector_3);
						Triangles.Add(XTopYBottomZTopVector_5);
						Triangles.Add(XTopYTopZTopVector_6);
						Triangles.Add(XTopYBottomZTopVector_5);
						Triangles.Add(XBottomYTopZTopVector_3);
						UV0.Append(TextureData.TopFaceUV);
					}
					if (XTopYTopZBottom_7 && XBottomYTopZBottom_2 && XBottomYBottomZBottom_0 && XTopYBottomZBottom_4)
					{
						int XBottomYBottomZBottomVector_0 = Vertices.Add(
							FVector(XMap.Key, YMap.Key, ZMap.Key));
						int XBottomYTopZBottomVector_2 = Vertices.Add(
							FVector(XMap.Key, YMap.Key + Snap, ZMap.Key));
						int XTopYBottomZBottomVector_4 = Vertices.Add(
							FVector(XMap.Key + Snap, YMap.Key, ZMap.Key));
						int XTopYTopZBottomVector_7 = Vertices.Add(
							FVector(XMap.Key + Snap, YMap.Key + Snap, ZMap.Key));
						Triangles.Add(XBottomYTopZBottomVector_2);
						Triangles.Add(XBottomYBottomZBottomVector_0);
						Triangles.Add(XTopYBottomZBottomVector_4);
						Triangles.Add(XTopYBottomZBottomVector_4);
						Triangles.Add(XTopYTopZBottomVector_7);
						Triangles.Add(XBottomYTopZBottomVector_2);
						UV0.Append(TextureData.BottomUV);
					}
				}
			}
		}
	}
	BlocksMeshes->CreateMeshSection(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
	BlocksMeshes->SetMaterial(0, BlocksMaterial);
}

void AChunk::PlaceBlock(int32 X, int32 Y, int32 Z, EBlockType Block, FVector Forwards)
{
	FVector BlockLocation = FVector(X, Y, Z) - GetActorLocation();
	const int SnapX = Utils::SnapValue(BlockLocation.X, Snap);
	const int SnapY = Utils::SnapValue(BlockLocation.Y, Snap);
	const int SnapZ = Utils::SnapValue(BlockLocation.Z, Snap);
	PlaceBuilding(SnapX, SnapY, SnapZ, Block);
	UpdateMesh();
}

EBlockType AChunk::DestroyBlock(int32 X, int32 Y, int32 Z, FVector Forwards)
{
	EBlockType Block = EBlockType::Grass;
	FVector BlockLocation = FVector(X, Y, Z) - GetActorLocation();
	if (X % static_cast<int>(Snap) == 0)
	{
		if (Forwards.X > 0)
		{
			BlockLocation.X += Snap / 2;
		}
		else
		{
			BlockLocation.X -= Snap / 2;
		}
	}
	if (Y % static_cast<int>(Snap) == 0)
	{
		if (Forwards.Y > 0)
		{
			BlockLocation.Y += Snap / 2;
		}
		else
		{
			BlockLocation.Y -= Snap / 2;
		}
	}
	if (Z % static_cast<int>(Snap) == 0)
	{
		if (Forwards.Z > 0)
		{
			BlockLocation.Z += Snap / 2;
		}
		else
		{
			BlockLocation.Z -= Snap / 2;
		}
	}
	DrawDebugSphere(GetWorld(), FVector(Utils::SnapValue(BlockLocation.X + GetActorLocation().X, Snap),
	                                    Utils::SnapValue(BlockLocation.Y + GetActorLocation().Y, Snap),
	                                    Utils::SnapValue(BlockLocation.Z + GetActorLocation().Z, Snap)), 25.f, 10,
	                FColor(255, 0, 0), true);
	DrawDebugSphere(GetWorld(), BlockLocation + GetActorLocation(), 25.f, 10, FColor(255, 255, 0), true);
	const int SnapX = Utils::SnapValue(BlockLocation.X, Snap);
	const int SnapY = Utils::SnapValue(BlockLocation.Y, Snap);
	const int SnapZ = Utils::SnapValue(BlockLocation.Z, Snap);
	if (BuildingExist(SnapX, SnapY, SnapZ))
	{
		Block = *Blocks.Find(SnapX)->Find(SnapY)->Find(SnapZ);
		Blocks.Find(SnapX)->Find(SnapY)->Remove(SnapZ);
		Blocks.Find(SnapX)->Find(SnapY)->Shrink();
		UpdateMesh();
	}
	return Block;
}

void AChunk::GenerateBaseTerrain(UFastNoiseWrapper* NoiseGenerator)
{
	// fill map with locations for blocks
	const int StartXAxis = Utils::SnapValue(GetActorLocation().X, Snap) / Snap;
	const int StartYAxis = Utils::SnapValue(GetActorLocation().Y, Snap) / Snap;

	UE_LOG(LogTemp, Warning, TEXT("x=%d, %f"), StartXAxis, GetActorLocation().X);
	UE_LOG(LogTemp, Warning, TEXT("y=%d, %f"), StartYAxis, GetActorLocation().Y);
	for (int XAxis = 0; XAxis < ChunkSize; XAxis++)
	{
		for (int YAxis = 0; YAxis < ChunkSize; YAxis++)
		{
			const int Noise2d = FMath::Min(
				FMath::Max(
					static_cast<float>(
						Utils::SnapValue(
							NoiseGenerator->GetNoise2D(XAxis + StartXAxis, YAxis + StartYAxis) * Snap * Snap, Snap
						)
					),
					-Snap * ChunkHeight
				),
				Snap * ChunkHeight);
			EBlockType Block = BlockSpawnManager->GetBlockByHeight(
				NoiseGenerator->GetNoise3D(XAxis + StartXAxis, YAxis + StartYAxis, Noise2d / Snap), Noise2d / Snap);
			FVector BlockLocation = FVector(XAxis * Snap, YAxis * Snap, Noise2d);
			if (!BuildingExist(BlockLocation.X, BlockLocation.Y, BlockLocation.Z))
			{
				PlaceBuilding(BlockLocation.X, BlockLocation.Y, BlockLocation.Z, Block);
			}
			while (BlockLocation.Z > -Snap * ChunkHeight)
			{
				Block = BlockSpawnManager->GetBlockByHeight(
					NoiseGenerator->GetNoise3D(XAxis + StartXAxis, YAxis + StartYAxis, BlockLocation.Z / Snap),
					BlockLocation.Z / Snap);
				BlockLocation.Z -= Snap;
				if (!BuildingExist(BlockLocation.X, BlockLocation.Y, BlockLocation.Z))
				{
					PlaceBuilding(BlockLocation.X, BlockLocation.Y, BlockLocation.Z, Block);
				}
			}
		}
	}
}

bool AChunk::BuildingExist(int32 X, int32 Y, int32 Z)
{
	if (Blocks.Contains(X))
	{
		if (Blocks.Find(X)->Contains(Y))
		{
			if (Blocks.Find(X)->Find(Y)->Contains(Z))
			{
				return true;
			}
		}
	}
	return false;
}

bool AChunk::ShouldBuild(int32 X, int32 Y, int32 Z)
{
	return !(BuildingExist(X + Snap, Y, Z) && BuildingExist(X, Y + Snap, Z) && BuildingExist(X, Y, Z + Snap) &&
		BuildingExist(X - Snap, Y, Z) && BuildingExist(X, Y - Snap, Z) && BuildingExist(X, Y, Z - Snap));
}

void AChunk::PlaceBuilding(int32 X, int32 Y, int32 Z, EBlockType Value)
{
	if (Blocks.Contains(X))
	{
		if (Blocks.Find(X)->Contains(Y))
		{
			if (!Blocks.Find(X)->Find(Y)->Contains(Z))
			{
				TMap<int32, EBlockType>* ZMap = Blocks.Find(X)->Find(Y);
				ZMap->Add(Z, Value);
			}
		}
		else
		{
			TMap<int32, EBlockType> ZMap = TMap<int32, EBlockType>();
			ZMap.Add(Z, Value);
			TMap<int32, TMap<int32, EBlockType>>* YMap = Blocks.Find(X);
			YMap->Add(Y, ZMap);
		}
	}
	else
	{
		TMap<int32, EBlockType> ZMap = TMap<int32, EBlockType>();
		ZMap.Add(Z, Value);
		TMap<int32, TMap<int32, EBlockType>> YMap = TMap<int32, TMap<int32, EBlockType>>();
		YMap.Add(Y, ZMap);
		Blocks.Add(X, YMap);
	}
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
