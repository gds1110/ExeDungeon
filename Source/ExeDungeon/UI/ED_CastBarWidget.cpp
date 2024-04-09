// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ED_CastBarWidget.h"
#include "Character/ED_PlayerCharacter.h"
#include "Components/ProgressBar.h"
#include "Kismet/KismetMathLibrary.h"

UED_CastBarWidget::UED_CastBarWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{

	bIsCasting = false;
	if (pbCastBar)
	{
		pbCastBar->SetPercent(0.f);
	}
}

void UED_CastBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UE_LOG(LogTemp, Log, TEXT("UITICK"));
}

void UED_CastBarWidget::StartUpdate()
{
	GetWorld()->GetTimerManager().SetTimer(CastBarHandle, this, &UED_CastBarWidget::UpdateBar, Interval, true);
	bIsCasting = true;
	SetVisibility(ESlateVisibility::Visible);
}

void UED_CastBarWidget::StopUpdate()
{
	GetWorld()->GetTimerManager().ClearTimer(CastBarHandle);
	pbCastBar->SetPercent(0.f);
	bIsCasting = false;
	SetVisibility(ESlateVisibility::Hidden);

}

void UED_CastBarWidget::UpdateBar()
{
	if (pbCastBar != nullptr)
	{

		auto Player = Cast<AED_PlayerCharacter>(OwningActor);
		if (Player)
		{
			float ElapsedValue = GetWorld()->GetTimerManager().GetTimerElapsed(Player->InteractTimerHandle);
			float RemaningValue = GetWorld()->GetTimerManager().GetTimerRemaining(Player->InteractTimerHandle);
		    float PercentValue =UKismetMathLibrary::NormalizeToRange(ElapsedValue, 0.f, RemaningValue);
			pbCastBar->SetPercent(PercentValue);
		}
	}
}

void UED_CastBarWidget::SetCastBarPercent(float InPercent)
{

}

void UED_CastBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Log, TEXT("NativeConstruct"));
	if (pbCastBar)
	{
		pbCastBar->SetPercent(0.f);

	}
		SetVisibility(ESlateVisibility::Hidden);
}
