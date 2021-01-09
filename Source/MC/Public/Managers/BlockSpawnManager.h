#pragma once
#include "BlockTextureManager.h"
#include "utils/Utils.h"
#include "BlockSpawnManager.generated.h"


USTRUCT(BlueprintType)
struct FSpawnData
{
	GENERATED_BODY()
	FSpawnData(): SpawnHeight(FVector2D()), SpawnPoints(0.f) {}
	FSpawnData(FVector2D SpawnHeight, float SpawnPoints): SpawnHeight(SpawnHeight), SpawnPoints(SpawnPoints) {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D SpawnHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnPoints;
};

UCLASS(Blueprintable)
class UBlockSpawnManager final : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=TextureMap)
	TMap<EBlockType, FSpawnData> Blocks;

	UFUNCTION(BlueprintCallable, Category=Actions)
	void RegisterBlock(const EBlockType Block, const FSpawnData UVs)
	{
		if (Blocks.Contains(Block))
		{
			Blocks.Remove(Block);
			Blocks.Shrink();
		}
		Blocks.Add(Block, UVs);
	};
	UFUNCTION(BlueprintCallable, Category=Actions)
	FSpawnData GetBlock(const EBlockType Block)
	{
		return *Blocks.Find(Block);
	}

	EBlockType GetBlockByHeight(float Z)
	{
		TArray<TPair<EBlockType, FSpawnData>> PossibleSpawns;
		float TotalPoints = 0;
		for (const TPair<EBlockType, FSpawnData> Block : Blocks)
		{
			if (Z > Block.Value.SpawnHeight.X && Z < Block.Value.SpawnHeight.Y)
			{
				PossibleSpawns.Add(Block);
				TotalPoints += Block.Value.SpawnPoints;
			}
		}
		const float SelectedPoint = Utils::GetPointIn(TotalPoints, Z);
		float LeftPoints = SelectedPoint;
		while (true)
		{
			for (const TPair<EBlockType, FSpawnData> Block : PossibleSpawns)
			{
				if (LeftPoints - Block.Value.SpawnPoints > 0)
				{
					LeftPoints -= Block.Value.SpawnPoints;
				}
				else
				{
					return Block.Key;
				}
			}
		}
	}
};
