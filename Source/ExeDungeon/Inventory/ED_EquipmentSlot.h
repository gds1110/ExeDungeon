// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/ED_SlotBase.h"
#include "ED_EquipmentSlot.generated.h"

/**
 * 
 */



UCLASS()
class EXEDUNGEON_API UED_EquipmentSlot : public UED_SlotBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Slot Data")
	EEquipSlotType SlotType;
	UPROPERTY(EditAnywhere, Category = "Slot Data")
	EItemType ItemSlotType;
public:
	virtual void Refresh() override;

protected:
	virtual void NativeConstruct() override;
protected:
	//부모 클래스로
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;


	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UED_EquipmentSlot> DragVisualClass;
};
