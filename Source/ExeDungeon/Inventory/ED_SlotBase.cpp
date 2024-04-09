// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ED_SlotBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "ED_WidgetDragDrop.h"
#include "Components/Image.h"
#include "Item/ED_ItemDatabase.h"


void UED_SlotBase::SetPlayer(AED_PlayerCharacter* _Player)
{
	
		Player = _Player;
	
}

void UED_SlotBase::NativeConstruct()
{

	Super::NativeConstruct();
	if (DefaultImg != nullptr)
	{
		if (ItemIcon != nullptr)
		{
			if (IconImage != nullptr)
			{
				ItemIcon->SetBrushFromTexture(IconImage);
			}
			else
			{
				ItemIcon->SetBrushFromTexture(DefaultImg);
			}
		}

	}
	//AmountText = Cast<UTextBlock>(GetWidgetFromName(TEXT("AmountText")));

}

void UED_SlotBase::Refresh()
{

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

FReply UED_SlotBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	return reply.NativeReply.Handled().DetectDrag(TakeWidget(), EKeys::RightMouseButton);
}

void UED_SlotBase::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
}

bool UED_SlotBase::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    return false;
}

void UED_SlotBase::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
}
