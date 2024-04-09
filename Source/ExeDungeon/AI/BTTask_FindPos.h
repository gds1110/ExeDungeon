// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindPos.generated.h"

/**
 * 
 */
UCLASS()
class EXEDUNGEON_API UBTTask_FindPos : public UBTTaskNode
{
	GENERATED_BODY()
	
	public:
		UBTTask_FindPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
