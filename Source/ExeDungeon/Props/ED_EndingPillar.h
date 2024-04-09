// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Props/ED_Props.h"
#include "ED_EndingPillar.generated.h"

/**
 * 
 */
UCLASS()
class EXEDUNGEON_API AED_EndingPillar : public AED_Props
{
	GENERATED_BODY()
	
	
public:

	virtual void InteractAction() override;
	virtual void InteractAction(class AED_PlayerCharacter* player) override;
	virtual FName GetInterName() override;
	virtual bool GetIsInteract() override;

};
