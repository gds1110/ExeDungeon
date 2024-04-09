// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonTile/ED_DungeonTile.h"
#include "Components/BoxComponent.h"
#include "Props/ED_Door.h"
#include "Props/ED_Props.h"
#include "Props/ED_RandomProps.h"
#include "Props/ED_WallTorch.h"


// Sets default values
AED_DungeonTile::AED_DungeonTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	TileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMeshComp"));
	LeftWallMeshComponent= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftWallMeshComp"));
	RightWallMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightWallMeshComp"));
	UpWallMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UpWallMeshComp"));
	DownWallMeshComponent= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DownWallMeshComp"));
	RootComponent = TileMeshComponent;
	TileBoxComponent->SetupAttachment(TileMeshComponent);
	LeftWallMeshComponent->SetupAttachment(TileMeshComponent);
	RightWallMeshComponent->SetupAttachment(TileMeshComponent);
	UpWallMeshComponent->SetupAttachment(TileMeshComponent);
	DownWallMeshComponent->SetupAttachment(TileMeshComponent);

	LeftWallMeshComponent->SetHiddenInGame(true);
	RightWallMeshComponent->SetHiddenInGame(true);
	UpWallMeshComponent->SetHiddenInGame(true);
	DownWallMeshComponent->SetHiddenInGame(true);

	if (TileBoxComponent != nullptr)
	{
		TileBoxComponent->SetEnableGravity(false);
	}
	bIsForward = false;
	bIsBack = false;
	bIsLeft = false;
	bIsRight = false;
	bIsVisited = false;
	bIsOutline = false;
	bIsPath = false;
	bIsDoor = false;
	bIsRoom = false;
	bIsBossRoom = false;
	bIsAlreadySetting = false;
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempWallMesh(TEXT("/Script/Engine.StaticMesh'/Game/MedievalDungeon/Meshes/Architecture/Dungeon/SM_Dungeon_Wall.SM_Dungeon_Wall'"));
	if (TempWallMesh.Object)
	{
		WallMesh = TempWallMesh.Object;
	}
	if (WallMesh != nullptr)
	{
		LeftWallMeshComponent->SetStaticMesh(WallMesh);
		RightWallMeshComponent->SetStaticMesh(WallMesh);
		UpWallMeshComponent->SetStaticMesh(WallMesh);
		DownWallMeshComponent->SetStaticMesh(WallMesh);
	}

	static ConstructorHelpers::FClassFinder<AED_RandomProps> RandomPropClassRef(TEXT("/Game/Dungeon/Blueprint/Prop/BP_RandomProps.BP_RandomProps_C"));
	if (RandomPropClassRef.Class)
	{
		RandomPropClass = RandomPropClassRef.Class;
	}
	static ConstructorHelpers::FClassFinder<AED_WallTorch> TorchClassRef(TEXT("/Game/Dungeon/Blueprint/InteractProp/BP_Torch1.BP_Torch1_C"));
	if (TorchClassRef.Class)
	{
		TorchClass = TorchClassRef.Class;
	}


	ChildChandalier = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildChandalierComp"));
	ChildChandalier->SetupAttachment(TileMeshComponent);
	ChildChandalier->SetRelativeLocation(FVector(300, 300, 640));

	ChildChandalier->SetHiddenInGame(true);

	/*ChildTorch = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildTorchComp"));
	ChildTorch->SetChildActorClass(TorchClass);
	ChildTorch->SetupAttachment(TileMeshComponent);

	ChildTorch->SetHiddenInGame(true);*/
}

// Called when the game starts or when spawned
void AED_DungeonTile::BeginPlay()
{
	Super::BeginPlay();
	//CheckDirectionWall();

}

