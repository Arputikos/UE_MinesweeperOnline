// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MinesweeperMap.h"
#include "GameFramework/GameModeBase.h"
#include "UE_MinesweeperGameMode.generated.h"

class UMinesweeperBackend;
class AUE_MinesweeperBlockGrid;

/** GameMode class to specify pawn and playercontroller */
UCLASS(minimalapi)
class AUE_MinesweeperGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUE_MinesweeperGameMode();

	UPROPERTY(BlueprintReadOnly)
	UMinesweeperBackend* Backend;

	UPROPERTY(BlueprintReadOnly)
	AUE_MinesweeperBlockGrid* BlocksGrid;

	UPROPERTY(BlueprintReadOnly)
	FMinesweeperMap CurrentMap;

	UFUNCTION(BlueprintCallable)
	void InitGame();

	UFUNCTION(BlueprintCallable)
	void NewGame(int map);

	UFUNCTION(BlueprintCallable)
	void BlockClicked(int x, int y);

protected:
	//Delegate-called functions
	UFUNCTION()
	void OnNewLevel(FString message);

	UFUNCTION()
	void OnMapRequestComplete(FString message);

	UFUNCTION()
	void OnOpenBlockRequestComplete(FString message);

	UFUNCTION()
	void NewMapFromData(FString message);

	UFUNCTION()
	void UpdateMapFromData(FString message);

	UFUNCTION()
	FMinesweeperMap ParseToMap(FString message);
};
