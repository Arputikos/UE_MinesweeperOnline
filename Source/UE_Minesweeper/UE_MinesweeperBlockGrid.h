// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CameraControllerComponent.h"
#include "MinesweeperMap.h"
#include "GameFramework/Actor.h"
#include "UE_MinesweeperBlockGrid.generated.h"

class UCameraComponent;
class AUE_MinesweeperBlock;

/** Class used to spawn blocks and manage score */
UCLASS(minimalapi)
class AUE_MinesweeperBlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

public:
	AUE_MinesweeperBlockGrid();

	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 SizeX;

	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 SizeY;

	/** Spacing of blocks */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing;

protected:
	UPROPERTY()
	TArray<AUE_MinesweeperBlock*> Blocks;
	
	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

	virtual void Tick(float DeltaSeconds) override;

public:

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }

	void NewMap(FMinesweeperMap& map);

	void Update(FMinesweeperMap& map);

	void RemoveBlocks();

	UPROPERTY()
	UCameraControllerComponent* CameraController;
};



