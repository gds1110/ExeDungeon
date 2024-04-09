// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ED_CharacterStat.generated.h"


USTRUCT(BlueprintType)
struct FED_CharacterStat : public FTableRowBase
{
	GENERATED_BODY()



public:
	FED_CharacterStat() : MaxHp(0.0f), Attack(0.0f), AttackRange(0.0f), AttackSpeed(0.0f), MovementSpeed(0.0f){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
		float MovementSpeed;

	FED_CharacterStat operator+(const FED_CharacterStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FED_CharacterStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FED_CharacterStat) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}

};