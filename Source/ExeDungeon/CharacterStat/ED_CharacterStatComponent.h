// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ED_CharacterStat.h"
#include "ED_CharacterStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate,float/*HP*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXEDUNGEON_API UED_CharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UED_CharacterStatComponent();

protected:
	virtual void InitializeComponent();
public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	virtual void BeginPlay() override;
protected:
	void SetHp(float NewHp);
	
public:
	void SetLevelStat(int32 InNewLevel);
	void SetEnemyLevelStat(int32 InNewLevel);
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }
	FORCEINLINE void SetModifierStat(const FED_CharacterStat& InModifierStat) { ModifierStat = InModifierStat; }
	FORCEINLINE void SetWeaponModifierStat(const FED_CharacterStat& InModifierStat) { WeaponModifierStat = InModifierStat; }
	FORCEINLINE void SetArmorModifierStat(const FED_CharacterStat& InModifierStat) { ArmorModifierStat = InModifierStat; }
	FORCEINLINE FED_CharacterStat GetTotalStat() const { return BaseStat + ModifierStat+ WeaponModifierStat+ ArmorModifierStat; }
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }
	float ApplyDamage(float InDamage);
	void HealHp(float InHeal);
	void InitStatComp();
protected:

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;
	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRadius;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FED_CharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FED_CharacterStat ModifierStat;	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FED_CharacterStat WeaponModifierStat;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FED_CharacterStat ArmorModifierStat;

};
