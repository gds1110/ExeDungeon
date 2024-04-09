// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ED_InventoryEntry.h"
#include "InventoryWidget.h"
#include "Item/ED_ItemDatabase.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Item/ED_Item.h"
#include "Character/ED_PlayerCharacter.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "ED_WidgetDragDrop.h"
#include "Item/ED_LootBoxUI.h"
#include "Item/ED_RootBox.h"

void UED_InventoryEntry::NativeConstruct()
{
	Super::NativeConstruct();

	//ItemIcon = Cast<UImage>(GetWidgetFromName(TEXT("ItemIcon")));

	//SlotButton = Cast<UButton>(GetWidgetFromName(TEXT("SlotBtn")));

	if (SlotBtn != nullptr)
	{
		SlotBtn->OnPressed.AddDynamic(this, &UED_InventoryEntry::ButtonDownCall);
	}
	if (AmountText != nullptr)
	{

		AmountText->SetVisibility(ESlateVisibility::Hidden);

	}
}

void UED_InventoryEntry::DropItem()
{
	
	if (ItemData.ItemClass != nullptr)
	{
		auto ditem =GetWorld()->SpawnActor<AED_Item>(ItemData.ItemClass, Player->playerFronVector, FRotator());
		ditem->ThisData = ItemData;
		Player->Inventory[SlotNum] = FItemData();
	}
	Player->RefreshInventory();
	//PlayerCharacter->Inventory.Remove(ItemData);
	//PlayerCharacter->InventoryWidget->RefreshInventory(PlayerCharacter->Inventory);
	//DropButton->OnClicked.Clear();


}

void UED_InventoryEntry::Refresh()
{
	
	

	if (AmountText != nullptr)
	{
		if (bIsStack == false)
		{
			AmountText->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			Amount = ItemData.Amount;
			AmountText->SetVisibility(ESlateVisibility::Visible);
			FString _text = "x " + FString::FromInt(ItemData.Amount);
			AmountText->SetText(FText::FromString(_text));
		}
	}
}

void UED_InventoryEntry::ClearSlot()
{

}

void UED_InventoryEntry::ButtonDownCall()
{
	FEventReply reply;

	//reply = UWidgetBlueprintLibrary::DetectDragIfPressed()

}



void UED_InventoryEntry::Init(FItemData _data, EItemLocated loc)
{
	ItemData = _data;
	ItemLocate = loc;
	
	if (ItemData.Icon == nullptr)
	{
		if (DefaultImg != nullptr)
		{
			IconImage = DefaultImg;
			ItemIcon->SetBrushFromTexture(DefaultImg);
		}
	}
	else if(ItemData.Icon!=nullptr)
	{
		IconImage = ItemData.Icon;
		ItemIcon->SetBrushFromTexture(IconImage);
	}

	if (ItemData.bIsStackable==EStackType::NoneStack)
	{
		bIsStack = false;
		AmountText->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		bIsStack = true;
		Amount = ItemData.Amount;
		AmountText->SetVisibility(ESlateVisibility::Visible);
		FString _text = "x " + FString::FromInt(ItemData.Amount);
		AmountText->SetText(FText::FromString(_text));
	}
}

