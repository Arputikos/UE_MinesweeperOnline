// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IWebSocket.h"
#include "UObject/Object.h"
#include "MinesweeperBackend.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewLevelRequestComplete, FString, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMapRequestComplete, FString, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOpenBlockRequestComplete, FString, Message);

UENUM()
enum MESSAGE_TYPE
{
	NewLevel UMETA(DisplayName="New Level"),
	Map UMETA(DisplayName="Map"),
	Open UMETA(DisplayName="Open"),
	None UMETA(Hidden)
};

UCLASS(BlueprintType)
class UE_MINESWEEPER_API UMinesweeperBackend : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void Connect();

	void RequestNewLevel(int level);

	void RequestMap();

	void RequestOpenBlock(int x, int y);

	UPROPERTY(BlueprintAssignable)
	FOnNewLevelRequestComplete OnNewLevelRequestComplete;
	
	UPROPERTY(BlueprintAssignable)
	FOnMapRequestComplete OnMapRequestComplete;

	UPROPERTY(BlueprintAssignable)
	FOnOpenBlockRequestComplete OnOpenBlockRequestComplete;

	UFUNCTION(BlueprintCallable)
	void Close();

private:
	TSharedPtr<IWebSocket> Socket;

	bool Initialized = false;

	MESSAGE_TYPE LastRequestedAction = MESSAGE_TYPE::None;

	UFUNCTION(BlueprintCallable)
	void Send(const FString msg);

	void OnMessageRecieved(const FString& message);
};
