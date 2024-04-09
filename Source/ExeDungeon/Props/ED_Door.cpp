// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/ED_Door.h"
#include "Components/BoxComponent.h"
#include "Physics/EDCollision.h"
#include "Character/ED_PlayerCharacter.h"


AED_Door::AED_Door()
{
	if (BoxCollision != nullptr)
	{
		BoxCollision->SetCollisionProfileName(CPROFILE_EDINTERACT);
	}
	DoorWay = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorWay"));
	DoorLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorLeft"));
	DoorRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorRight"));
	DoorWay->SetCollisionProfileName(CPROFILE_EDINTERACT);
	DoorLeft->SetCollisionProfileName("BlockAllDynamic");
	DoorRight->SetCollisionProfileName("BlockAllDynamic");

	DoorLeft->SetupAttachment(DoorWay);
	DoorRight->SetupAttachment(DoorWay);
}

void AED_Door::InteractAction()
{
	if (bIsDoorClosed)
	{
		UE_LOG(LogTemp, Log, TEXT("Interact Door"));
		Timeline.Play();
	}
	else
	{
		Timeline.Reverse();
	}
	bIsDoorClosed = !bIsDoorClosed;
}

void AED_Door::InteractAction(AED_PlayerCharacter* player)
{
	const FVector MyLocation = GetActorLocation();
	const FVector PlayerLoca = player->GetActorLocation();

	FVector DoorForward = GetActorForwardVector();

	FVector DirectionToPlayer = PlayerLoca - MyLocation;
	
	
	DirectionToPlayer.Normalize();

	const float DotProduct = FVector::DotProduct(DoorForward, DirectionToPlayer);

	if (bIsDoorClosed)
	{

		IsFront = (DotProduct > 0) ? true : false;

		UE_LOG(LogTemp, Log, TEXT("Interact Door"));
		Timeline.Play();
	}
	else
	{
		Timeline.Reverse();
	}
	bIsDoorClosed = !bIsDoorClosed;
}

bool AED_Door::GetIsInteract()
{
	return true;
}

void AED_Door::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AED_Door::OpenDoor);
		Timeline.AddInterpFloat(CurveFloat, TimelineProgress);
	}
	
}

void AED_Door::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	Timeline.TickTimeline(DeltaTime);

}

void AED_Door::OpenDoor(float Value)
{
	FRotator Rot = FRotator(0.f, 90.f * Value, 0.f);
	FRotator MinusRot = FRotator(0.f, 90.f * -Value, 0.f);
	if (IsFront)
	{
		DoorLeft->SetRelativeRotation(Rot);
		DoorRight->SetRelativeRotation(MinusRot);
	}
	else
	{
		DoorLeft->SetRelativeRotation(MinusRot);
		DoorRight->SetRelativeRotation(Rot);
	}
}

void AED_Door::SetDoorOnSameSide()
{

}
