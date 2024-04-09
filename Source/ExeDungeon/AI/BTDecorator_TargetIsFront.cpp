// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_TargetIsFront.h"
#include "ABAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/ED_CharacterAIInterface.h"


UBTDecorator_TargetIsFront::UBTDecorator_TargetIsFront()
{
	NodeName = TEXT("IsFront");

}

bool UBTDecorator_TargetIsFront::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	IED_CharacterAIInterface* AIPawn = Cast<IED_CharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target)
	{
		return false;
	}

	FVector Direction = Target->GetActorLocation() - ControllingPawn->GetActorLocation();

	Direction.Normalize();
	
	FVector ForWardVector = ControllingPawn->GetActorForwardVector();

	float DotProduct = FVector::DotProduct(Direction, ForWardVector);

	bResult = (DotProduct > 0.8f) ? true : false;

	return bResult;
}
