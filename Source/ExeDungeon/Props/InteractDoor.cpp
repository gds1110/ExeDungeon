// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/InteractDoor.h"

// Sets default values
AInteractDoor::AInteractDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

