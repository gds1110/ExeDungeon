// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Struct/CustomStruct.h"
#include "GameData/ED_MonsterClassData.h"
#include "ED_DungeonGenerator.generated.h"

UCLASS()
class EXEDUNGEON_API AED_DungeonGenerator : public AActor
{

	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AED_DungeonGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	// 던전 방의 총 갯수
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 MinRoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxRoom;

	// 타일 갯수 최소 ~ 최대 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RoomMinTileNumX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RoomMaxTileNumX;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RoomMinTileNumY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RoomMaxTileNumY;

	//타일 갯수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DungeonTileNumX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DungeonTileNumY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DungeonTileDepth;

	//타일 간의 간격
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DungeonTileGapX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DungeonTileGapY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DungeonTileGapZ;

	//던전 크기 최소값과 최대값 좌표
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinPointX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinPointY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinPointZ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxPointX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxPointY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxPointZ;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UStaticMesh> DungeonTile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AED_DungeonTile> BaseTileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, FBox> DuplicatBoxs; //겹치는 박스
	TArray<FBox> DuplicateBoxs;
	TArray<FBox> AllBoxs;
	F3DDungeon DungeonMap;
	TArray<FDungeonRoom> DungeonRooms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AED_DungeonTile* StartPoint;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AED_Props> DoorClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABossRoom> BossRoomBP;

public:
	UPROPERTY(EditAnywhere)
	UED_MonsterClassData* MonsterDatas;
public:
	UFUNCTION()
	void GenerateDungeon();

	UFUNCTION()
	void MakeRoom();

	void GenerateDoor();


	bool CheckStickOut();
	bool CheckZInterSect(const FBox& TargetBox, const FBox& OtherBox);

	TArray<FVector> GetBoxVertices(const FBox& InBox);
	bool CheckTileInsideFBox(const FBox& InBox, const FVector& InTilePos);

	void SettingDungeonFBox();
	void MergeBoxs();
	TArray<FBox> GetVaildRooms();
	FBox GetVaildRoom();
	FBox GetVaildRoomUnduplicated();
	FBox GetRoomByValue(int XNum, int YNum);
	TArray<FVector> GetIndexFromCube(const FBox& InBox);
	TArray<FVector> GetCenterPosTileFromCube(const FBox& InBox);

	FVector GetLocationFromIndex(FVector Index);
	FVector GetIndexFromLocation(FVector Location);
	bool CheckVaildGrid(FVector Index);
	bool CheckVaildIndex(FVector Index);
	void SMTDungeon();
	void CheckConnectionRooms();
	void MakePathRoomToRoom();
	void MakePathByIndexByIndex(TArray<FVector> InPaths);
	void FindNearestDoors();
	TArray<FVector> GetPaths(FVector StartIndex, FVector TargetIndex);
	TArray<class AED_DungeonTile*> FindBFS(class AED_DungeonTile* StartTile);
	TArray<class AED_DungeonTile*> DungeonRoomFindBFS(class AED_DungeonTile* StartTile);

	//BossRoom

	void GenerateBossRoom();
	void FindStartPoint();

	float CalculateDistance(const FVector& Start, const FVector& Target);

	//Props 
	public:

		int32 ChandlaierNum;

		void OnChandlaier();
		void GenerateProps();
		void SpawnEnemys();
};
