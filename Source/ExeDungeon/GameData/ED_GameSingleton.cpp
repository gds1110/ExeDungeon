// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/ED_GameSingleton.h"

DEFINE_LOG_CATEGORY(LOG_GameSingleton);

UED_GameSingleton::UED_GameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/Dungeon/CharacterStatData/ED_CharacterStat.ED_CharacterStat'"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FED_CharacterStat*>(Value);
			}
		);
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);

	static ConstructorHelpers::FObjectFinder<UDataTable> EDataTableRef(TEXT("/Script/Engine.DataTable'/Game/Dungeon/CharacterStatData/ED_EnemyCharacterStat.ED_EnemyCharacterStat'"));
	if (nullptr != EDataTableRef.Object)
	{
		const UDataTable* DataTable = EDataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, EnemyStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FED_CharacterStat*>(Value);
			}
		);
	}

	CharacterMaxLevel = EnemyStatTable.Num();
	ensure(CharacterMaxLevel > 0);
}

UED_GameSingleton& UED_GameSingleton::Get()
{
	// TODO: insert return statement here

	if (GEngine->GameSingleton)
	{
		UED_GameSingleton* Singleton = CastChecked<UED_GameSingleton>(GEngine->GameSingleton);
		if (Singleton)
		{
			return *Singleton;
		}
	}
	UE_LOG(LOG_GameSingleton, Error, TEXT("iNVAILD"));
	return *NewObject<UED_GameSingleton>();
}
