// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ED_Widget.h"
#include "ED_MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXEDUNGEON_API UED_MainWidget : public UED_Widget
{



	GENERATED_BODY()
public:
	UED_MainWidget();
protected:
	virtual void NativeConstruct() override;
public:
	void Init();
	
	void RefreshWidgets();
	void RefreshEquipWindow();

public:
	UPROPERTY(EditAnywhere,meta = (BindWidget), Category = Inventory)
	class UInventoryWidget* InventoryWidget;
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = Equipment)
	class UED_EquipmentWindow* EquipWidget;

	UPROPERTY()
	class AED_PlayerCharacter* Player;
	UPROPERTY(EditAnywhere,meta = (BindWidget), Category = Interact)
	UUserWidget* InteractWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LootBox, meta = (BindWidget))
	class UED_LootBoxUI* LootBox;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	// HUD Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UED_HUDWidget> HUDWdigetClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TObjectPtr<class UED_HUDWidget> EDHUDWdiget;

};