// Called every frame
void AED_DungeonTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AED_DungeonTile::CheckDirectionWall()
{
	/*if (bIsBossRoom)
	{
		return;
	}*/

	if (bIsAlreadySetting)
	{
		return;
	}

	const FVector Directions[] = { FVector::ForwardVector,FVector::BackwardVector,FVector::LeftVector,FVector::RightVector };
	const float CastDistance = 600.0f;

	LeftWallMeshComponent->SetHiddenInGame(true);
	LeftWallMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightWallMeshComponent->SetHiddenInGame(true);
	RightWallMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UpWallMeshComponent->SetHiddenInGame(true);
	UpWallMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DownWallMeshComponent->SetHiddenInGame(true);
	DownWallMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bIsOutline = false;
	auto UpChildComps = UpWallMeshComponent->GetAttachChildren();
	for (int i = 0; i < UpChildComps.Num(); i++)
	{
		UpChildComps[i]->SetHiddenInGame(true);
		UpChildComps[i]->OnActorEnableCollisionChanged();
		UStaticMeshComponent* TempComp = Cast<UStaticMeshComponent>(UpChildComps[i]);
		if (TempComp != nullptr)
		{
			TempComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}	
	auto RightChildComps = RightWallMeshComponent->GetAttachChildren();
	for (int i = 0; i < RightChildComps.Num(); i++)
	{
		RightChildComps[i]->SetHiddenInGame(true);
		RightChildComps[i]->OnActorEnableCollisionChanged();
		UStaticMeshComponent* TempComp = Cast<UStaticMeshComponent>(RightChildComps[i]);
		if (TempComp != nullptr)
		{
			TempComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

	}	
	auto LeftChildComps = LeftWallMeshComponent->GetAttachChildren();
	for (int i = 0; i < LeftChildComps.Num(); i++)
	{
		LeftChildComps[i]->SetHiddenInGame(true);
		LeftChildComps[i]->OnActorEnableCollisionChanged();
		UStaticMeshComponent* TempComp = Cast<UStaticMeshComponent>(LeftChildComps[i]);
		if (TempComp != nullptr)
		{
			TempComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

	}	
	auto DownChildComps = DownWallMeshComponent->GetAttachChildren();
	for (int i = 0; i < DownChildComps.Num(); i++)
	{
		DownChildComps[i]->SetHiddenInGame(true);
		DownChildComps[i]->OnActorEnableCollisionChanged();
		UStaticMeshComponent* TempComp = Cast<UStaticMeshComponent>(DownChildComps[i]);
		if (TempComp != nullptr)
		{
			TempComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

	}


	for (const FVector& Direction : Directions)
	{
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this); 
		FVector BoxLocation = TileBoxComponent->GetComponentLocation();
		FVector StartLocation = BoxLocation;
		FVector EndLocation = StartLocation + (Direction * CastDistance);
		FHitResult OutHitResult;

		bool bHit = GetWorld()->LineTraceSingleByChannel
		(
			OutHitResult,
			StartLocation,
			EndLocation,
			ECollisionChannel::ECC_GameTraceChannel2,
			CollisionParams
		);

#if ENABLE_DRAW_DEBUG
		DrawDebugLine(
			GetWorld(),
			StartLocation,
			EndLocation,
			FColor::Red,
			false,
			150.0f,
			0,
			15
		);
#endif
		FVector ExtentBox = FVector(10, 10, 600);

		if (bHit==true)
		{

			AActor* HitActor = OutHitResult.GetActor();
			//UE_LOG(LogTemp, Log, TEXT("Start Actor :%s  Hit Actor Name : %s"), *GetName(), *HitActor->GetName());
			AED_DungeonTile* HitTile = Cast<AED_DungeonTile>(HitActor);
			if (HitTile)
			{
				
				if (Direction == FVector::ForwardVector)
				{
					//UE_LOG(LogTemp, Log, TEXT("%s : ForwardVector"), *GetName());
					if (!(this->bIsPath == true && HitTile->bIsBossRoom))
					{

					bIsForward = true;
					}
					NeighborTile tempTile;
					tempTile.Direction = ETileDirection::Forward;
					tempTile.NeighborTile = HitTile;
					NeighborTiles.Add(tempTile);
				}
				if (Direction == FVector::BackwardVector)
				{
					//	UE_LOG(LogTemp, Log, TEXT("%s : BackwardVector"), *GetName());
					if (!(this->bIsPath == true && HitTile->bIsBossRoom))
					{

					bIsBack = true;
					}
					NeighborTile tempTile;
					tempTile.Direction = ETileDirection::Back;
					tempTile.NeighborTile = HitTile;
					NeighborTiles.Add(tempTile);
				}
				if (Direction == FVector::LeftVector)
				{
					//UE_LOG(LogTemp, Log, TEXT("%s : LeftVector"), *GetName());
					if (!(this->bIsPath == true && HitTile->bIsBossRoom))
					{

					bIsLeft = true;				
					}
					NeighborTile tempTile;
					tempTile.Direction = ETileDirection::Left;
					tempTile.NeighborTile = HitTile;
					NeighborTiles.Add(tempTile);
				}
				if (Direction == FVector::RightVector)
				{
					//	UE_LOG(LogTemp, Log, TEXT("%s : RightVector"), *GetName());
					if (!(this->bIsPath == true && HitTile->bIsBossRoom))
					{

					bIsRight = true;		
					}
					NeighborTile tempTile;
					tempTile.Direction = ETileDirection::Right;
					tempTile.NeighborTile = HitTile;
					NeighborTiles.Add(tempTile);
				}
			}
			bIsOutline = false;
		}
	}

	
	if (bIsForward == false)
	{
		UpWallMeshComponent->SetHiddenInGame(false);
		UpWallMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		auto ChildComps = UpWallMeshComponent->GetAttachChildren();
		for (int i = 0; i < ChildComps.Num(); i++)
		{
			ChildComps[i]->SetHiddenInGame(false);
			ChildComps[i]->OnActorEnableCollisionChanged();

			UStaticMeshComponent* TempComp = Cast<UStaticMeshComponent>(ChildComps[i]);
				if (TempComp != nullptr)
				{
					TempComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				}
			
		}
		bIsOutline = true;

	}
	if (bIsBack == false)
	{
		DownWallMeshComponent->SetHiddenInGame(false);
		DownWallMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		auto ChildComps = DownWallMeshComponent->GetAttachChildren();
		for (int i = 0; i < ChildComps.Num(); i++)
		{
			ChildComps[i]->SetHiddenInGame(false);
			ChildComps[i]->OnActorEnableCollisionChanged();
			UStaticMeshComponent* TempComp = Cast<UStaticMeshComponent>(ChildComps[i]);
			if (TempComp != nullptr)
			{
				TempComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			}
		}
		bIsOutline = true;

	}
	if (bIsLeft == false)
	{
		LeftWallMeshComponent->SetHiddenInGame(false);	
		LeftWallMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		auto ChildComps = LeftWallMeshComponent->GetAttachChildren();
		for (int i = 0; i < ChildComps.Num(); i++)
		{
			ChildComps[i]->SetHiddenInGame(false);
			ChildComps[i]->OnActorEnableCollisionChanged();
			UStaticMeshComponent* TempComp = Cast<UStaticMeshComponent>(ChildComps[i]);
			if (TempComp != nullptr)
			{
				TempComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			}
		}
		bIsOutline = true;

	}
	if (bIsRight == false)
	{
		RightWallMeshComponent->SetHiddenInGame(false);
		RightWallMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		auto ChildComps = RightWallMeshComponent->GetAttachChildren();
		for (int i = 0; i < ChildComps.Num(); i++)
		{
			ChildComps[i]->SetHiddenInGame(false);
			ChildComps[i]->OnActorEnableCollisionChanged();
			UStaticMeshComponent* TempComp = Cast<UStaticMeshComponent>(ChildComps[i]);
			if (TempComp != nullptr)
			{
				TempComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			}
		}
		bIsOutline = true;
	}
}

void AED_DungeonTile::CheckIsDoor()
{

	
}

void AED_DungeonTile::CheckNeighborWall()
{

	for (int i = 0; i < NeighborTiles.Num(); i++)
	{
		if (NeighborTiles[i].NeighborTile->bIsPath == true)
		{
			if (this->bIsPath == false)
			{
				NeighborTiles[i].NeighborTile->bIsDoor = true;
			}
		}
	}
}

void AED_DungeonTile::CheckDoorSide()
{
	/*if (bIsBossRoom)
	{
		return;
	}*/

	if (this->bIsDoor == false)
	{
		return;
	}

	for (int i = 0; i < NeighborTiles.Num(); i++)
	{
		if (NeighborTiles[i].NeighborTile->bIsRoom == true)
		{
			DoorSide.Add(NeighborTiles[i].Direction);
		}
	}

}

void AED_DungeonTile::SetBossRoom()
{
	bIsBossRoom = true;
	LeftWallMeshComponent->SetHiddenInGame(true);
	RightWallMeshComponent->SetHiddenInGame(true);
	UpWallMeshComponent->SetHiddenInGame(true);
	DownWallMeshComponent->SetHiddenInGame(true);
	
	TArray<USceneComponent*> Comps;
	TileMeshComponent->GetChildrenComponents(true, Comps);

	for (auto Child : Comps)
	{
		if (Child->GetName() == "CeilComp")
		{
			continue;
		}

		Child->SetHiddenInGame(true);
	}
}

void AED_DungeonTile::GenerateDoor()
{

	for (int i = 0; i < DoorSide.Num(); i++)
	{
		FVector BoxLocation = TileBoxComponent->GetComponentLocation();
		
		FVector TempSpawnLocation;
		AED_Door* Door;
		FRotator NewRotation;
			
		
		switch (DoorSide[i])
		{
		case ETileDirection::Forward:
			TempSpawnLocation = BoxLocation+(FVector::ForwardVector*300.f);
			Door = GetWorld()->SpawnActor<AED_Door>(DoorClass, TempSpawnLocation, FRotator::ZeroRotator);
			NewRotation = FRotationMatrix::MakeFromX(FVector::ForwardVector).Rotator();
			Door->SetActorRotation(NewRotation);
			break;
		case ETileDirection::Back:
			 TempSpawnLocation = BoxLocation + (FVector::BackwardVector * 300.f);
			 Door = GetWorld()->SpawnActor<AED_Door>(DoorClass, TempSpawnLocation, FRotator::ZeroRotator);
			  NewRotation = FRotationMatrix::MakeFromX(FVector::BackwardVector).Rotator();
			 Door->SetActorRotation(NewRotation);
			break;
		case ETileDirection::Left:
			TempSpawnLocation = BoxLocation + (FVector::LeftVector * 300.f);
			 Door = GetWorld()->SpawnActor<AED_Door>(DoorClass, TempSpawnLocation, FRotator::ZeroRotator);
			  NewRotation = FRotationMatrix::MakeFromX(FVector::LeftVector).Rotator();
			 Door->SetActorRotation(NewRotation);
			break;
		case ETileDirection::Right:
			 TempSpawnLocation = BoxLocation + (FVector::RightVector * 300.f);
			 Door = GetWorld()->SpawnActor<AED_Door>(DoorClass, TempSpawnLocation, FRotator::ZeroRotator);
			  NewRotation = FRotationMatrix::MakeFromX(FVector::RightVector).Rotator();
			 Door->SetActorRotation(NewRotation);
			break;
		default:
			break;
		}
	}


}

void AED_DungeonTile::BossRoomEntry()
{
	bIsAlreadySetting = true;
	DownWallMeshComponent->SetHiddenInGame(false);
	bIsBossRoom = false;

	bIsRoom = false;
}

void AED_DungeonTile::GenerateRandomProps()
{

	if (bIsOutline)
	{
		return;	
	}

	int32 RandomPropNum = FMath::RandRange(1, 4);

	for (int i = 0; i < RandomPropNum; i++)
	{

		float RandX =FMath::RandRange(100,500);
		float RandY= FMath::RandRange(100, 500);
		FVector RandomLocation = GetActorLocation() + FVector(RandX, RandY, 0);

		AED_RandomProps* TempProp = GetWorld()->SpawnActor<AED_RandomProps>(AED_RandomProps::StaticClass(), RandomLocation, FRotator::ZeroRotator);

	}

}

void AED_DungeonTile::GenerateOutlineTorch()
{
	if (!bIsOutline)
	{
		return;
	}

	AED_WallTorch* TempProp = GetWorld()->SpawnActor<AED_WallTorch>(TorchClass, GetActorLocation(), FRotator::ZeroRotator);
	FVector WallLocation;
	FRotator NewRotation;




	if (bIsBack == false)
	{
		WallLocation = FVector(0, 300, 350);
		NewRotation = FRotator(0, -90, 0);
	}
	else if (bIsForward == false)
	{
		WallLocation = FVector(600, 300, 350);
		NewRotation = FRotator(0, 90, 0);

	}
	else if (bIsLeft == false)
	{
		WallLocation = FVector(300, 0, 350);
		NewRotation = FRotator(0, 0, 0);

	}
	else if (bIsRight == false)
	{
		WallLocation = FVector(300, 600, 350);
		NewRotation = FRotator(0, 180, 0);

	}
	TempProp->SetActorLocation(GetActorLocation()+WallLocation);
	TempProp->SetActorRotation(NewRotation);

}

void AED_DungeonTile::ChandalierOn()
{

	ChildChandalier->SetHiddenInGame(false);

}

