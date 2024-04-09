// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ED_HUDWidget.h"
#include "UI/ED_HpBarWidget.h"
#include "Interface/ED_HUDInterface.h"
#include "ED_WidgetDragDrop.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Inventory/ED_InventoryEntry.h"
#include "UI/ED_CastBarWidget.h"

UED_HUDWidget::UED_HUDWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UED_HUDWidget::UpdateHpBar(float NewCurrentHp)
{
	if (HpBar != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("HPBAR IS NOT NULL"));
		HpBar->UpdateBar(NewCurrentHp);
	}
}

void UED_HUDWidget::HudInit()
{
	IED_HUDInterface* HudPawn = Cast<IED_HUDInterface>(GetOwningPlayerPawn());
	if (HudPawn)
	{
		HudPawn->SetupHUDWidget(this);
		HpBar->SetOwningActor(OwningActor);
		CastBar->SetOwningActor(OwningActor);
	}
}

void UED_HUDWidget::SetMaxHp(float MaxHp)
{

	HpBar->SetMaxHp(MaxHp);

}

void UED_HUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UED_HUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}


bool UED_HUDWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UED_WidgetDragDrop* ItemDrop = Cast<UED_WidgetDragDrop>(InOperation);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Drag End"));

	if (ItemDrop)
	{
		if (ItemDrop->InData.ItemClass != nullptr)
		{
			ItemDrop->FromSlot->DropItem();
		}
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Drag true"));

		return true;
	}

	return false;
}

//FReply UED_HUDWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	FEventReply reply;
//	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Click : click Hud start"));
//
//	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
//	{
//		reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
//
//		return reply.NativeReply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
//	}
//
//	return reply.NativeReply;
//}
//
//void UED_HUDWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Mouse In Hud"));
//
//}
//
//void UED_HUDWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
//{
//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Mouse Out Hud"));
//
//	Super::NativeOnMouseLeave(InMouseEvent);
//
//}
