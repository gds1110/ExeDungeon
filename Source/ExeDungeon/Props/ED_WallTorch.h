// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Props/ED_Props.h"
#include "ED_WallTorch.generated.h"

/**
 * 
 */
UCLASS()
class EXEDUNGEON_API AED_WallTorch : public AED_Props
{
	GENERATED_BODY()
	
public:
	AED_WallTorch();
	
public:
	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* FireParticle;

	UPROPERTY(EditAnywhere)
	class UPointLightComponent* TorchLightComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* TorchMesh;

	virtual void InteractAction(AED_PlayerCharacter* player) override;

public:
	bool bIsFire;

protected:
	// Called when the game starts or when spawned
	void BeginPlay();
};
