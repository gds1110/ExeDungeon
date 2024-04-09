// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "MAnimNotify_AttackHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class EXEDUNGEON_API UMAnimNotify_AttackHitCheck : public UAnimNotify_PlayMontageNotify
{
	GENERATED_BODY()
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
