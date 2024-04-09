// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ED_InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UED_InteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class EXEDUNGEON_API IED_InteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void InteractAction()=0;
	virtual void InteractAction(class AED_PlayerCharacter* player) = 0;
	virtual float GetInteractTime() = 0;
	virtual FName GetInterName()=0;
	virtual bool GetIsInteract()=0;
};
