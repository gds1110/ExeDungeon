// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "ED_ItemDatabase.h"
#include "Interface/ED_InteractInterface.h"
#include "ED_Item.generated.h"

/**
 * 
 */

UCLASS()
class EXEDUNGEON_API AED_Item : public AActor,public IED_InteractInterface
{
	GENERATED_BODY()
	
public:
	AED_Item();
	
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere)
	FItemData ThisData;

	UPROPERTY(EditAnywhere)
	FName DataId;
	UPROPERTY(EditAnywhere)
	FName InterName;
	void FindMyData();
	UPROPERTY(EditAnywhere, Category = Interact)
	float InteractTime = 0;

	virtual void InteractAction() override;
	virtual void InteractAction(class AED_PlayerCharacter* player) override;
	virtual FName GetInterName() override;
	virtual float GetInteractTime() override;
	virtual bool GetIsInteract() override;
};
