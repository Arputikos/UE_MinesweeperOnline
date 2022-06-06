// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_MinesweeperPlayerController.h"

AUE_MinesweeperPlayerController::AUE_MinesweeperPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}
