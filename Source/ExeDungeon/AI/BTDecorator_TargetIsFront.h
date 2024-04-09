// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_TargetIsFront.generated.h"

/**
 * 
 */
UCLASS()
class EXEDUNGEON_API UBTDecorator_TargetIsFront : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_TargetIsFront();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
