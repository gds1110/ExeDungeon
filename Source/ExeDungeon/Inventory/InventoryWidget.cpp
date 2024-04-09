// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryWidget.h"
#include "Components/ListView.h"
#include "ED_InventoryEntry.h"
#include "ED_SlotBase.h"
#include "Components/UniformGridPanel.h"


void UInventoryWidget::RefreshInventory(const TArray<FItemData>& Inventory)
{

	//ItemList->ClearListItems();

	//for (FItemData ItemData : Inventory)
	//{
	//	UItemUIObject* UIItem = NewObject<UItemUIObject>();
	//	UIItem->ItemData = ItemData;
	//	ItemList->AddItem(UIItem);
	//}

	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		ItemSlots[i]->ItemData = Inventory[i];
		//ItemSlots[i]->Refresh();
		ItemSlots[i]->Init(Inventory[i], EItemLocated::Inventory);
	}
}

void UInventoryWidget::Init()
{
	Row = 6;
	Col = 5;
	ItemSlots.SetNum(Row * Col);
	int32 SetNum = 0;
	for (int32 i = 0; i < Row; i++)
	{
		for (int32 j = 0; j < Col; j++)
		{
			UED_InventoryEntry* ISlot = CreateWidget<UED_InventoryEntry>(GetWorld(), SlotClass);
			InventoryPanel->AddChildToUniformGrid(ISlot, i, j);
			ISlot->SlotNum = Col * i + j;
		
			ItemSlots[Col * i + j] = ISlot;
			ISlot->Init(FItemData(), EItemLocated::Inventory);
			ISlot->SetPlayer(Player);
		}
	}


}

void UInventoryWidget::SetPlayer(AED_PlayerCharacter* _Player)
{
	Player = _Player;
}

void UInventoryWidget::NativeConstruct()
{

	InventoryPanel = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("InventoryPanel")));

}
