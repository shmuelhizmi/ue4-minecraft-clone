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
	Glass,
};

USTRUCT(BlueprintType)
struct FBlockTextureData
{
	GENERATED_USTRUCT_BODY()
	FBlockTextureData(): TopFaceUV(TArray<FVector2D>()), BottomUV(TArray<FVector2D>()),
	                     AroundBlocksUV(TArray<FVector2D>()), IsTransparent(false)
	{
	}

	FBlockTextureData(TArray<FVector2D> TopFaceUV, TArray<FVector2D> BottomUV, TArray<FVector2D> AroundBlocksUV,
	                  bool IsTransparent) :
		TopFaceUV(TopFaceUV), BottomUV(BottomUV), AroundBlocksUV(AroundBlocksUV), IsTransparent(IsTransparent)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector2D> TopFaceUV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector2D> BottomUV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVector2D> AroundBlocksUV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsTransparent;
};

UCLASS(Blueprintable)
class UBlockTextureManager final : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=TextureMap)
	TMap<EBlockType, FBlockTextureData> Blocks;

	UFUNCTION(BlueprintCallable, Category=Actions)
	void RegisterBlock(const EBlockType Block, const FBlockTextureData UVs)
	{
		if (Blocks.Contains(Block))
		{
			Blocks.Remove(Block);
			Blocks.Shrink();
		}
		Blocks.Add(Block, UVs);
	};
	UFUNCTION(BlueprintCallable, Category=Actions)
	FBlockTextureData GetBlock(const EBlockType Block)
	{
		const auto FoundBlock = Blocks.Find(Block);
		return FBlockTextureData(Utils::Divide(FoundBlock->TopFaceUV, TextureSize),
		                         Utils::Divide(FoundBlock->BottomUV, TextureSize),
		                         Utils::Divide(FoundBlock->AroundBlocksUV, TextureSize), FoundBlock->IsTransparent);
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D TextureSize;
};
