// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ED_Widget.h"
#include "ED_ItemDatabase.h"
#include "ED_LootBoxUI.generated.h"

/**
 * 
 */


UCLASS()
class EXEDUNGEON_API UED_LootBoxUI : public UED_Widget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* LootWindow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UED_InventoryEntry> DragVisualClass;
	UPROPERTY(EditAnywhere)
	FName InterName;
	void AddSlots();
	void AddItems(FItemData _item);
	void RefreshSlots();
};
