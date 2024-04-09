// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ED_LootBoxUI.h"
#include "Inventory/ED_InventoryEntry.h"
#include "Components/VerticalBox.h"

void UED_LootBoxUI::AddSlots()
{

	//LootWindow->AddChildToVerticalBox();

}

void UED_LootBoxUI::AddItems(FItemData _item)
{
	UED_InventoryEntry* ISlot = CreateWidget<UED_InventoryEntry>(GetWorld(), DragVisualClass);
	ISlot->Init(_item,EItemLocated::RootBox);
	//LootWindow->AddChildToVerticalBox(ISlot);
	LootWindow->AddChild(ISlot);
}

void UED_LootBoxUI::RefreshSlots()
{

	LootWindow->ClearChildren();

}
