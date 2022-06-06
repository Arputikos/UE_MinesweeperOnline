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

	void SetupCameraToMap(float SizeX, float SizeY, float BlockSpacing);

protected:
	UPROPERTY()
	UCameraComponent* PlayerCamera;

	UPROPERTY()
	float TargetCameraOrthoWidth = 1000;
};
