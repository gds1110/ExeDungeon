// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossRoom.generated.h"

UCLASS()
class EXEDUNGEON_API ABossRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossRoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class AED_DungeonTile> DoorTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	UChildActorComponent* Orbit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector EntryVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AED_DungeonTile> BaseTileClass;
};
