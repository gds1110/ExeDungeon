// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ABAI.h"
#include "ED_AIController.generated.h"

/**
 * 
 */
UCLASS()
class EXEDUNGEON_API AED_AIController : public AAIController
{
	GENERATED_BODY()
	
	
public:
	AED_AIController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn);

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
};
