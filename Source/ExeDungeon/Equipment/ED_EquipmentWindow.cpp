// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/ED_EquipmentWindow.h"
#include "Inventory/ED_EquipmentSlot.h"
#include "Inventory/ED_SlotBase.h"
#include "Character/ED_PlayerCharacter.h"
#include "Components/TextBlock.h"
#include "CharacterStat/ED_CharacterStatComponent.h"

void UED_EquipmentWindow::Refresh()
{
	if (WeaponSlot)
	{
		WeaponSlot->Refresh();
	}
	if (ArmorSlot)
	{
		ArmorSlot->Refresh();
	}
	
	if (Player)
	{
	
		FString AtKTxt = FString::Printf(TEXT("ATK : %f "), Player->GetStat()->GetTotalStat().Attack);
		FString SpdTxt = FString::Printf(TEXT("ATKSPD : %f"), Player->GetStat()->GetTotalStat().AttackSpeed);
		FString MaxHpTxt = FString::Printf(TEXT("HP : %f"), Player->GetStat()->GetTotalStat().MaxHp);
	
		AtkText->SetText(FText::FromString(AtKTxt));
		SpdText->SetText(FText::FromString(SpdTxt));
		HpText->SetText(FText::FromString(MaxHpTxt));



	}
}

void UED_EquipmentWindow::Init(AED_PlayerCharacter* _Player)
{

	WeaponSlot->SetPlayer(_Player);
	ArmorSlot->SetPlayer(_Player);
	Player = _Player;

}

void UED_EquipmentWindow::NativeConstruct()
{
	Super::NativeConstruct();

	WeaponSlot->SlotType = EEquipSlotType::Weapon;
	WeaponSlot->ItemSlotType = EItemType::Weapon;
	ArmorSlot->SlotType = EEquipSlotType::Armor;
	ArmorSlot->ItemSlotType = EItemType::Armor;


	

}
