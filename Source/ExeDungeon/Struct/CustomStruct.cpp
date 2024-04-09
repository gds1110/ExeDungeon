// Fill out your copyright notice in the Description page of Project Settings.


#include "Struct/CustomStruct.h"
#include "DungeonTile/ED_DungeonTile.h"
#include "Engine/World.h"

// Sets default values
ACustomStruct::ACustomStruct()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACustomStruct::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomStruct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void F3DDungeon::InitializeTile(int32 InTileGapX, int32 InTileGapY, int32 InTileGapZ)
{
	for (int32 x = 0; x < D_SizeX; x++)
	{
		for (int32 y = 0; y < D_SizeY; y++)
		{
			for (int32 z = 0; z < D_SizeZ; z++)
			{
				FVector TempTilePos = FVector(x * InTileGapX, y * InTileGapY, z * InTileGapZ);

				FVector TempTileCenter = FVector(TempTilePos.X + (InTileGapX / 2), TempTilePos.Y + (InTileGapY / 2), TempTilePos.Z + (InTileGapZ / 2));
				if (z == 0)
				{
					TempTileCenter.Z = TempTilePos.Z + ((-1 * InTileGapZ) / 2);
				}
				//DungeonData[x][y][z]->SetActorLocation(TempTilePos);
			}
		}
	}
}

void F3DDungeon::SetDungeonTile(FVector InVector, AED_DungeonTile* InTile)
{
	if (InTile == nullptr)
	{
		return;
	}

	DungeonData[InVector.X][InVector.Y][InVector.Z] = InTile;
	InTile->TileIndex = InVector;
	DungeonData[InVector.X][InVector.Y][InVector.Z]->bIsRoom = true;
	
}

FVector FDungeonRoom::GetCenterPos()
{
	if (DungeonCells.Num() > 0)
	{
		int32 CellNum = DungeonCells.Num();
		AED_DungeonTile* StartTile = DungeonCells[0];
		AED_DungeonTile* LastTile = DungeonCells.Last();
		
		FVector LastTileLocation = LastTile->GetActorLocation();
		FVector StartTileLocation = StartTile->GetActorLocation();

		LastTileLocation = LastTileLocation + FVector(600.0f, 600.0f, 0);

		FVector CenterVector = StartTileLocation + (LastTileLocation - StartTileLocation)/2;
	
		CenterPos = CenterVector;
		//UE_LOG(LogTemp, Log, TEXT("center x %f y %f z%f"), CenterPos.X, CenterPos.Y, CenterPos.Z);
	
	}


	return CenterPos;
}

FVector FDungeonRoom::GetCenterIndex()
{
	if (DungeonCells.Num() > 0)
	{
		AED_DungeonTile* StartTile = DungeonCells[0];
		AED_DungeonTile* LastTile = DungeonCells.Last();


		FVector CenterVector = StartTile->GetActorLocation() - LastTile->GetActorLocation();
		CenterIndex = CenterVector;

	}


	return CenterIndex;
}

//해당 좌표에대한 나의 좌표중 가장 가까운 좌표
FVector FDungeonRoom::GetNearestTileIndex(FVector InVector)
{
	float MinDist = 99999999999;
	FVector RetIndex;
	for (int32 i = 0; i < DungeonCells.Num(); i++)
	{
		FVector TempTilelocation = DungeonCells[i]->GetActorLocation();
		if (TempTilelocation == InVector)
		{
			continue;
		}
		float NewDist = abs(FVector::Distance(TempTilelocation, InVector));
		if (NewDist < MinDist)
		{
			MinDist = NewDist;
			RetIndex = DungeonCells[i]->TileIndex;
		}
	}


	return RetIndex;
}

//StartVector와 TargetVector의 가장가까운 거리의 인덱스를 DungeonRoom의 Cells에서 찾아서 인덱스 반환
FVector FDungeonRoom::GetNearestTileIndex(FVector StartVector, FVector TargetVector)
{
	float MinDist = 99999999999;
	FVector RetIndex;

	for (int32 i = 0; i < DungeonCells.Num(); i++)
	{
		float NewDist = abs(FVector::Distance(StartVector, TargetVector));
		if (NewDist < MinDist)
		{
			MinDist = NewDist;
			RetIndex = DungeonCells[i]->TileIndex;
		}
	}


	return RetIndex;
}

AED_DungeonTile* FDungeonRoom::GetCenterTile()
{
	return DungeonCells[DungeonCells.Num() / 2];
}
