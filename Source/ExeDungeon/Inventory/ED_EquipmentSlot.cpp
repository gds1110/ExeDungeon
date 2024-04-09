// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ED_EquipmentSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "ED_WidgetDragDrop.h"
#include "Character/ED_PlayerCharacter.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"




void UED_EquipmentSlot::Refresh()
{
	
	if (Player != nullptr)
	{

		if (this->ItemSlotType == EItemType::Weapon)
		{
			this->ItemData = Player->WeaponSlot;
		}
		else if (this->ItemSlotType == EItemType::Armor)
		{
			this->ItemData = Player->ArmorSlot;
		}

	}

	if (ItemData.Icon == nullptr)
	{
		if (DefaultImg != nullptr)
		{
			IconImage = DefaultImg;
			ItemIcon->SetBrushFromTexture(DefaultImg);
		}
	}
	else
	{
		IconImage = ItemData.Icon;
		ItemIcon->SetBrushFromTexture(IconImage);
	}


}

void UED_EquipmentSlot::NativeConstruct()
{
	Super::NativeConstruct();
	if (AmountText != nullptr)
	{

		AmountText->SetVisibility(ESlateVisibility::Hidden);

	}

}

FReply UED_EquipmentSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Click : click start"));

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		
			reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);



			return reply.NativeReply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
		
	}
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) == true)
	{
		if (Player != nullptr)
		{
			// 플레이어의 인벤토리로 이동

			//자리 없을경우 그대로
			//Player
			bool IsItemMove = false;

			IsItemMove = Player->InventoryAddItem(this->ItemData);
			if (IsItemMove == true)
			{
				this->ItemData = FItemData();
			
				if (this->ItemSlotType == EItemType::Weapon)
				{
					Player->WeaponSlot = FItemData();
					Player->EquipWeaponClear();
				}
				else if (this->ItemSlotType == EItemType::Weapon)
				{
					Player->ArmorSlot = FItemData();

				}


			}

			Refresh();
		}

		return reply.NativeReply.Handled().DetectDrag(TakeWidget(), EKeys::RightMouseButton);

	}

	return reply.NativeReply;
}

void UED_EquipmentSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (OutOperation == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Draging start"));

		UED_WidgetDragDrop* oper = NewObject<UED_WidgetDragDrop>();
		OutOperation = oper;
		
		oper->InData = this->ItemData;
		oper->FromLocate = EItemLocated::Equipment;
		if (DragVisualClass != nullptr)
		{
			UED_EquipmentSlot* visual = CreateWidget<UED_EquipmentSlot>(GetWorld(), DragVisualClass);
			visual->ItemData = this->ItemData;
			visual->IconImage = this->ItemData.Icon;
			visual->ItemIcon->SetBrushFromTexture(this->ItemData.Icon);
			
			oper->DefaultDragVisual = visual;
		}

	}

}

bool UED_EquipmentSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{

	UED_WidgetDragDrop* oper = Cast<UED_WidgetDragDrop>(InOperation);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Drag End"));

	if (oper != nullptr)
	{
		//swap player invetory(fromnum , this slotnum);
		//Player->SwapInvenItem(oper->FromNum, this->SlotNum);
		
		// from -> inventory

		if (oper->FromLocate == EItemLocated::Inventory)
		{
			// 현재 슬롯 아이템을 oper 의 from 으로 옮김.
			// oper 의 아이템을 현재 슬롯으로 옮김.
			// 위치는 equip
			if (oper->InData.ItemType == this->ItemSlotType)
			{
				if (this->ItemData.ItemClass != nullptr)
				{
					FItemData tempdata = this->ItemData;
					//this->ItemData = tempdata;
					Player->Inventory[oper->FromNum] = this->ItemData;
					Player->EquipWeapon(oper->InData);
				}
				else
				{
					Player->EquipWeapon(oper->InData);
					Player->Inventory[oper->FromNum] = FItemData();
					Player->RefreshInventory();
				}
			}

		}
		Refresh();

		FVector2D LocalDragPosition;

		return true;
	}
	else if (oper == nullptr)
	{
		//AED_PlayerCharacter* PlayerCharacter = Cast<AED_PlayerCharacter>(GetOwningPlayer()->GetPawn());
		//if (ItemData.Class != nullptr)
		//{
		//	GetWorld()->SpawnActor<AED_Item>(ItemData.Class, PlayerCharacter->GetActorLocation(), FRotator());
		//	PlayerCharacter->Inventory[th]
		//	//PlayerCharacter->InventoryWidget->RefreshInventory(PlayerCharacter->Inventory);
		//}
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Out"));

	}
	//DropItem();

	return false;
}

void UED_EquipmentSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
}
