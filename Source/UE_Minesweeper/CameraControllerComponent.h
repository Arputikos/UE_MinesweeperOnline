#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CameraControllerComponent.generated.h"

class UCameraComponent;

UCLASS(BlueprintType)
class UE_MINESWEEPER_API UCameraControllerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UCameraControllerComponent();

	virtual void BeginPlay() override;

	void Update(float DeltaTime);

	void Move(float x, float y);
	void SetupCameraToMap(float SizeX, float SizeY, float BlockSpacing);

	void Zoom(float val) { TargetCameraOrthoWidth -= val; }

	void SetupCameraToMap(float SizeX, float SizeY, float BlockSpacing, FVector gridPos);

protected:
	UPROPERTY()
	UCameraComponent* PlayerCamera;

	UPROPERTY()
	float TargetCameraOrthoWidth = 1000;
};
