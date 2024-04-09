// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterStat/ED_CharacterStat.h"
#include "ED_GameSingleton.generated.h"



DECLARE_LOG_CATEGORY_EXTERN(LOG_GameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class EXEDUNGEON_API UED_GameSingleton : public UObject
{
	GENERATED_BODY()
	
	
public:
	UED_GameSingleton();
	static UED_GameSingleton& Get();

public:
	FORCEINLINE FED_CharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FED_CharacterStat(); }
	FORCEINLINE FED_CharacterStat GetEnemyStat(int32 InLevel) const { return EnemyStatTable.IsValidIndex(InLevel - 1) ? EnemyStatTable[InLevel - 1] : FED_CharacterStat(); }


	UPROPERTY()
	int32 CharacterMaxLevel;

private:
	TArray<FED_CharacterStat> CharacterStatTable;
	TArray<FED_CharacterStat> EnemyStatTable;


	
};
