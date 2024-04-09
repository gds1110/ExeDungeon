// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ED_ItemDatabase.h"
#include "ED_EquipmentWindow.generated.h"


class UED_EquipmentSlot;

UCLASS()
class EXEDUNGEON_API UED_EquipmentWindow : public UUserWidget
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = Slot)
	UED_EquipmentSlot* WeaponSlot;
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = Slot)
	UED_EquipmentSlot* ArmorSlot;

public:
	void Refresh();
	void Init(class AED_PlayerCharacter* _Player);
	class AED_PlayerCharacter* Player;
protected:
	virtual void NativeConstruct() override;
		
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = Txt)
	class UTextBlock* AtkText;
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = Txt)
	class UTextBlock* SpdText;
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = Txt)
	class UTextBlock* HpText;
};
