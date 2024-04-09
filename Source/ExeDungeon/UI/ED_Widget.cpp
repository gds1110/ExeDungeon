// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ED_Widget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "ED_WidgetDragDrop.h"


bool UED_Widget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    return false;
}

FReply UED_Widget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Click : click Hud start"));

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true)
	{
		reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);

		return reply.NativeReply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	return reply.NativeReply;
}

void UED_Widget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	FString tempstring = this->GetName() + " enter";
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, tempstring);
}

void UED_Widget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	FString tempstring = this->GetName() + " Out";

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, tempstring);

	Super::NativeOnMouseLeave(InMouseEvent);
}
