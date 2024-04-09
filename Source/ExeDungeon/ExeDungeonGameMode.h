// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Item/ED_ItemDatabase.h"
#include "Interface/ED_GameInterface.h"
#include "ExeDungeonGameMode.generated.h"

UCLASS(minimalapi)
class AExeDungeonGameMode : public AGameModeBase, public IED_GameInterface
{
	GENERATED_BODY()

public:
	AExeDungeonGameMode();

public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Datatable)
	class UDataTable* ItemTable;


	FVector PlayerStartLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<APawn> TestPawnClass;

public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	uint8 bIsCleared : 1;

public:
	virtual void BeginPlay() override;

public:
	FItemData FindData(FName Id);
	FItemData GetRandomData();
	
public:
	virtual void OnPlayerDead() override;
	virtual bool IsGameClear() override;
	virtual void GameClear() override;

};



