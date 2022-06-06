#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MinesweeperMap.generated.h"

USTRUCT(BlueprintType)
struct UE_MINESWEEPER_API FMinesweeperMap
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int Width = 0;

	UPROPERTY(BlueprintReadOnly)
	int Height = 0;

	UPROPERTY(BlueprintReadOnly)
	TArray<int> BlockStates;

	void Init(int width, int height, TArray<int> blockStates)
	{
		Width = width;
		Height = height;
		BlockStates = blockStates;
	}

	int GetStateAt(int IdX, int IdY)
	{
		if(IdY * Width + IdX >= BlockStates.Num())
		{
			UE_LOG(LogTemp, Error, TEXT("Wrong block index"));
			return -1;
		}
		
		return BlockStates[IdY*Width + IdX];
	}
};