FReply UED_InventoryEntry::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Click : click start"));

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		if (this->ItemLocate == EItemLocated::Inventory)
		{
			reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);

			return reply.NativeReply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
		}
	}
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) == true)
	{
		switch (this->ItemLocate)
		{
		case EItemLocated::Inventory:
			
			switch (this->ItemData.ItemType)
			{
			case EItemType::None:

				break;
			case EItemType::Weapon:
				// 장비가 있을경우 교체
				// 장비가 없을경우 장비칸으로 이동
				// 인벤토리에서 제거
				if (Player->WeaponSlot.ItemClass != nullptr)
				{
					//Swap
					FItemData tempData = Player->WeaponSlot;
					Player->EquipWeapon(this->ItemData);
					Player->Inventory[SlotNum] = tempData;
				}
				else
				{
					Player->EquipWeapon(this->ItemData);				
					Player->Inventory[SlotNum] = FItemData();
					
				}


				break;
			case EItemType::Armor:
				if (Player->ArmorSlot.ItemClass != nullptr)
				{
					//Swap
					FItemData tempData = this->ItemData;
					this->ItemData = Player->ArmorSlot;
					Player->EquipWeapon(this->ItemData);
				}
				else
				{
					Player->EquipWeapon(this->ItemData);
					Player->Inventory[SlotNum] = FItemData();

				}
				break;
			case EItemType::Charm:
				
				break;
			case EItemType::Scroll:
				// 사용
				break;
			case EItemType::Potion:
				Player->HealHp(20.0f);
				ItemData.Amount = ItemData.Amount - 1;
				Amount = Amount - 1;
				Player->Inventory[SlotNum] = ItemData;
				if (this->ItemData.Amount < 1)
				{
					Player->Inventory[SlotNum] = FItemData();
				}

				// 사용
				break;
			case EItemType::Gold:
				
				break;
			default:
				break;
			}

			break;
		case EItemLocated::RootBox:
			InLocatedLootBoxAction();
			break;
		case EItemLocated::Equipment:
			
			break;
		case EItemLocated::Ground:
			
			break;
		default:
			break;
		}

		//리프레쉬
		Player->RefreshInventory();
		Player->RefreshEquipmentWindow();

		return reply.NativeReply.Handled().DetectDrag(TakeWidget(), EKeys::RightMouseButton);

	}
	
	return reply.NativeReply;
}

void UED_InventoryEntry::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (this->ItemData.ItemClass==nullptr)
	{
		return;
	}

	if (OutOperation == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Draging start"));

		UED_WidgetDragDrop* oper = NewObject<UED_WidgetDragDrop>();
		OutOperation = oper;
		oper->FromNum = this->SlotNum;
		oper->InData = this->ItemData;
		oper->FromSlot = this;
		if (DragVisualClass != nullptr)
		{
			UED_InventoryEntry* visual = CreateWidget<UED_InventoryEntry>(GetWorld(), DragVisualClass);
			visual->SlotNum = this->SlotNum;
			visual->ItemData = this->ItemData;
			visual->IconImage = this->ItemData.Icon;
			visual->ItemIcon->SetBrushFromTexture(this->ItemData.Icon);

			if (visual->ItemData.bIsStackable == EStackType::NoneStack)
			{
				visual->AmountText->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				visual->AmountText->SetVisibility(ESlateVisibility::Visible);
				FString _text = "x " + FString::FromInt(this->ItemData.Amount);
				visual->AmountText->SetText(FText::FromString(_text));
			}

			oper->DefaultDragVisual = visual;
		}
		
	}
}

bool UED_InventoryEntry::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UED_WidgetDragDrop* oper = Cast<UED_WidgetDragDrop>(InOperation);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Drag End"));

	if (oper != nullptr)
	{
		
		//swap player invetory(fromnum , this slotnum);
		if (oper->FromLocate == EItemLocated::Inventory)
		{
			Player->SwapInvenItem(oper->FromNum, this->SlotNum);
			Player->RefreshInventory();

		}
		if (oper->FromLocate == EItemLocated::Equipment)
		{
			if (this->ItemData.ID!="None")
			{
				//Swap
				if (this->ItemData.ItemType == oper->InData.ItemType)
				{
					if (this->ItemData.ItemType == EItemType::Weapon)
					{
						Player->WeaponSlot = this->ItemData;
						Player->EquipWeapon(this->ItemData);
						this->ItemData = oper->InData;
						Player->Inventory[this->SlotNum] = oper->InData;


					}
					if (this->ItemData.ItemType == EItemType::Armor)
					{
						Player->ArmorSlot = this->ItemData;
						this->ItemData = oper->InData;
						Player->Inventory[this->SlotNum] = oper->InData;
					}
					
				
				}


			}
			else if(this->ItemData.ID == "None")
			{
					this->ItemData = oper->InData;
					Player->Inventory[this->SlotNum] = oper->InData;
				if (oper->InData.ItemType == EItemType::Weapon)
				{
					Player->WeaponSlot = FItemData();
					Player->EquipWeaponClear();
				}

				if (oper->InData.ItemType == EItemType::Armor)
				{
					Player->Inventory[this->SlotNum] = oper->InData;
					Player->ArmorSlot = FItemData();
				}
			}
			Player->RefreshEquipmentWindow();
			Player->RefreshInventory();
		}
		
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
	DropItem();

	return false;
}

