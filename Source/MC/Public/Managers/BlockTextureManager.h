#pragma once
#include "utils/Utils.h"
#include "BlockTextureManager.generated.h"


UENUM(BlueprintType)
enum class EBlockType: uint8
{
	Grass,
	Stone,
	Coal,
	Iron,
	Gold,
	RedStone,
	Emerald,
	Diamond,
	Lapis,
};

USTRUCT(BlueprintType)
struct FBlockUVs
{
	GENERATED_USTRUCT_BODY()
	FBlockUVs(): TopFaceUV(TArray<FVector2D>()), BottomUV(TArray<FVector2D>()), AroundBlocksUV(TArray<FVector2D>()) {}
	FBlockUVs(TArray<FVector2D> TopFaceUV, TArray<FVector2D> BottomUV, TArray<FVector2D> AroundBlocksUV) :
		TopFaceUV(TopFaceUV), BottomUV(BottomUV), AroundBlocksUV(AroundBlocksUV)
	{
	}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector2D> TopFaceUV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector2D> BottomUV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector2D> AroundBlocksUV;
};

UCLASS(Blueprintable)
class UBlockTextureManager final : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=TextureMap)
	TMap<EBlockType, FBlockUVs> Blocks;

	UFUNCTION(BlueprintCallable, Category=Actions)
	void RegisterBlock(const EBlockType Block, const FBlockUVs UVs)
	{
		if (Blocks.Contains(Block))
		{
			Blocks.Remove(Block);
			Blocks.Shrink();
		}
		Blocks.Add(Block, UVs);
	};
	UFUNCTION(BlueprintCallable, Category=Actions)
	FBlockUVs GetBlock(const EBlockType Block)
	{
		const auto FoundBlock =  Blocks.Find(Block);
		return FBlockUVs(Utils::Divide(FoundBlock->TopFaceUV , TextureSize), Utils::Divide(FoundBlock->BottomUV , TextureSize), Utils::Divide(FoundBlock->AroundBlocksUV , TextureSize));
	}
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D TextureSize;
};
