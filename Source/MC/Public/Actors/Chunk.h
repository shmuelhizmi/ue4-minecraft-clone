// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FastNoiseGenerator/Public/FastNoiseWrapper.h"
#include "ProceduralMeshComponent.h"
#include "Managers/BlockSpawnManager.h"

#include "Chunk.generated.h"


UCLASS()
class MC_API AChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunk();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Material)
	UMaterialInterface* BlocksMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Meshes)
	UProceduralMeshComponent* BlocksMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Blocks)
	UBlockSpawnManager* BlockSpawnManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Blocks)
	UBlockTextureManager* BlockTextureManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Stats)
	int ChunkSize;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Stats)
	int ChunkHeight;

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Stats)
	float Snap;

	// map[x][y][z] = block
	TMap<int32, TMap<int32, TMap<int32, EBlockType>>> Blocks;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	bool BuildingExist(int32 X, int32 Y, int32 Z);
	bool ShouldBuild(int32 X, int32 Y, int32 Z);
	void PlaceBuilding(int32 X, int32 Y, int32 Z, EBlockType Value);

	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void GenerateChunk(UFastNoiseWrapper* NoiseGenerator);
	
	void GenerateBaseTerrain(UFastNoiseWrapper* NoiseGenerator);

	void UpdateMesh();

	UFUNCTION(BlueprintCallable, Category=Actions)
	void PlaceBlock(int32 X, int32 Y, int32 Z, EBlockType Block);
	UFUNCTION(BlueprintCallable, Category=Actions)
	void DestroyBlock(int32 X, int32 Y, int32 Z);
};
