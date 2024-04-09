// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ED_MonsterClassData.generated.h"

/**
 * 
 */
UCLASS()
class EXEDUNGEON_API UED_MonsterClassData : public UDataAsset
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AED_EnemyCharacter> NormalGoblin;	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AED_EnemyCharacter> RedGoblin;
};
