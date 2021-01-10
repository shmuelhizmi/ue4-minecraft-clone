#pragma once

class Utils
{
public:
	static int SnapValue(const float Value,const int Snap)
	{
		const int IntValue = FMath::Floor(Value);
		if (IntValue == 0)
		{
			return 0;
		}
		return IntValue - (IntValue % Snap);
	}

	static float GetPointIn(const float Points, float Z)
	{
		Z = FMath::Abs(Z);
		while (Z > Points)
		{
			Z -= Points;
		}
		return Z;
	}

	static TArray<FVector2D> Divide(TArray<FVector2D> Value1, FVector2D Value2)
	{
		TArray<FVector2D> Result;
		for (FVector2D Item : Value1)
		{
			Result.Add(Item / Value2);
		}
		return Result;
	}
};
