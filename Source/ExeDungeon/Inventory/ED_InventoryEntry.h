// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ED_SlotBase.h"
#include "ED_InventoryEntry.generated.h"

/**
 * 
 */

class UTextBlock;
class UButton;
class UImage;




UCLASS()
class EXEDUNGEON_API UED_InventoryEntry : public UED_SlotBase
{
	GENERATED_BODY()

public:


protected:
	virtual void NativeConstruct() override;

private:
	
protected:
	//void NativeOnDragDetected

public:
	int32 SlotNum;

	UFUNCTION()
	void DropItem();
	
	UFUNCTION()
	void Refresh();	

	UFUNCTION()
	void ClearSlot();

	UFUNCTION(BlueprintCallable)
	void ButtonDownCall();

	UFUNCTION()
	void Init(FItemData _data,EItemLocated loc);

	bool IsOut;

protected:
	//부모 클래스로
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;


	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UED_InventoryEntry> DragVisualClass;
	
	void InLocatedLootBoxAction();
	void InLocatedInventoryAction();
	void InLocatedEquipmentAction();
	void AddToInventory(FItemData _Data);
};
