// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ED_InteractInterface.h"
#include "ED_Props.generated.h"

class UBoxComponent;

UCLASS()
class EXEDUNGEON_API AED_Props : public AActor, public IED_InteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AED_Props();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void InteractAction() override;
	virtual void InteractAction(class AED_PlayerCharacter* player) override;
	virtual float GetInteractTime() override;

	virtual FName GetInterName() override;
	virtual bool GetIsInteract() override;

	UPROPERTY(EditAnywhere,Category = Interact)
	float InteractTime=0;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere)
	FName InterName;
};
