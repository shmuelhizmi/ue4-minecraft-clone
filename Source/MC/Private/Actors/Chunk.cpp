// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/Chunk.h"

#include <algorithm>

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
	TArray<FVector2D> UV1;
	TArray<FVector2D> UV2;
	TArray<FVector2D> UV3;
	TArray<FVector> Normals;
	TArray<FColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	for (const TPair<int32, TMap<int32, TMap<int32, EBlockType>>> XMap : Blocks)
	{
		for (const TPair<int32, TMap<int32, EBlockType>> YMap : XMap.Value)
		{
			for (const TPair<int32, EBlockType> ZMap : YMap.Value)
			{
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
					if (!BuildingExist(XMap.Key, YMap.Key, ZMap.Key + Snap))
					{
						XTopYBottomZTop_5 = true;
						XTopYTopZTop_6 = true;
						XBottomYBottomZTop_1 = true;
						XBottomYTopZTop_3 = true;
					}
					if (!BuildingExist(XMap.Key, YMap.Key, ZMap.Key - Snap))
					{
						XTopYBottomZBottom_4 = true;
						XTopYTopZBottom_7 = true;
						XBottomYBottomZBottom_0 = true;
						XBottomYTopZBottom_2 = true;
					}
					if (!BuildingExist(XMap.Key, YMap.Key + Snap, ZMap.Key))
					{
						XTopYTopZTop_6 = true;
						XTopYTopZBottom_7 = true;
						XBottomYTopZBottom_2 = true;
						XBottomYTopZTop_3 = true;
					}
					if (!BuildingExist(XMap.Key, YMap.Key - Snap, ZMap.Key))
					{
						XTopYBottomZTop_5 = true;
						XTopYBottomZBottom_4 = true;
						XBottomYBottomZBottom_0 = true;
						XBottomYBottomZTop_1 = true;
					}
					if (!BuildingExist(XMap.Key + Snap, YMap.Key, ZMap.Key))
					{
						XTopYBottomZBottom_4 = true;
						XTopYTopZBottom_7 = true;
						XTopYBottomZTop_5 = true;
						XTopYTopZTop_6 = true;
					}
					if (!BuildingExist(XMap.Key - Snap, YMap.Key, ZMap.Key))
					{
						XBottomYBottomZBottom_0 = true;
						XBottomYTopZBottom_2 = true;
						XBottomYBottomZTop_1 = true;
						XBottomYTopZTop_3 = true;
					}
					auto XOffset = Utils::SnapValue(GetActorLocation().X, Snap);
					auto YOffset = Utils::SnapValue(GetActorLocation().Y, Snap);
					int32 XBottomYBottomZBottomVector_0;
					if (XBottomYBottomZBottom_0)
					{
						XBottomYBottomZBottomVector_0 = Vertices.Add(
							FVector(XMap.Key - XOffset, YMap.Key - YOffset, ZMap.Key)); // 0 - 0 0 0
						VertexColors.Add(FColor(255,255,255));
						UV0.Add(BlockTextureManager->GetBlock(ZMap.Value).BottomUV[0]);
					}
					int32 XBottomYBottomZTopVector_1;
					if (XBottomYBottomZTop_1)
					{
						XBottomYBottomZTopVector_1 = Vertices.Add(
							FVector(XMap.Key - XOffset, YMap.Key - YOffset, ZMap.Key + Snap)); // 1 - 0 0 100
						VertexColors.Add(FColor(255,255,255));
						UV0.Add(BlockTextureManager->GetBlock(ZMap.Value).BottomUV[3]);

					}
					int32 XBottomYTopZBottomVector_2;
					if (XBottomYTopZBottom_2)
					{
						XBottomYTopZBottomVector_2 = Vertices.Add(
							FVector(XMap.Key - XOffset, YMap.Key - YOffset + Snap, ZMap.Key)); // 2 - 0 100 0
						VertexColors.Add(FColor(255,255,255));
						UV0.Add(BlockTextureManager->GetBlock(ZMap.Value).BottomUV[1]);

					}
					int32 XBottomYTopZTopVector_3;
					if (XBottomYTopZTop_3)
					{
						XBottomYTopZTopVector_3 = Vertices.Add(
							FVector(XMap.Key - XOffset, YMap.Key - YOffset + Snap, ZMap.Key + Snap));
						// 3 - 0 100 100
						VertexColors.Add(FColor(255,255,255));
						UV0.Add(BlockTextureManager->GetBlock(ZMap.Value).BottomUV[2]);

					}
					int32 XTopYBottomZBottomVector_4;
					if (XTopYBottomZBottom_4)
					{
						XTopYBottomZBottomVector_4 = Vertices.Add(
							FVector(XMap.Key - XOffset + Snap, YMap.Key - YOffset, ZMap.Key)); // 4 - 100, 0, 0
						VertexColors.Add(FColor(255,255,255));
						UV0.Add(BlockTextureManager->GetBlock(ZMap.Value).BottomUV[1]);
					}
					int32 XTopYBottomZTopVector_5;
					if (XTopYBottomZTop_5)
					{
						XTopYBottomZTopVector_5 = Vertices.Add(
							FVector(XMap.Key - XOffset + Snap, YMap.Key - YOffset, ZMap.Key + Snap));
						// 5 - 100 0 100
						VertexColors.Add(FColor(255,255,255));
						UV0.Add(BlockTextureManager->GetBlock(ZMap.Value).BottomUV[2]);

					}
					int32 XTopYTopZTopVector_6;
					if (XTopYTopZTop_6)
					{
						XTopYTopZTopVector_6 = Vertices.Add(
							FVector(XMap.Key - XOffset + Snap, YMap.Key - YOffset + Snap, ZMap.Key + Snap));
						// 6 - 100 100 100
						VertexColors.Add(FColor(255,255,255));
						UV0.Add(BlockTextureManager->GetBlock(ZMap.Value).BottomUV[3]);

					}
					int32 XTopYTopZBottomVector_7;
					if (XTopYTopZBottom_7)
					{
						XTopYTopZBottomVector_7 = Vertices.Add(
							FVector(XMap.Key - XOffset + Snap, YMap.Key - YOffset + Snap, ZMap.Key));
						// 7 - 100 100 0
						VertexColors.Add(FColor(255,255,255));
						UV0.Add(BlockTextureManager->GetBlock(ZMap.Value).BottomUV[0]);
					}

					if (XBottomYBottomZBottom_0 && XBottomYTopZBottom_2 && XBottomYTopZTop_3 && XBottomYBottomZTop_1)
					{
						Triangles.Add(XBottomYBottomZBottomVector_0);
						Triangles.Add(XBottomYTopZBottomVector_2);
						Triangles.Add(XBottomYTopZTopVector_3);
						Triangles.Add(XBottomYTopZTopVector_3);
						Triangles.Add(XBottomYBottomZTopVector_1);
						Triangles.Add(XBottomYBottomZBottomVector_0);
					}
					if (XTopYBottomZBottom_4 && XBottomYBottomZBottom_0 && XBottomYBottomZTop_1 && XTopYBottomZTop_5)
					{
						Triangles.Add(XBottomYBottomZBottomVector_0);
						Triangles.Add(XBottomYBottomZTopVector_1);
						Triangles.Add(XTopYBottomZBottomVector_4);
						Triangles.Add(XTopYBottomZBottomVector_4);
						Triangles.Add(XBottomYBottomZTopVector_1);
						Triangles.Add(XTopYBottomZTopVector_5);
					}
					if (XTopYBottomZBottom_4 && XTopYBottomZTop_5 && XTopYTopZBottom_7 && XTopYTopZTop_6)
					{
						Triangles.Add(XTopYBottomZBottomVector_4);
						Triangles.Add(XTopYBottomZTopVector_5);
						Triangles.Add(XTopYTopZBottomVector_7);
						Triangles.Add(XTopYTopZBottomVector_7);
						Triangles.Add(XTopYBottomZTopVector_5);
						Triangles.Add(XTopYTopZTopVector_6);
					}
					if (XTopYTopZBottom_7 && XTopYTopZTop_6 && XBottomYTopZBottom_2 && XBottomYTopZTop_3)
					{
						Triangles.Add(XTopYTopZBottomVector_7);
						Triangles.Add(XTopYTopZTopVector_6);
						Triangles.Add(XBottomYTopZTopVector_3);
						Triangles.Add(XBottomYTopZTopVector_3);
						Triangles.Add(XBottomYTopZBottomVector_2);
						Triangles.Add(XTopYTopZBottomVector_7);
					}
					if (XBottomYTopZTop_3 && XTopYTopZTop_6 && XTopYBottomZTop_5 && XBottomYBottomZTop_1)
					{
						Triangles.Add(XBottomYBottomZTopVector_1);
						Triangles.Add(XBottomYTopZTopVector_3);
						Triangles.Add(XTopYBottomZTopVector_5);
						Triangles.Add(XTopYTopZTopVector_6);
						Triangles.Add(XTopYBottomZTopVector_5);
						Triangles.Add(XBottomYTopZTopVector_3);
					}
					if (XTopYTopZBottom_7 && XBottomYTopZBottom_2 && XBottomYBottomZBottom_0 && XTopYBottomZBottom_4)
					{
						Triangles.Add(XBottomYTopZBottomVector_2);
						Triangles.Add(XBottomYBottomZBottomVector_0);
						Triangles.Add(XTopYBottomZBottomVector_4);
						Triangles.Add(XTopYBottomZBottomVector_4);
						Triangles.Add(XTopYTopZBottomVector_7);
						Triangles.Add(XBottomYTopZBottomVector_2);
					}
				}
			}
		}
	}
	BlocksMeshes->CreateMeshSection(0, Vertices, Triangles, Normals, UV0, VertexColors, Tangents, true);
	BlocksMeshes->SetMaterial(0, BlocksMaterial);
}

