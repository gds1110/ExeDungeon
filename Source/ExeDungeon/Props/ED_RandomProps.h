// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Props/ED_Props.h"
#include "Engine/StreamableManager.h"
#include "ED_RandomProps.generated.h"

/**
 * 
 */
UCLASS(config=ExeDungeon)
class EXEDUNGEON_API AED_RandomProps : public AED_Props
{
	GENERATED_BODY()
	
public:
	AED_RandomProps();
protected:
	virtual void PostInitializeComponents() override;
	void PropMeshLoadCompleted();

public:

	UPROPERTY(config)
	TArray<FSoftObjectPath> RandomMeshs;

	UPROPERTY(EditAnywhere, Category = RandomMesh)
	UStaticMeshComponent* MeshComp;

	virtual bool GetIsInteract() override;

	TSharedPtr<FStreamableHandle> PropMeshHandle;
};
