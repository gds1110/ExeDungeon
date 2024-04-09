// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "Item/ED_ItemDatabase.h"
#include "ED_WidgetDragDrop.generated.h"

/**
 * 
 */
UCLASS()
class EXEDUNGEON_API UED_WidgetDragDrop : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	int FromNum;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FItemData InData;
	UPROPERTY()
	class UED_InventoryEntry* FromSlot;
	UPROPERTY()
	EItemLocated FromLocate;
};
