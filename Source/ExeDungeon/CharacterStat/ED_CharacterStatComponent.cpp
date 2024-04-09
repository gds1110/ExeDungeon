// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ED_CharacterStatComponent.h"
#include "GameData/ED_GameSingleton.h"

// Sets default values for this component's properties
UED_CharacterStatComponent::UED_CharacterStatComponent()
{
	CurrentLevel = 1;


	bWantsInitializeComponent = true;

	AttackRadius = 50.0f;
}



void UED_CharacterStatComponent::InitializeComponent()
{
	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
}

void UED_CharacterStatComponent::BeginPlay()
{
	/*SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);*/
	
	UE_LOG(LogTemp, Log, TEXT("StatComponent BeginPlay %f"), BaseStat.MaxHp);
}

void UED_CharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, GetTotalStat().MaxHp);

	OnHpChanged.Broadcast(CurrentHp);
}

void UED_CharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UED_GameSingleton::Get().CharacterMaxLevel);
	BaseStat = UED_GameSingleton::Get().GetCharacterStat(CurrentLevel);
	check(BaseStat.MaxHp > 0.0f);

}

void UED_CharacterStatComponent::SetEnemyLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UED_GameSingleton::Get().CharacterMaxLevel);
	BaseStat = UED_GameSingleton::Get().GetEnemyStat(CurrentLevel);
	check(BaseStat.MaxHp > 0.0f);

	SetHp(BaseStat.MaxHp);
}

float UED_CharacterStatComponent::ApplyDamage(float InDamage)
{

	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
	UE_LOG(LogTemp, Log, TEXT("ATTACKED DAMAGE : %f"), ActualDamage);
	return ActualDamage;
}

void UED_CharacterStatComponent::HealHp(float InHeal)
{


	const float PrevHp = CurrentHp;
	SetHp(PrevHp + InHeal);

}

void UED_CharacterStatComponent::InitStatComp()
{

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
}


