// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ED_HpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/ED_CharacterWidgetInterface.h"

UED_HpBarWidget::UED_HpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1;
}

void UED_HpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpBar);

	IED_CharacterWidgetInterface* CharacterWidget = Cast<IED_CharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}

}

void UED_HpBarWidget::UpdateBar(float NewCurrentHp)
{

	UE_LOG(LogTemp, Log, TEXT("MaxHp = %f"), MaxHp);
	ensure(MaxHp > 0.0f);
	if (HpBar)
	{
		HpBar->SetPercent(NewCurrentHp / MaxHp);
	}
}
