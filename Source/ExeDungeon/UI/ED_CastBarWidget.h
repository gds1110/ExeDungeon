// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ED_Widget.h"
#include "ED_CastBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXEDUNGEON_API UED_CastBarWidget : public UED_Widget
{
	GENERATED_BODY()
public:
	UED_CastBarWidget(const FObjectInitializer& ObjectInitializer);
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> pbCastBar;
public:
	virtual void NativeTick(const FGeometry& MyGeometry,float InDeltaTime) override;
	bool bIsCasting;

	void StartUpdate();
	void StopUpdate();
	void UpdateBar();

	void SetCastBarPercent(float InPercent);
private:
	FTimerHandle CastBarHandle;
	float Interval = 0.01f;

protected:
	virtual void NativeConstruct() override;

};
