// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_MinesweeperGameMode.h"
#include "MinesweeperBackend.h"
#include "UE_MinesweeperBlockGrid.h"
#include "UE_MinesweeperPlayerController.h"
#include "UE_MinesweeperPawn.h"

AUE_MinesweeperGameMode::AUE_MinesweeperGameMode()
{
	// no pawn by default
	DefaultPawnClass = AUE_MinesweeperPawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AUE_MinesweeperPlayerController::StaticClass();
}

void AUE_MinesweeperGameMode::InitGame()
{
	BlocksGrid = nullptr;
	
	Backend = NewObject<UMinesweeperBackend>(this, FName("Backend"));
	Backend->OnNewLevelRequestComplete.AddDynamic(this, &AUE_MinesweeperGameMode::OnNewLevel);
	Backend->OnMapRequestComplete.AddDynamic(this, &AUE_MinesweeperGameMode::OnMapRequestComplete);
	Backend->OnOpenBlockRequestComplete.AddDynamic(this, &AUE_MinesweeperGameMode::OnOpenBlockRequestComplete);
	Backend->Connect();
}

void AUE_MinesweeperGameMode::NewGame(int map)
{
	//map is valid in <1,4> range
	Backend->RequestNewLevel(map);
	//Backend->Send("map");
	//BlocksGrid->NewMap(3, 5);
}

void AUE_MinesweeperGameMode::BlockClicked(int x, int y)
{
	Backend->RequestOpenBlock(x, y);
}

void AUE_MinesweeperGameMode::OnNewLevel(FString message)
{
	Backend->RequestMap();
}

void AUE_MinesweeperGameMode::OnMapRequestComplete(FString message)
{
	if(nullptr == BlocksGrid)
		NewMapFromData(message);
	else
		UpdateMapFromData(message);
}

void AUE_MinesweeperGameMode::OnOpenBlockRequestComplete(FString message)
{
	Backend->RequestMap();
}

void AUE_MinesweeperGameMode::NewMapFromData(FString message)
{
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	BlocksGrid = GetWorld()->SpawnActor<AUE_MinesweeperBlockGrid>(Location, Rotation);

	CurrentMap = ParseToMap(message);
	UE_LOG(LogTemp, Warning, TEXT("New level: w=%i, h=%i"), CurrentMap.Width, CurrentMap.Height);
	BlocksGrid->NewMap(CurrentMap);
}

void AUE_MinesweeperGameMode::UpdateMapFromData(FString message)
{
	CurrentMap = ParseToMap(message);
	UE_LOG(LogTemp, Warning, TEXT("Updating level: w=%i, h=%i"), CurrentMap.Width, CurrentMap.Height);
	BlocksGrid->Update(CurrentMap);
}

FMinesweeperMap AUE_MinesweeperGameMode::ParseToMap(FString message)
{
	FMinesweeperMap map;
	TArray<int> values;
	TArray<FString> lines;
	message.ParseIntoArrayLines(lines);
	if(lines.Num() > 0)
		lines.RemoveAt(0);//map: text
	
	map.Height = lines.Num();
	if(map.Height < 1)
		return map;
	map.Width = lines[0].Len();
	if(map.Width < 1)
		return map;
	for(int i=0;i<lines.Num();i++)//for each row
	{
		for(int j=0;j<lines[i].Len();j++)//for each column
		{
			values.Add(-1);

			int idx = i*map.Width+j;
			TCHAR* c = &lines[i][j];
			if(*c == '*')
				values[idx] = 10;
			else
			{
				for(int n=0;n<9;n++)
					if(*c == (TCHAR)('0'+n))
					{
						values[idx] = n;
						break;
					}
			}
		}
	}
	map.BlockStates = values;
	return map;
}
