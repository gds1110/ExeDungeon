// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ED_MainWidget.h"
#include "Inventory/InventoryWidget.h"
#include "Item/ED_LootBoxUI.h"
#include "Equipment/ED_EquipmentWindow.h"
#include "UI/ED_HUDWidget.h"


UED_MainWidget::UED_MainWidget()
{
	static ConstructorHelpers::FClassFinder<UED_HUDWidget> EDHUDRef(TEXT("/Game/Dungeon/Blueprint/UI/WBP_EDHUD.WBP_EDHUD_C"));
	if (EDHUDRef.Class)
	{
		HUDWdigetClass = EDHUDRef.Class;
	}
}

void UED_MainWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UED_MainWidget::Init()
{
	//InventoryWidget = CreateWidget<UInventoryWidget>(Cast<APlayerController>(GetController()), InventroyWidgetClass);
	//InventoryWidget->SetPlayer(this);
	//InventoryWidget->Init();

	EDHUDWdiget = CreateWidget<UED_HUDWidget>(this, HUDWdigetClass);
	if (EDHUDWdiget)
	{
		EDHUDWdiget->AddToViewport();
		EDHUDWdiget->SetOwningActor(Cast<AActor>(Player));
		EDHUDWdiget->HudInit();
	}

	InventoryWidget->SetPlayer(this->Player);
	InventoryWidget->Init();

	InventoryWidget->AddToViewport(0);
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);

	//InteractWidget = CreateWidget(Cast<APlayerController>(GetController()), InteractWidgetClass);
	InteractWidget->AddToViewport(0);
	InteractWidget->SetVisibility(ESlateVisibility::Collapsed);

	LootBox->AddToViewport(0);
	LootBox->SetVisibility(ESlateVisibility::Collapsed);

	EquipWidget->AddToViewport(0);
	EquipWidget->SetVisibility(ESlateVisibility::Collapsed);
	EquipWidget->Init(this->Player);


	

}

void UED_MainWidget::RefreshWidgets()
{
}

void UED_MainWidget::RefreshEquipWindow()
{

	EquipWidget->Refresh();
	
}

void UED_MainWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag End In Out"));


}
