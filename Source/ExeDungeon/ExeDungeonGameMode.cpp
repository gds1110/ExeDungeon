// Copyright Epic Games, Inc. All Rights Reserved.

#include "ExeDungeonGameMode.h"
#include "ExeDungeonCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Item/ED_ItemDatabase.h"
#include "ExeDungeonPlayerController.h"


AExeDungeonGameMode::AExeDungeonGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Dungeon/Blueprint/Character/Player/BP_EDPlayerCharacter.BP_EDPlayerCharacter_C"));
	TestPawnClass = PlayerPawnClassFinder.Class;
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}

void AExeDungeonGameMode::BeginPlay()
{
	Super::BeginPlay();
	
}

FItemData AExeDungeonGameMode::FindData(FName Id)
{

	//FItemData* RetData = ItemTable->FindRow<FItemData>(FName("ID"), *Id.ToString());
	FItemData* RetData;

	TArray<FName> RowNames = ItemTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		FItemData* TempData = ItemTable->FindRow<FItemData>(RowName, Id.ToString());
		if (TempData->ID==Id)
		{
			RetData = TempData;
			return *RetData;
		}
	}

	RetData = ItemTable->FindRow<FItemData>("NULL", FString(""));
	return *RetData;
}

FItemData AExeDungeonGameMode::GetRandomData()
{
	if (!ItemTable)
	{
		return FItemData();
	}
	const int32 NumRows = ItemTable->GetRowNames().Num();

	const int32 RandomIndex = FMath::RandRange(1, NumRows - 1); //첫번째는  Null이라

	FItemData* RetData;
	FName RandomRow = ItemTable->GetRowNames()[RandomIndex];
	RetData = ItemTable->FindRow<FItemData>(RandomRow, FString(""));
	
	return *RetData;
}

void AExeDungeonGameMode::OnPlayerDead()
{
	AExeDungeonPlayerController* EDPlayerController = Cast<AExeDungeonPlayerController>(GetWorld()->GetFirstPlayerController());
	if (EDPlayerController)
	{
		EDPlayerController->GameOver();
	}
}

bool AExeDungeonGameMode::IsGameClear()
{
	return bIsCleared;
}

void AExeDungeonGameMode::GameClear()
{
	bIsCleared = true;
	AExeDungeonPlayerController* EDPlayerController = Cast<AExeDungeonPlayerController>(GetWorld()->GetFirstPlayerController());
	if (EDPlayerController)
	{
		EDPlayerController->GameClear();
	}
}
