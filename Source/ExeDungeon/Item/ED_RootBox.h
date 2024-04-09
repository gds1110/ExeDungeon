// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ED_InteractInterface.h"
#include "ED_ItemDatabase.h"
#include "ED_RootBox.generated.h"

UCLASS()
class EXEDUNGEON_API AED_RootBox : public AActor, public IED_InteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AED_RootBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void InteractAction() override;
	virtual void InteractAction(class AED_PlayerCharacter* player) override;
	virtual FName GetInterName() override;
	virtual float GetInteractTime() override;
	virtual bool GetIsInteract() override;
	void MakeRandomLoots();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LootBox)
	TArray<FItemData> LootItems;
	UPROPERTY(EditAnywhere, Category = Interact)
	float InteractTime = 0;
	void RemoveData(FItemData _data);
	UPROPERTY(EditAnywhere)
	FName InterName;
};
