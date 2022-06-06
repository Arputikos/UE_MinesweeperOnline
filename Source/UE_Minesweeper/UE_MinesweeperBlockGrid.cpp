// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_MinesweeperBlockGrid.h"

#include "MinesweeperMap.h"
#include "UE_MinesweeperBlock.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

AUE_MinesweeperBlockGrid::AUE_MinesweeperBlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;
	
	// Set defaults
	SizeX = 3;
	SizeY = 5;
	BlockSpacing = 300.f;
}


void AUE_MinesweeperBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	
}

void AUE_MinesweeperBlockGrid::NewMap(FMinesweeperMap& map)
{
	RemoveBlocks();

	SizeX = map.Width;
	SizeY = map.Height;
	
	this->SetActorLocation(FVector(-(SizeX - 1)* BlockSpacing/2.0, -(SizeY - 1) * BlockSpacing/2.0, 75.0f));

	for(int32 IdY=0; IdY<map.Height; IdY++)
	{
		for(int32 IdX=0; IdX<map.Width; IdX++)
		{
			const float XOffset = IdX * BlockSpacing;
			const float YOffset = IdY * BlockSpacing;

			const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

			AUE_MinesweeperBlock* NewBlock = GetWorld()->SpawnActor<AUE_MinesweeperBlock>(BlockLocation, FRotator(0,0,0));
			if (NewBlock != nullptr)
			{
				NewBlock->Init(IdX, IdY);
				NewBlock->SetState(map.GetStateAt(IdX, IdY));
				NewBlock->OwningGrid = this;
				Blocks.Add(NewBlock);
			}			
		}
	}
}

void AUE_MinesweeperBlockGrid::Update(FMinesweeperMap& map)
{
	for(int32 IdX=0; IdX<map.Width; IdX++)
	{
		for(int32 IdY=0; IdY<map.Height; IdY++)
		{
			if(IdY * map.Width + IdX >= Blocks.Num())
				return;
			
			AUE_MinesweeperBlock* CurrBlock = Blocks[IdY * map.Width + IdX];
			if (CurrBlock != nullptr)
				CurrBlock->SetState(map.GetStateAt(IdX, IdY));
		}
	}
}

void AUE_MinesweeperBlockGrid::RemoveBlocks()
{
	for(auto b : Blocks)
		b->Destroy();
	
	Blocks.Empty();
}

#undef LOCTEXT_NAMESPACE
