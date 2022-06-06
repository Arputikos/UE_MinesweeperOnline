// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_MinesweeperBlock.h"

#include "MinesweeperBackend.h"
#include "UE_MinesweeperBlockGrid.h"
#include "UE_MinesweeperGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Materials/MaterialInstance.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

AUE_MinesweeperBlock::AUE_MinesweeperBlock()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, BaseMaterial(TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"))
			, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	//ExplosionParticleSystemClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> UnhidePS(TEXT("ParticleSystem'/Game/Puzzle/Particles/P_Unhide.P_Unhide'"));
	if(UnhidePS.Succeeded())
		UnhideParticleSystemClass = UnhidePS.Object;
	else
		UnhideParticleSystemClass = nullptr;
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionPS(TEXT("ParticleSystem'/Game/Puzzle/Particles/P_Explosion.P_Explosion'"));
	if(ExplosionPS.Succeeded())
		ExplosionParticleSystemClass = ExplosionPS.Object;
	else
		ExplosionParticleSystemClass = nullptr;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &AUE_MinesweeperBlock::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &AUE_MinesweeperBlock::OnFingerPressedBlock);

	// Create static mesh component
	Text = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text"));
	Text->SetRelativeLocation(FVector(0.f,0.f,70.0f));
	Text->SetRelativeRotation(FRotator(90.f,0.f,270.f));
	Text->SetRelativeScale3D(FVector(7.0f, 7.0f, 7.0f));
	Text->SetText(FText::FromString(""));
	Text->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	Text->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	Text->SetupAttachment(DummyRoot);

	// Save a pointer to the orange material
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();

	State = -1;
}

void AUE_MinesweeperBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}


void AUE_MinesweeperBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}

void AUE_MinesweeperBlock::HandleClicked()
{
	AUE_MinesweeperGameMode* GameMode = (AUE_MinesweeperGameMode*)GetWorld()->GetAuthGameMode();
	GameMode->BlockClicked(IdX, IdY);
}

void AUE_MinesweeperBlock::Init(int X, int Y)
{
	IdX = X;
	IdY = Y;
}

void AUE_MinesweeperBlock::SetState(int NewState)
{
	if(NewState != State && NewState != 10)
	{
		//Unhide particle FX
		if(UnhideParticleSystemClass)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UnhideParticleSystemClass, GetActorLocation(), FRotator::ZeroRotator, FVector(3, 3, 3));
	}
	
	State = NewState;
	if(State == -1)
	{
		Text->SetText(FText::FromString(""));
		BlockMesh->SetMaterial(0, BaseMaterial);
	}
	else if(State == 0)
	{
		Text->SetText(FText::FromString(""));
		BlockMesh->SetMaterial(0, BlueMaterial);
	}
	else if(State < 9)
	{
		Text->SetText(FText::FromString(FString::FromInt(NewState)));
		BlockMesh->SetMaterial(0, BlueMaterial);
	}
	else
	{
		Text->SetText(FText::FromString("*"));
		BlockMesh->SetMaterial(0, OrangeMaterial);
		//Explosion!
		if(ExplosionParticleSystemClass)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticleSystemClass, GetActorLocation(), FRotator::ZeroRotator, FVector(4, 4, 4));
	}
}