void UED_InventoryEntry::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("InMouse"));

}

void UED_InventoryEntry::InLocatedLootBoxAction()
{
	if (Player == nullptr)
	{
		Player = Cast<AED_PlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		this->Player = Cast<AED_PlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	}
	if (this->ItemLocate == EItemLocated::RootBox)
	{
		auto Inventory = Player->Inventory;

		if (Inventory.Contains(ItemData))
		{
			if (ItemData.bIsStackable == EStackType::Stackable)
			{
				for (int32 i = 0; i < Player->Inventory.Num(); i++)
				{
					if (Player->Inventory[i] == ItemData)
					{
						Player->Inventory[i].Amount += ItemData.Amount;
						Player->RefreshInventory();
						this->SetVisibility(ESlateVisibility::Hidden);
						this->SetIsEnabled(false);
						Player->CurrentBox->RemoveData(ItemData);

						return;
					}
				}
			}
			else
			{
				for (int32 i = 0; i < Player->Inventory.Num(); i++)
				{
					if (Player->Inventory[i].ItemType == EItemType::None)
					{
						Player->Inventory[i] = ItemData;
						Player->RefreshInventory();

						this->SetVisibility(ESlateVisibility::Hidden);
						this->SetIsEnabled(false);
						Player->CurrentBox->RemoveData(ItemData);

						return;
					}
				}
			}
		}
		else
		{
			for (int32 i = 0; i < Player->Inventory.Num(); i++)
			{
				if (Player->Inventory[i].ItemType == EItemType::None)
				{
					Player->Inventory[i] = ItemData;
					Player->RefreshInventory();

					this->SetVisibility(ESlateVisibility::Hidden);
					this->SetIsEnabled(false);
					Player->CurrentBox->RemoveData(ItemData);

					return;
				}
			}
		}
	}

}

void UED_InventoryEntry::InLocatedInventoryAction()
{
	//right mouse down
	if (Player == nullptr)
	{
		Player = Cast<AED_PlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		this->Player = Cast<AED_PlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	}
	if (this->ItemLocate == EItemLocated::Inventory)
	{
		auto Inventory = Player->Inventory;
		switch (this->ItemData.ItemType)
		{
		case EItemType::None:
			break;
		case EItemType::Weapon:
			// 착용
			break;
		case EItemType::Armor:
			// 착용
			break;
		case EItemType::Charm:
			break;
		case EItemType::Scroll:
			break;
		case EItemType::Potion:
			// 회복
			break;
		case EItemType::Gold:
			break;
		default:
			break;
		}
	}
}

void UED_InventoryEntry::InLocatedEquipmentAction()
{
	//right mouse down
	

}

void UED_InventoryEntry::AddToInventory(FItemData _Data)
{
	if (Player != nullptr)
	{
		auto Inventory = Player->Inventory;

		if (Inventory.Contains(_Data))
		{
			if (_Data.bIsStackable == EStackType::Stackable)
			{
				for (int32 i = 0; i < Player->Inventory.Num(); i++)
				{
					if (Player->Inventory[i] == _Data)
					{
						Player->Inventory[i].Amount += _Data.Amount;
						Player->RefreshInventory();
						break;
					}
				}
			}
			else
			{
				for (int32 i = 0; i < Player->Inventory.Num(); i++)
				{
					if (Player->Inventory[i].ItemType == EItemType::None)
					{
						Player->Inventory[i] = _Data;
						Player->RefreshInventory();
						break;
					}
				}
			}
		}
		else
		{
			for (int32 i = 0; i < Player->Inventory.Num(); i++)
			{
				if (Player->Inventory[i].ItemType == EItemType::None)
				{
					Player->Inventory[i] = _Data;
					Player->RefreshInventory();
					break;
				}
			}
		}
	}
}