void AChunk::PlaceBlock(int32 X, int32 Y, int32 Z, EBlockType Block)
{
	PlaceBuilding(Utils::SnapValue(X, Snap), Utils::SnapValue(Y, Snap), Utils::SnapValue(Z, Snap), Block);
	UpdateMesh();
}

void AChunk::DestroyBlock(int32 X, int32 Y, int32 Z)
{
	if (BuildingExist(X, Y, Z))
	{
		Blocks.Find(X)->Find(Y)->Remove(Z);
		Blocks.Find(X)->Find(Y)->Shrink();
		UpdateMesh();
	}
}

void AChunk::GenerateBaseTerrain(UFastNoiseWrapper* NoiseGenerator)
{
	// fill map with locations for blocks
	int StartXAxis = Utils::SnapValue(GetActorLocation().X, Snap) / Snap;
	int StartYAxis = Utils::SnapValue(GetActorLocation().Y, Snap) / Snap;

	UE_LOG(LogTemp, Warning, TEXT("x=%d, %f"), StartXAxis, GetActorLocation().X);
	UE_LOG(LogTemp, Warning, TEXT("y=%d, %f"), StartYAxis, GetActorLocation().Y);
	for (int XAxis = StartXAxis; XAxis < ChunkSize + StartXAxis; XAxis++)
	{
		for (int YAxis = StartYAxis; YAxis < ChunkSize + StartYAxis; YAxis++)
		{
			const int Noise2d = Utils::Max(
				Utils::SnapValue(NoiseGenerator->GetNoise2D(XAxis, YAxis) * Snap * Snap, Snap), -Snap * ChunkHeight);
			EBlockType Block = BlockSpawnManager->GetBlockByHeight(NoiseGenerator->GetNoise3D(XAxis, YAxis, Noise2d / Snap) * Snap);
			FVector BlockLocation = FVector(XAxis * Snap, YAxis * Snap, Noise2d);
			if (!BuildingExist(BlockLocation.X, BlockLocation.Y, BlockLocation.Z))
			{
				PlaceBuilding(BlockLocation.X, BlockLocation.Y, BlockLocation.Z, Block);
			}
			while (BlockLocation.Z > -Snap * ChunkHeight)
			{
				Block = BlockSpawnManager->GetBlockByHeight(NoiseGenerator->GetNoise3D(XAxis, YAxis, BlockLocation.Z / Snap) * Snap);
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
