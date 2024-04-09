// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ED_Widget.h"
#include "ED_HpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXEDUNGEON_API UED_HpBarWidget : public UED_Widget
{
	GENERATED_BODY()
	
public:
	UED_HpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	 virtual void NativeConstruct() override;


public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateBar(float NewCurrentHp);
protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpBar;
	UPROPERTY()
	float MaxHp;
};
