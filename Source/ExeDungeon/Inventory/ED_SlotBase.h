// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ED_SlotBase.generated.h"


class UTextBlock;
class UButton;
class UImage;


/**
 * 
 */
UCLASS()
class EXEDUNGEON_API UED_SlotBase : public UUserWidget
{

	GENERATED_BODY()

public:


	UPROPERTY()
	class AED_PlayerCharacter* Player;

	void SetPlayer(class AED_PlayerCharacter* _Player);


	UPROPERTY(EditAnywhere)
	FItemData ItemData;
protected:
	UPROPERTY(EditAnywhere)
	UTexture2D* DefaultImg;

	// 격자 간격의 엔벤토리로 변경예정 및 이미지도 추가
	UPROPERTY()
	UTexture2D* IconImage;

	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;

	UPROPERTY()
	UTextBlock* ItemName;

	UPROPERTY()
	UButton* DropButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SlotBtn;

	int32 Amount;


	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmountText;

	UPROPERTY(EditAnywhere)
	EItemLocated ItemLocate;

	bool bIsStack=false;
public:
	FORCEINLINE EItemLocated GetSlotLocate() { return ItemLocate; }
	FORCEINLINE void SetSlotLocate(EItemLocated _locate) { ItemLocate = _locate; }

protected:
	virtual void NativeConstruct() override;

	virtual void Refresh();
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;


	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

};
