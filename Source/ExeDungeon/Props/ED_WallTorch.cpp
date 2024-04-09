// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/ED_WallTorch.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/LightComponent.h"
#include "Physics/EDCollision.h"
#include "Components/PointLightComponent.h"

AED_WallTorch::AED_WallTorch()
{
	TorchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TorchMeshComp"));
	FireParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	TorchLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("TorchLightComp"));

	TorchMesh->SetCollisionProfileName(CPROFILE_EDINTERACT);
	FireParticle->SetupAttachment(TorchMesh);
	TorchLightComponent->SetupAttachment(TorchMesh);
}

void AED_WallTorch::InteractAction(AED_PlayerCharacter* player)
{
	if (bIsFire == true)
	{
		FireParticle->SetVisibility(false);
		TorchLightComponent->SetVisibility(false);
		
	}
	else
	{
		FireParticle->SetVisibility(true);
		TorchLightComponent->SetVisibility(true);
	}
	bIsFire = !bIsFire;
}

void AED_WallTorch::BeginPlay()
{
	Super::BeginPlay();
	bIsFire = true;
	FireParticle->SetVisibility(true);
	TorchLightComponent->SetVisibility(true);
}
