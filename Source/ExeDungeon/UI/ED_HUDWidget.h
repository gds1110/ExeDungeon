// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ED_Widget.h"
#include "ED_HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXEDUNGEON_API UED_HUDWidget : public UED_Widget
{
	GENERATED_BODY()
	
	
public:
	UED_HUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	void UpdateHpBar(float NewCurrentHp);
	void HudInit();
	void SetMaxHp(float MaxHp);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UED_CastBarWidget> CastBar;
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UED_HpBarWidget> HpBar;



private:
	void NativeOnInitialized();
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
};
