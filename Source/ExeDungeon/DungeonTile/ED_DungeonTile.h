// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ED_DungeonTile.generated.h"


UENUM(BlueprintType)
enum class ETileDirection : uint8
{
	Forward,
	Back,
	Left,
	Right
};

struct NeighborTile
{
	AED_DungeonTile* NeighborTile;
	ETileDirection Direction;
};
UCLASS()
class EXEDUNGEON_API AED_DungeonTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AED_DungeonTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnyWhere,Category=Tile)
	bool bIsRoom;
	UPROPERTY(EditAnyWhere, Category = Tile)
	bool bIsPath;
	UPROPERTY(EditAnyWhere, Category = Tile)
	bool bIsVisited; //BFSÀü¿ë
	UPROPERTY(EditAnyWhere, Category = Tile)
	bool bIsOutline;
	UPROPERTY(EditAnyWhere, Category = Tile)
	bool bIsDoor;
	UPROPERTY(EditAnyWhere, Category = Tile)
	bool bIsBossRoom;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FVector TileCenterPos;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FVector TileIndex;
	UPROPERTY()
	TArray<ETileDirection> DoorSide;

	TArray<NeighborTile> NeighborTiles;

	
public:
	UPROPERTY()
	bool bIsForward;
	UPROPERTY()
	bool bIsBack;
	UPROPERTY()
	bool bIsLeft;
	UPROPERTY()
	bool bIsRight;
	
	UPROPERTY()
	bool bIsAlreadySetting;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 RoomNumber;
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TObjectPtr<class UBoxComponent> TileBoxComponent;
	UPROPERTY(EditAnyWhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> TileMeshComponent;
	UPROPERTY(EditAnyWhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> LeftWallMeshComponent;
	UPROPERTY(EditAnyWhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> RightWallMeshComponent;
	UPROPERTY(EditAnyWhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> UpWallMeshComponent;
	UPROPERTY(EditAnyWhere,BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> DownWallMeshComponent;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> TileMesh;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TObjectPtr<UStaticMesh> WallMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AED_Door> DoorClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AED_RandomProps> RandomPropClass;
	//Props

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UChildActorComponent> ChildChandalier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UChildActorComponent> ChildTorch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class AED_Door> OnDoor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AED_WallTorch> TorchClass;

public:
	void CheckDirectionWall();
	void CheckIsDoor();
	void CheckNeighborWall();
	void CheckDoorSide();
	void SetBossRoom();
	void GenerateDoor();

	void BossRoomEntry();

	void GenerateRandomProps();
	void GenerateOutlineTorch();
public:
	void ChandalierOn();
};
