// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/ED_Props.h"
#include "Components/BoxComponent.h"

// Sets default values
AED_Props::AED_Props()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AED_Props::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AED_Props::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AED_Props::InteractAction()
{
}

void AED_Props::InteractAction(AED_PlayerCharacter* player)
{
}

float AED_Props::GetInteractTime()
{
	return InteractTime;
}

FName AED_Props::GetInterName()
{
	return InterName;
}

bool AED_Props::GetIsInteract()
{
	return true;
}

