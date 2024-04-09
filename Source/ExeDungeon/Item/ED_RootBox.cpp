// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ED_RootBox.h"
#include "Character/ED_PlayerCharacter.h"
#include "Item/ED_LootBoxUI.h"
#include "EXEDungeonGameMode.h"

// Sets default values
AED_RootBox::AED_RootBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AED_RootBox::BeginPlay()
{
	Super::BeginPlay();
	
	AExeDungeonGameMode* CurMode = Cast<AExeDungeonGameMode>(GetWorld()->GetAuthGameMode());
	if (CurMode)
	{
		const int32 RandomIndex = FMath::RandRange(1, 3); //첫번째는  Null이라
		for (int32 i = 0; i < RandomIndex; i++)
		{
			FItemData RandomItem = CurMode->GetRandomData();
			//player->LootList.Add(RandomItem);
			LootItems.Add(RandomItem);
		}
	}
}

void AED_RootBox::InteractAction()
{
}

void AED_RootBox::InteractAction(AED_PlayerCharacter* player)
{
	if (!player->LootBox->IsVisible())
	{
		player->LootBox->RefreshSlots();
		player->CurrentBox = this;
		for (int32 i = 0; i < LootItems.Num(); i++)
		{
			player->LootBox->AddItems(LootItems[i]);
		}
		player->ToggleLootBox();
	}
	else
	{
		if (player->CurrentBox == this)
		{
			player->CurrentBox = nullptr;
		}
		player->ToggleLootBox();

	}
}

FName AED_RootBox::GetInterName()
{
	return InterName;
}

float AED_RootBox::GetInteractTime()
{
	return InteractTime;
}

bool AED_RootBox::GetIsInteract()
{
	return true;
}

void AED_RootBox::MakeRandomLoots()
{


}

void AED_RootBox::RemoveData(FItemData _data)
{
	int32 removeIndex = -1;
	for (int32 i = 0; i < LootItems.Num(); i++)
	{
		if (LootItems[i] == _data)
		{
			removeIndex = i;
			break;
		}
	}
	if (removeIndex !=-1)
	{
		LootItems.RemoveAt(removeIndex);
	}

	if (LootItems.Num() < 1)
	{
		Destroy();
	}
}


