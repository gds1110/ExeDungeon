// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Props/ED_Props.h"
#include <Components/TimelineComponent.h>
#include "ED_Door.generated.h"

/**
 * 
 */
UCLASS()
class EXEDUNGEON_API AED_Door : public AED_Props
{
	GENERATED_BODY()
	
public:
	AED_Door();



	UPROPERTY(EditAnywhere)
	FTimeline Timeline;					// Timeline 생성

	UPROPERTY(EditAnywhere)
	UCurveFloat* CurveFloat;			// Timeline 커브

	UPROPERTY(EditDefaultsOnly,Category = "Door")
	bool bIsDoorOpened;
	UPROPERTY(EditDefaultsOnly,Category = "Door")
	bool bIsDoorClosed;

	bool IsFront;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Door")
	TObjectPtr<UStaticMeshComponent> DoorLeft;
	UPROPERTY(EditDefaultsOnly, Category = "Door")
	TObjectPtr<UStaticMeshComponent> DoorRight;
	UPROPERTY(EditDefaultsOnly, Category = "Door")
	TObjectPtr<UStaticMeshComponent> DoorWay;

	UPROPERTY(EditDefaultsOnly, Category = "Prop")
	USceneComponent* TorchOne;
	UPROPERTY(EditDefaultsOnly, Category = "Prop")
	USceneComponent* TorchTwo;


protected:
	// Called when the game starts or when spawned
	void BeginPlay();

public:
	// Called every frame
	void Tick(float DeltaTime);
public:

	UFUNCTION()
	void OpenDoor(float Value);
	UFUNCTION()
	void SetDoorOnSameSide();

	virtual void InteractAction() override;
	virtual void InteractAction(AED_PlayerCharacter* player) override;
	virtual bool GetIsInteract() override;

};
