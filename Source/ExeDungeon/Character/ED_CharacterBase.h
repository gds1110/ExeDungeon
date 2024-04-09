// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/EDAnimationAttackInterface.h"
#include "Interface/ED_CharacterWidgetInterface.h"
#include "ED_CharacterBase.generated.h"

UCLASS()
class EXEDUNGEON_API AED_CharacterBase : public ACharacter, public IEDAnimationAttackInterface,public IED_CharacterWidgetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AED_CharacterBase();




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UEDComboActionData> ComboActionData;


protected:
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void NotifyComboActionEnd();

	// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead(); 
	void PlayDeadAnimation();
	float DeadEventDelayTime = 2.0f;

	//stat
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category=Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UED_CharacterStatComponent> Stat;

	//widget
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> HpBar;

public:
	virtual void SetupCharacterWidget(class UED_Widget* InUserWidget) override;


	// Stat Section
public:
	int32 GetLevel();
	void SetLevel(int32 InNewLevel);

	class UED_CharacterStatComponent* GetStat() { return Stat; }

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraComponent> EffectComponent;
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* BleedEffect;
};
