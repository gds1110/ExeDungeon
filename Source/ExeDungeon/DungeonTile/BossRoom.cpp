// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonTile/BossRoom.h"
#include "DungeonTile/ED_DungeonTile.h"

// Sets default values
ABossRoom::ABossRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABossRoom::BeginPlay()
{
	Super::BeginPlay();
	
	//if (BaseTileClass != nullptr)
	//{
	//	//DoorTile = GetWorld()->SpawnActor<AED_DungeonTile>(BaseTileClass, EntryVector, FRotator::ZeroRotator);
	//
	//	Orbit = NewObject<UChildActorComponent>(this);
	//	Orbit->bEditableWhenInherited = true;
	//	Orbit->RegisterComponent();
	//	Orbit->AttachToComponent(this->RootComponent,FAttachmentTransformRules::SnapToTargetIncludingScale);
	//	Orbit->SetChildActorClass(BaseTileClass);
	//	Orbit->CreateChildActor();
	//	//Orbit->SetRelativeLocation(EntryVector);


	//	if (Cast<AED_DungeonTile>(Orbit->GetChildActor()))
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("Orbit is TILE"));

	//		DoorTile = Cast<AED_DungeonTile>(Orbit->GetChildActor());
	//		if (DoorTile != nullptr)
	//		{
	//			UE_LOG(LogTemp, Log, TEXT("DoorTile is TILE"));

	//		}
	//	}

	//	Orbit->SetRelativeLocation(EntryVector);

	//	UE_LOG(LogTemp, Log, TEXT("BossRoom Entry X : %f , Y : %f , Z : %f"), DoorTile->GetActorLocation().X, DoorTile->GetActorLocation().Y, DoorTile->GetActorLocation().Z);
	//	
	//}
	/*if (DoorTile != nullptr)
	{
		DoorTile->SetActorRelativeLocation(EntryVector);
	}*/

}

// Called every frame
void ABossRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

