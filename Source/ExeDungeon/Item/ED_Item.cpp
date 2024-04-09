// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ED_Item.h"
#include "EXEDungeonGameMode.h"
#include "Character/ED_PlayerCharacter.h"

AED_Item::AED_Item()
{


}

void AED_Item::BeginPlay()
{
	Super::BeginPlay();

	FindMyData();

}

void AED_Item::FindMyData()
{
	AExeDungeonGameMode* CurMode = Cast<AExeDungeonGameMode>(GetWorld()->GetAuthGameMode());
	if (ThisData.ItemClass == nullptr)
	{
		if (CurMode != nullptr)
		{
			ThisData = CurMode->FindData(DataId);
			UE_LOG(LogTemp, Log, TEXT("%s find"), ThisData.ID);
		}
	}

}

void AED_Item::InteractAction()
{


}

void AED_Item::InteractAction(AED_PlayerCharacter* player)
{

	auto Inventory = player->Inventory;

	if (Inventory.Contains(ThisData))
	{
		if (ThisData.bIsStackable == EStackType::Stackable)
		{
			for (int32 i = 0; i < Inventory.Num(); i++)
			{
				if (Inventory[i] == ThisData)
				{
					Inventory[i].Amount += ThisData.Amount;
					player->RefreshInventory();
					Destroy();

					return;
				}
			}
		}
	}
	else
	{
		for (int32 i = 0; i < Inventory.Num(); i++)
		{
			if (Inventory[i].ItemType == EItemType::None)
			{
				Inventory[i] = ThisData;
				player->RefreshInventory();
				
				Destroy();

				return;
			}
		}
	}

}

FName AED_Item::GetInterName()
{
	return InterName;
}

float AED_Item::GetInteractTime()
{
	return InteractTime;
}

bool AED_Item::GetIsInteract()
{
	return true;
}
