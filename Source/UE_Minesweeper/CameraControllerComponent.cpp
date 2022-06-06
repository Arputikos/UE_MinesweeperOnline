#include "CameraControllerComponent.h"

#include "Camera/CameraComponent.h"

UCameraControllerComponent::UCameraControllerComponent()
{
}

void UCameraControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerCamera = nullptr;
	if (APlayerController* PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		PlayerCamera = PC->GetViewTarget()->FindComponentByClass<UCameraComponent>();
		PlayerCamera->SetOrthoWidth(10000);
		TargetCameraOrthoWidth = PlayerCamera->OrthoWidth;
	}
}

void UCameraControllerComponent::Update(float DeltaTime)
{
	if(PlayerCamera && PlayerCamera->OrthoWidth - TargetCameraOrthoWidth > 10.0f)
		PlayerCamera->SetOrthoWidth(FMath::Lerp(PlayerCamera->OrthoWidth, TargetCameraOrthoWidth, DeltaTime));
}

void UCameraControllerComponent::SetupCameraToMap(float SizeX, float SizeY, float BlockSpacing)
{
	//Quickly set up the camera
	if(PlayerCamera)
	{
		//If the map is more wide than the view, fit on X axis
		float MapAspectRatio = SizeX / SizeY;
		if(MapAspectRatio > PlayerCamera->AspectRatio)
			TargetCameraOrthoWidth = SizeX * BlockSpacing * 1.85f;
		else
			TargetCameraOrthoWidth = SizeY * BlockSpacing * 1.85f;
	}
}
