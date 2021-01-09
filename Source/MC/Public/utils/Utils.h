#pragma once

class Utils
{
public:
	static int SnapValue(int Value, int Snap)
	{
		if (Value != 0)
		{
			return Value - (Value % Snap);
		}
		return 0;
	}

	static int Max(int Value1, int Value2)
	{
		if (Value1 > Value2)
		{
			return  Value1;
		}
		return Value2;
	}

	static float GetPointIn(const float Points, float Z)
	{
		while (Z > Points)
		{
			Z -= Points;
		}
		return  Z;
	}
	static TArray<FVector2D> Divide(TArray<FVector2D> Value1, FVector2D Value2)
	{
		TArray<FVector2D> Result;
		for (FVector2D Item : Value1)
		{
			Result.Add(Item / Value2);
		}
		return  Result;
	}
};
