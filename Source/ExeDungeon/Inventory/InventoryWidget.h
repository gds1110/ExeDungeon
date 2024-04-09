// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ED_ItemDatabase.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */

class UListView;
class UED_InventoryEntry;
class UUniformGridPanel;

UCLASS()
class EXEDUNGEON_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void RefreshInventory(const TArray<FItemData>& Inventory);
	void Init();
	void SetPlayer(class AED_PlayerCharacter* _Player);

	int32 Row;
	int32 Col;

	UPROPERTY()
	class AED_PlayerCharacter* Player;
protected:
	virtual void NativeConstruct() override;
private:
	UPROPERTY(EditAnywhere)
	UTexture2D* DefaultImg;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UUniformGridPanel* InventoryPanel;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UED_InventoryEntry> SlotClass;
	
	TArray<UED_InventoryEntry*> ItemSlots;
};
