// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ED_CharacterBase.h"
#include "Interface/ED_CharacterAIInterface.h"
#include "ED_EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class EXEDUNGEON_API AED_EnemyCharacter : public AED_CharacterBase , public IED_CharacterAIInterface
{
	GENERATED_BODY()
	
public:
	AED_EnemyCharacter();

	void AttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	
	bool bIsAttacking;
protected:
	void SetDead() override;

	void BeginPlay() override;

	virtual void EnemyAttack() override;
	// AI Section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;
	virtual bool GetIsAttacked() override;
	FAICharacterAttackFinished OnAttackFinished;

public:
	TSubclassOf<class AED_RootBox> RootBoxClass;
};
