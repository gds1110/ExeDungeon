// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomStruct.generated.h"

UCLASS()
class EXEDUNGEON_API ACustomStruct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACustomStruct();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

USTRUCT(Atomic, BlueprintType)
struct FDungeonRoom 
{

	GENERATED_BODY()
	
public:
	FDungeonRoom() {};
	FDungeonRoom(TArray<class AED_DungeonTile*>Cells,int32 Number):DungeonCells(Cells),RoomNum(Number){}

public:
	int32 Width;
	int32 Height;
	int32 Depth;
	FVector CenterPos;
	FVector CenterIndex;
	TArray<class AED_DungeonTile*> DungeonCells;
	TArray<FVector> RoomToRoomPaths;
	int32 RoomNum;
	TArray<FVector> DungeonCellsPos; // 예비용
	TArray<int32> ConnectRoomsNum;
	TArray<TPair<FVector/*출발타일 인덱스*/, FVector/*도착타일 인덱스*/>> EdgeVectors;
public:
	FVector GetCenterPos();
	FVector GetCenterIndex();
	FVector GetNearestTileIndex(FVector InVector);
	FVector GetNearestTileIndex(FVector StartVector,FVector TargetVector);
	class AED_DungeonTile* GetCenterTile();
};

USTRUCT(Atomic, BlueprintType)
struct FDungeonCell
{

	GENERATED_BODY()

	public:
		bool IsRoom = false;
		int32 RoomNum;
		int32 Width;
		int32 Height;
		int32 Depth;
		int32 X;
		int32 Y;
		int32 Z;
		FVector TilePos;
		FVector CenterPos;
		FVector Index;
		int32 F;
		int32 G;
		int32 H;
		FVector ParentNodeIndex;
		FDungeonCell* ParentCell;
public:
	class AED_DungeonTile* MyTile;
	
	int32 GetF() { return G + H; }

	void SetTile(class AED_DungeonTile* InTile)
	{
		MyTile = InTile;
	}
	AED_DungeonTile* GetTile()
	{
		return MyTile;
	}

	bool operator==(const FDungeonCell& Other) const
	{
		return (Index == Other.Index);
	}

	FDungeonCell& operator=(const FDungeonCell& Other)
	{
		if (this == &Other)
		{
			return *this;
		}
		F = Other.F;
		G = Other.G;
		H = Other.H;
		IsRoom = Other.IsRoom;
		Index = Other.Index;
		ParentNodeIndex = Other.ParentNodeIndex;
		ParentCell = Other.ParentCell;
		return *this;
	}
};

USTRUCT(Atomic, BlueprintType)
struct F3DDungeon
{

	GENERATED_BODY()

		F3DDungeon()
	{
		Initialize(10, 10, 10);

	}
	public:
		int32 D_SizeX;
		int32 D_SizeY;
		int32 D_SizeZ;
		int32 TileGapX;
		int32 TileGapY;
		int32 TileGapZ;
		TArray<TArray<TArray<class AED_DungeonTile*>>> DungeonData;
		TArray<class AED_DungeonTile*> VaildDungeonCell;
		TArray<class AED_DungeonTile*> VaildPaths;

		void Initialize(int32 SizeX, int32 SizeY, int32 Depth)
		{
			D_SizeX = SizeX;
			D_SizeY = SizeY;
			D_SizeZ = Depth;

			DungeonData.Empty();
			DungeonData.SetNum(SizeX);

			for (int32 i = 0; i < SizeX; ++i)
			{
				DungeonData[i].SetNum(SizeY);

				for (int32 j = 0; j < SizeY; ++j)
				{
					DungeonData[i][j].SetNum(Depth);
				}
			}
		}

		void InitializeTile(int32 InTileGapX, int32 InTileGapY, int32 InTileGapZ);

		
		void SetDungeonTile(FVector InVector,class AED_DungeonTile* InTile);

		class AED_DungeonTile* GetDungeonCell(FVector InVector)
		{
			if (DungeonData[InVector.X][InVector.Y][InVector.Z] != nullptr)
			{
				return DungeonData[InVector.X][InVector.Y][InVector.Z];
			}

			return nullptr;
		}
};

template <typename InElementType>
struct TPriorityQueueNode {
	InElementType Element;
	float Priority;

	TPriorityQueueNode()
	{
	}

	TPriorityQueueNode(InElementType InElement, float InPriority)
	{
		Element = InElement;
		Priority = InPriority;
	}

	bool operator<(const TPriorityQueueNode<InElementType> Other) const
	{
		return Priority < Other.Priority;
	}
};

template <typename InElementType>
class TPriorityQueue {
public:
	TPriorityQueue()
	{
		Array.Heapify();
	}

public:
	// Always check if IsEmpty() before Pop-ing!
	InElementType Pop()
	{
		TPriorityQueueNode<InElementType> Node;
		Array.HeapPop(Node);
		return Node.Element;
	}

	TPriorityQueueNode<InElementType> PopNode()
	{
		TPriorityQueueNode<InElementType> Node;
		Array.HeapPop(Node);
		return Node;
	}

	void Push(InElementType Element, float Priority)
	{
		Array.HeapPush(TPriorityQueueNode<InElementType>(Element, Priority));
	}

	bool IsEmpty() const
	{
		return Array.Num() == 0;
	}

private:
	TArray<TPriorityQueueNode<InElementType>> Array;
};

//// -- Usage
//// Lower number means higher priority
//
//// -- Construction
//TPriorityQueue<UTile*> Frontier;
//
//// -- Adding elements: (element, priority)
//Frontier.Push(TargetTile, 0.0f);
//Frontier.Push(Grid->At(10, 16), 8.0f);
//Frontier.Push(StartTile, 1000.0f);
//
//// -- Iteration
//while (!Frontier.IsEmpty()) {
//	UTile* Current = Frontier.Pop(); // Removes it from the top of the heap
//	Current->DoMagic();
//}
//
