// Fill out your copyright notice in the Description page of Project Settings.


#include "DungeonGenerator/ED_DungeonGenerator.h"
#include "DungeonTile/ED_DungeonTile.h"
#include "ED_DungeonGenerator.h"
#include "Kismet/KismetMathLibrary.h"
#include "DungeonTile/BossRoom.h"
#include "Character/ED_EnemyCharacter.h"
#include "ExeDungeonGameMode.h"
#include <Kismet/GameplayStatics.h>



// Sets default values
AED_DungeonGenerator::AED_DungeonGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ConstructorHelpers::FObjectFinder<UStaticMesh> RefTileMesh(TEXT(""));
	if (RefTileMesh.Succeeded())
	{
		DungeonTile = RefTileMesh.Object;
	}

	ConstructorHelpers::FClassFinder<AED_DungeonTile> BaseTileClassRef(TEXT("/Game/Dungeon/Blueprint/Tile/BP_BaseTile.BP_BaseTile_C"));
	if (BaseTileClassRef.Succeeded())
	{
		BaseTileClass = BaseTileClassRef.Class;
	}

	MinRoom = 2;
	MaxRoom = 5;

	DungeonTileGapX = 600.0f;
	DungeonTileGapY = 600.0f;
	DungeonTileGapZ = 600.0f;

	DungeonMap = F3DDungeon();

	DungeonTileNumX = 50;
	DungeonTileNumY = 50;
	DungeonTileDepth = 3;

	RoomMinTileNumX = 2;
	RoomMinTileNumY = 2;
	RoomMaxTileNumX = 8;
	RoomMaxTileNumY = 8;

	ChandlaierNum = 5;

}

// Called when the game starts or when spawned
void AED_DungeonGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	DungeonMap.Initialize((DungeonTileNumX), (DungeonTileNumY), (DungeonTileDepth));
	//DungeonMap.InitializeTile(DungeonTileGapX, DungeonTileGapY, DungeonTileGapZ);
	MakeRoom(); //가상의 룸 생성 및 실제 타일생성
	CheckConnectionRooms(); //겹쳐져있는 룸 생성
	SMTDungeon(); // 룸들의 간선찾기
	GenerateBossRoom();
	FindNearestDoors(); //간선간 서로 가까운 거리 및 경로 찾기
	MakePathRoomToRoom(); // 타일생성
	GenerateDoor();
	OnChandlaier();
	GenerateProps();
	FindStartPoint();
}

// Called every frame
void AED_DungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AED_DungeonGenerator::GenerateDungeon()
{


}

void AED_DungeonGenerator::MakeRoom()
{
	TArray<FBox> VaildBoxs = GetVaildRooms();
	TArray<FVector> TempVectors;

	int32 RoomNum = 0;
	//겹치지않는 방의 좌표들
	for (int i = 0; i < VaildBoxs.Num(); i++)
	{
		TempVectors = GetIndexFromCube(VaildBoxs[i]);
		TArray<AED_DungeonTile*> TempRoomTiles;
		TempRoomTiles.Empty();
		for (int j = 0; j < TempVectors.Num(); j++)
		{
			FVector SpawnLocation = GetLocationFromIndex(TempVectors[j]);
			AED_DungeonTile* NewTile = GetWorld()->SpawnActor<AED_DungeonTile>(BaseTileClass, SpawnLocation, FRotator::ZeroRotator);
		
			DungeonMap.SetDungeonTile(TempVectors[j],NewTile);
			DungeonMap.VaildDungeonCell.Add(DungeonMap.GetDungeonCell(TempVectors[j]));
			TempRoomTiles.Add(NewTile);
			NewTile->RoomNumber = RoomNum;
			NewTile->bIsRoom = true;
			//DrawDebugSphere
			//(
			//	GetWorld(),
			//	SpawnLocation,
			//	50.0f,
			//	5.0f,
			//	FColor::Yellow,
			//	false,
			//	150.0f
			//);
		}
		FDungeonRoom TempDungeonRoom = FDungeonRoom(TempRoomTiles,RoomNum);
		DungeonRooms.Add(TempDungeonRoom);
		RoomNum++;
	}
	TArray<FVector> DuplicateVector; //겹치는 방의 좌표들
	
	TArray<FBox> NewBoxs;
	TArray<TArray<int32>> CheckDupleBox;
	CheckDupleBox.SetNum(DuplicateBoxs.Num());

	for (int32 i = 0; i < DuplicateBoxs.Num(); i++)
	{
		CheckDupleBox[i].Init(-1, DuplicateBoxs.Num());
		//CheckDupleBox[i].SetNum(DuplicateBoxs.Num());
	}

	for (int32 i = 0; i < DuplicateBoxs.Num(); i++)
	{
		for (int32 j = 0; j < DuplicateBoxs.Num(); j++)
		{
			if (i == j)
			{
				continue;
			}
			if (CheckDupleBox[i].Contains(j) || CheckDupleBox[j].Contains(i))
			{
				continue;
			}
			if (DuplicateBoxs[i].IntersectXY(DuplicateBoxs[j]))
			{
				CheckDupleBox[i].Add(j);
			}
		}
	}
	for (int32 i = 0; i < CheckDupleBox.Num(); i++)
	{
		for (int32 j = 0; j < CheckDupleBox[i].Num(); j++)
		{
			if (CheckDupleBox[i][j] != -1)
			{
				UE_LOG(LogTemp, Log, TEXT("%d and %d Intercet"), i, CheckDupleBox[i][j]);
			}
		}
	}

	for (int i = 0; i < DuplicateBoxs.Num(); i++)
	{
		DuplicateVector = GetIndexFromCube(DuplicateBoxs[i]);
		TArray<AED_DungeonTile*> TempRoomTiles;
		TempRoomTiles.Empty();
		for (int j = 0; j < DuplicateVector.Num(); j++)
		{
			if (DungeonMap.GetDungeonCell(DuplicateVector[j]) == nullptr)
			{
				FVector SpawnLocation = GetLocationFromIndex(DuplicateVector[j]);
				AED_DungeonTile* NewTile = GetWorld()->SpawnActor<AED_DungeonTile>(BaseTileClass, SpawnLocation, FRotator::ZeroRotator);
				NewTile->bIsRoom = true;
				DungeonMap.SetDungeonTile(DuplicateVector[j], NewTile);
				DungeonMap.VaildDungeonCell.Add(DungeonMap.GetDungeonCell(DuplicateVector[j]));
				TempRoomTiles.Add(NewTile);

		/*		DrawDebugSphere
				(
					GetWorld(),
					SpawnLocation,
					50.0f,
					5.0f,
					FColor::Orange,
					false,
					150.0f
				);*/
				NewTile->RoomNumber = RoomNum;
			}
		}
		FDungeonRoom TempDungeonRoom = FDungeonRoom(TempRoomTiles,RoomNum);
		DungeonRooms.Add(TempDungeonRoom);
		RoomNum++;
	}
	//int32 VaildTileNum = DungeonMap.VaildDungeonCell.Num();
	//for (int32 i = 0; i < VaildTileNum; i++)
	//{
	//	AED_DungeonTile* TempTile = DungeonMap.VaildDungeonCell[i];
	//	if (TempTile != nullptr)
	//	{
	//		TempTile->CheckDirectionWall();

	//	}
	//}

}

void AED_DungeonGenerator::GenerateDoor()
{
	int32 VaildTileNum = DungeonMap.VaildDungeonCell.Num();

	for (int32 i = 0; i < VaildTileNum; i++)
	{
		AED_DungeonTile* TempTile = DungeonMap.VaildDungeonCell[i];
	
		if (TempTile != nullptr)
		{
			TempTile->CheckDoorSide();
		}
	}


	for (int32 i = 0; i < VaildTileNum; i++)
	{
		AED_DungeonTile* TempTile = DungeonMap.VaildDungeonCell[i];

		if (TempTile != nullptr)
		{
			TempTile->GenerateDoor();
		}
	}
}


bool AED_DungeonGenerator::CheckZInterSect(const FBox& TargetBox, const FBox& OtherBox)
{
	FVector TargetMin = TargetBox.Min;
	FVector TargetMax = TargetBox.Max;
	FVector OtherMin = OtherBox.Min;
	FVector OtherMax = OtherBox.Max;
	if (TargetMin == OtherMin && TargetMax == OtherMax)
	{
		return false;
	}


	return true;
}

TArray<FVector> AED_DungeonGenerator::GetBoxVertices(const FBox& InBox)
{
	TArray<FVector> BoxVertices;

	FVector Min = InBox.Min;
	FVector Max = InBox.Max;

	BoxVertices.Add(FVector(Min));
	BoxVertices.Add(FVector(Min.X, Min.Y, Max.Z));
	BoxVertices.Add(FVector(Min.X, Max.Y, Min.Z));
	BoxVertices.Add(FVector(Max.X, Min.Y, Min.Z));
	BoxVertices.Add(FVector(Max.X, Max.Y, Min.Z));
	BoxVertices.Add(FVector(Max.X, Min.Y, Max.Z));
	BoxVertices.Add(FVector(Min.X, Max.Y, Max.Z));
	BoxVertices.Add(FVector(Max));


	return BoxVertices;
}

TArray<FBox> AED_DungeonGenerator::GetVaildRooms()
{

	TArray<FBox> RetBox;
	TArray<FBox> Boxs;
	DuplicateBoxs.Empty();
	FVector Center = GetActorLocation() + FVector((DungeonTileGapX * (DungeonTileNumX - 1)) / 2, (DungeonTileGapY * (DungeonTileNumY - 1)) / 2, (DungeonTileGapZ * (DungeonTileDepth - 1)) / 2);
	UE_LOG(LogTemp, Log, TEXT("Dungeon Cube Center Point :X  %f Y : %f  Z : %f"), Center.X, Center.Y, Center.Z);

	FColor DrawColor = FColor::Green;  //bResult ? FColor::Green : FColor::Red;	
	FColor DrawRedColor = FColor::Red;  //bResult ? FColor::Green : FColor::Red;	
	float DebugLifeTime = 150.0f;
	FVector ExtentBox = FVector((DungeonTileGapX * DungeonTileNumX) / 2, (DungeonTileGapY * DungeonTileNumY) / 2, (DungeonTileGapZ * DungeonTileDepth) / 2);
	FBox DungeonBox = FBox(FVector(0, 0, 0), FVector((DungeonTileGapX * (DungeonTileNumX - 1)), (DungeonTileGapY * (DungeonTileNumY - 1)), (DungeonTileGapZ * (DungeonTileDepth - 1))));
	UE_LOG(LogTemp, Log, TEXT("Dungeon Cube Point :X  %f Y : %f  Z : %f"), DungeonBox.Max.X/DungeonTileGapX, DungeonBox.Max.Y/DungeonTileGapY, DungeonBox.Max.Z/DungeonTileGapZ);

	//DrawDebugBox
	//(
	//	GetWorld(),//그릴월드	
	//	Center,//위치	
	//	ExtentBox,
	//	DrawColor, //색깔	
	//	false,//지속여부		
	//	DebugLifeTime //지속시간
	//);


#pragma region TestBoxIndexRegion

	FBox TestBox = FBox(FVector(1200, 1200, -300), FVector(2400, 3600, 300));
	TArray<FVector> TestIndexs = GetIndexFromCube(TestBox);



	//DrawDebugBox
	//(
	//	GetWorld(),//그릴월드	
	//	TestBox.GetCenter(),//위치	
	//	TestBox.GetExtent(),
	//	FColor::Magenta, //색깔	
	//	false,//지속여부		
	//	DebugLifeTime //지속시간
	//);


#pragma endregion

	


	for (int32 i = 0; i < MaxRoom; i++)
	{
		int32 RandomXNum = FMath::RandRange(RoomMinTileNumX, RoomMaxTileNumX);
		int32 RandomYNum = FMath::RandRange(RoomMinTileNumY, RoomMaxTileNumY);

		FVector RandomCenter = UKismetMathLibrary::RandomPointInBoundingBox(Center, ExtentBox);
		int32 ClampCenterX = ((int32)RandomCenter.X / (int32)DungeonTileGapX) * DungeonTileGapX;
		int32 ClampCenterY = ((int32)RandomCenter.Y / (int32)DungeonTileGapY) * DungeonTileGapY;
		//int32 ClampCenterZ = ((int32)RandomCenter.Z / (int32)DungeonTileGapZ) * DungeonTileGapZ;
		int32 ClampCenterZ = ((int32)RandomCenter.Z / (int32)DungeonTileGapZ) * DungeonTileGapZ;
		FVector RoomCenter = FVector(ClampCenterX, ClampCenterY, ClampCenterZ);

		//FVector RoomCenter = UKismetMathLibrary::RandomPointInBoundingBox(Center, ExtentBox);
		FVector ExtentDungeonBox = FVector((DungeonTileGapX * RandomXNum) / 2, (DungeonTileGapY * RandomYNum) / 2, DungeonTileGapZ / 2);

		FVector Min = RoomCenter - ExtentDungeonBox;
		FVector Max = RoomCenter + ExtentDungeonBox;

		FBox RoomBox(Min, Max);

		DuplicatBoxs.Empty();

		// 던전 외곽에 걸쳐 생성되었는지 확인
		if ((RoomBox.Min.X < DungeonBox.Min.X) || (DungeonBox.Max.X < RoomBox.Max.X))
		{
			continue;
		}

		if ((RoomBox.Min.Y < DungeonBox.Min.Y) || (DungeonBox.Max.Y < RoomBox.Max.Y))
		{
			continue;
		}


		Boxs.Add(RoomBox);
	}
	for (int32 i = 0; i < Boxs.Num(); i++)
	{
		FColor DColor = FColor::Green;
		bool IsVaildBox = true;
		for (int32 j = 0; j < Boxs.Num(); j++)
		{
			if (i == j)
			{
				continue;
			}
			if (Boxs[i].Intersect(Boxs[j]))
			{
				if (Boxs[i].Min.Z == Boxs[j].Max.Z || Boxs[j].Min.Z == Boxs[i].Max.Z)
				{

				}
				else
				{
					DColor = FColor::Red;
					//DuplicatBoxs.Add(i, Boxs[j]);
					if (DuplicateBoxs.Contains(Boxs[j]) == false)
					{
						DuplicateBoxs.Add(Boxs[j]);
					}

					IsVaildBox = false;
				}
			}
		}

		FVector TempRoomMin = Boxs[i].Min;
		FVector TempRoomMax = Boxs[i].Max;

		/*for (const auto& Pair : DuplicatBoxs)
		{
			int32 Key = Pair.Key;
			const FBox& Value = Pair.Value;

		}*/



		if (IsVaildBox == true)
		{
			RetBox.Add(Boxs[i]);
			DColor = FColor::Cyan;
		}

		// De




		//DrawDebugBox //DrawDebugHelpers에서 제공하는 캡슐그리기 함수.
		//(
		//	GetWorld(),//그릴월드	
		//	Boxs[i].GetCenter(),//위치	
		//	Boxs[i].GetExtent(),
		//	DColor, //색깔	
		//	false,//지속여부		
		//	DebugLifeTime //지속시간
		//);
	}
	AllBoxs = Boxs;
	return RetBox;
}

FBox AED_DungeonGenerator::GetVaildRoom()
{
	return FBox();
}

FBox AED_DungeonGenerator::GetVaildRoomUnduplicated()
{
	FVector Center = GetActorLocation() + FVector((DungeonTileGapX * (DungeonTileNumX - 1)) / 2, (DungeonTileGapY * (DungeonTileNumY - 1)) / 2, (DungeonTileGapZ * (DungeonTileDepth - 1)) / 2);
	FVector ExtentBox = FVector((DungeonTileGapX * DungeonTileNumX) / 2, (DungeonTileGapY * DungeonTileNumY) / 2, (DungeonTileGapZ * DungeonTileDepth) / 2);
	FBox DungeonBox = FBox(FVector(0, 0, 0), FVector((DungeonTileGapX * (DungeonTileNumX - 1)), (DungeonTileGapY * (DungeonTileNumY - 1)), (DungeonTileGapZ * (DungeonTileDepth - 1))));
	
	FVector RandomCenter = UKismetMathLibrary::RandomPointInBoundingBox(Center, ExtentBox);
	int32 ClampCenterX = ((int32)RandomCenter.X / (int32)DungeonTileGapX) * DungeonTileGapX;
	int32 ClampCenterY = ((int32)RandomCenter.Y / (int32)DungeonTileGapY) * DungeonTileGapY;
	//int32 ClampCenterZ = ((int32)RandomCenter.Z / (int32)DungeonTileGapZ) * DungeonTileGapZ;
	int32 ClampCenterZ = ((int32)RandomCenter.Z / (int32)DungeonTileGapZ) * DungeonTileGapZ;
	FVector RoomCenter = FVector(ClampCenterX, ClampCenterY, ClampCenterZ);

	int32 RandomXNum = FMath::RandRange(RoomMinTileNumX, RoomMaxTileNumX);
	int32 RandomYNum = FMath::RandRange(RoomMinTileNumY, RoomMaxTileNumY);
	//FVector RoomCenter = UKismetMathLibrary::RandomPointInBoundingBox(Center, ExtentBox);
	FVector ExtentDungeonBox = FVector((DungeonTileGapX * RandomXNum) / 2, (DungeonTileGapY * RandomYNum) / 2, DungeonTileGapZ / 2);

	FVector Min = RoomCenter - ExtentDungeonBox;
	FVector Max = RoomCenter + ExtentDungeonBox;

	FBox RoomBox(Min, Max);

	if ((RoomBox.Min.X < DungeonBox.Min.X) || (DungeonBox.Max.X < RoomBox.Max.X))
	{

	}

	if ((RoomBox.Min.Y < DungeonBox.Min.Y) || (DungeonBox.Max.Y < RoomBox.Max.Y))
	{

	}

	return FBox();
}

FBox AED_DungeonGenerator::GetRoomByValue(int XNum, int YNum)
{
	FVector Center = GetActorLocation() + FVector((DungeonTileGapX * (DungeonTileNumX - 1)) / 2, (DungeonTileGapY * (DungeonTileNumY - 1)) / 2, (DungeonTileGapZ * (DungeonTileDepth - 1)) / 2);
	FVector ExtentBox = FVector((DungeonTileGapX * DungeonTileNumX) / 2, (DungeonTileGapY * DungeonTileNumY) / 2, (DungeonTileGapZ * DungeonTileDepth) / 2);
	FBox DungeonBox = FBox(FVector(0, 0, 0), FVector((DungeonTileGapX * (DungeonTileNumX - 1)), (DungeonTileGapY * (DungeonTileNumY - 1)), (DungeonTileGapZ * (DungeonTileDepth - 1))));
	FVector Min;
	FVector Max;

	FBox RetBox;
	bool IsInBox = true;
	while (IsInBox)
	{
		FVector RandomCenter = UKismetMathLibrary::RandomPointInBoundingBox(Center, ExtentBox);
		int32 ClampCenterX = ((int32)RandomCenter.X / (int32)DungeonTileGapX) * DungeonTileGapX;
		int32 ClampCenterY = ((int32)RandomCenter.Y / (int32)DungeonTileGapY) * DungeonTileGapY;
		//int32 ClampCenterZ = ((int32)RandomCenter.Z / (int32)DungeonTileGapZ) * DungeonTileGapZ;
		int32 ClampCenterZ = ((int32)RandomCenter.Z / (int32)DungeonTileGapZ) * DungeonTileGapZ;

		FVector RoomCenter = FVector(ClampCenterX, ClampCenterY, ClampCenterZ);

		//FVector RoomCenter = UKismetMathLibrary::RandomPointInBoundingBox(Center, ExtentBox);
		FVector ExtentDungeonBox = FVector((DungeonTileGapX * XNum) / 2, (DungeonTileGapY * YNum) / 2, DungeonTileGapZ / 2);

		Min = RoomCenter - ExtentDungeonBox;
		Max = RoomCenter + ExtentDungeonBox;
		


		if ((Min.X < DungeonBox.Min.X) || (DungeonBox.Max.X < Max.X))
		{
			continue;
		}

		if ((Min.Y < DungeonBox.Min.Y) || (DungeonBox.Max.Y < Max.Y))
		{
			continue;
		}
		FBox RoomBox(Min, Max);
		bool InturpContinue = false;
		for (int i = 0; i < AllBoxs.Num(); i++)
		{
			if (AllBoxs[i].Intersect(RoomBox))
			{
				InturpContinue = true;
				break;
			}
		}
		if (InturpContinue == true)
		{
			continue;
		}
		else
		{
			IsInBox = false;
			RetBox = RoomBox;
			break;
		}
	}
	

	return RetBox;
}

TArray<FVector> AED_DungeonGenerator::GetIndexFromCube(const FBox& InBox)
{
	TArray<FVector> RetVectors;

	FVector Min = InBox.Min;
	FVector Max = InBox.Max;

	FVector BoxSize = Max - Min;

	int32 StartX = Min.X/DungeonTileGapX;
	int32 StartY = Min.Y/DungeonTileGapY;
	int32 StartZ = Min.Z/DungeonTileGapZ;

	int32 TileIndexX = BoxSize.X/DungeonTileGapX;
	int32 TileIndexY = BoxSize.Y/DungeonTileGapY;
	int32 TileIndexZ = BoxSize.Z/DungeonTileGapZ;
	
	for (int32 i = 0; i < TileIndexX; i++)
	{
		for (int32 j = 0; j < TileIndexY; j++)
		{
			for (int32 k = 0; k < TileIndexZ; k++)
			{
				RetVectors.Add(FVector(StartX+i, StartY+j, StartZ));
			}
		}
	}

	return RetVectors;
}

TArray<FVector> AED_DungeonGenerator::GetCenterPosTileFromCube(const FBox& InBox)
{
	TArray<FVector> RetVectors;

	FVector Min = InBox.Min;
	FVector Max = InBox.Max;

	FVector BoxSize = Max - Min;

	int32 StartX = Min.X / DungeonTileGapX;
	int32 StartY = Min.Y / DungeonTileGapY;
	int32 StartZ = Min.Z / DungeonTileGapZ;

	int32 TileIndexX = BoxSize.X / DungeonTileGapX;
	int32 TileIndexY = BoxSize.Y / DungeonTileGapY;
	int32 TileIndexZ = BoxSize.Z / DungeonTileGapZ;

	for (int32 i = 0; i <= TileIndexX; i++)
	{
		for (int32 j = 0; j <= TileIndexY; j++)
		{
			for (int32 k = 0; k <= TileIndexZ; k++)
			{
				RetVectors.Add(FVector(StartX + i, StartY + j, StartZ + k));
			}
		}
	}

	return RetVectors;
}

FVector AED_DungeonGenerator::GetLocationFromIndex(FVector Index)
{
	FVector RetVector;
	
	RetVector = FVector(Index.X * DungeonTileGapX, Index.Y* DungeonTileGapY, Index.Z* DungeonTileGapZ);

	return RetVector;
}

FVector AED_DungeonGenerator::GetIndexFromLocation(FVector Location)
{
	int32 IndexX = Location.X / DungeonTileGapX;
	int32 IndexY = Location.Y / DungeonTileGapY;
	int32 IndexZ = Location.Z / DungeonTileGapZ;

	FVector RetIndex = FVector(IndexX, IndexY, IndexZ);

	return RetIndex;
}

void AED_DungeonGenerator::SMTDungeon()
{
	// 2차원 배열 던전일때만 
	int32 RoomsNum = DungeonRooms.Num();
	TArray<TArray<float>> RoomsGraph;

	RoomsGraph.SetNum(RoomsNum);
	for (int i = 0; i < RoomsGraph.Num(); i++)
	{
		RoomsGraph[i].SetNum(RoomsNum);
	}

	for (int i = 0; i < RoomsNum; i++)
	{
		for (int j = 0; j < RoomsNum; j++)
		{
			if (i == j)
			{
				RoomsGraph[i][j] = 0;
				continue;
			}
			float DistDTD = FVector::DistXY(DungeonRooms[i].GetCenterPos(), DungeonRooms[j].GetCenterPos());
			RoomsGraph[i][j] = DistDTD;
		}
	}
	TArray<int32> MST;
	TArray<int32> RestRooms;
	TArray<bool> Selected;
	Selected.Init(false, RoomsNum);
	MST.Add(0);
	float minDist = 99999999;
	int32 CurNum = MST[0];
	TArray<TPair<int32,int32>> edges;
	while (MST.Num() < RoomsNum)
	{
		int32 MinRoomNum = -1;
		minDist = 99999999;
		for (int i = 0; i < MST.Num(); i++)
		{
			for (int j = 0; j < RoomsGraph[MST[i]].Num(); j++)
			{
				if (RoomsGraph[MST[i]][j] == 0)
				{
					continue;
				}
				if (RoomsGraph[MST[i]][j] < minDist)
				{
					if (MST.Contains(j))
					{
						continue;
					}
			

					minDist = RoomsGraph[MST[i]][j];
					MinRoomNum = j;
					CurNum = MST[i];
				}
			}

		}
		if (MST.Contains(MinRoomNum) == false)
		{
			edges.Add(MakeTuple(CurNum, MinRoomNum));
			MST.Add(MinRoomNum);
		}
	}
	UE_LOG(LogTemp, Log, TEXT(" rooms  num %d"), RoomsNum);
	UE_LOG(LogTemp, Log, TEXT(" rooms edge num %d"), edges.Num());
	for (int i = 0; i < edges.Num(); i++)
	{
		int32 getkey = edges[i].Key;
		int32 getvalue = edges[i].Value;
		DungeonRooms[getkey].ConnectRoomsNum.Add(getvalue);
		UE_LOG(LogTemp, Log, TEXT(" key: %d , value : %d"), getkey,getvalue);
		//DrawDebugLine
		//(
		//	GetWorld(),
		//	DungeonRooms[getkey].GetCenterPos(),
		//	DungeonRooms[getvalue].GetCenterPos(),
		//	FColor::White,
		//	false,
		//	200.0f,
		//	0,
		//	50.0f
		//);

	}
}

void AED_DungeonGenerator::CheckConnectionRooms()
{
	TArray<AED_DungeonTile*> CheckArray;

	for (int i = 0; i < DungeonRooms.Num(); i++)
	{
		if (DungeonRooms[i].DungeonCells.Num() > 0)
		{
			CheckArray.Add(DungeonRooms[i].DungeonCells[0]);
		}
	}

	TArray<TArray<AED_DungeonTile*>> DupleCheckArray;

	for (int i = 0; i < DungeonMap.VaildDungeonCell.Num(); i++)
	{
		DungeonMap.VaildDungeonCell[i]->bIsVisited = false;
	}

	for (int i = 0; i < CheckArray.Num(); i++)
	{
		TArray<AED_DungeonTile*> TempArray = FindBFS(CheckArray[i]);
		if (TempArray.Num() > 0)
		{
			DupleCheckArray.Add(TempArray);
		}
	}
	UE_LOG(LogTemp, Log, TEXT("duple box number : %d"),DupleCheckArray.Num());
	// DungeonRooms 재정의
	DungeonRooms.Empty();
	for (int32 i = 0; i < DupleCheckArray.Num(); i++)
	{
		uint8 Red = FMath::Rand() % 256;
		uint8 Green = FMath::Rand() % 256;
		uint8 Blue = FMath::Rand() % 256;
		FColor RandomColor(Red, Green, Blue);

		for (int32 j = 0; j < DupleCheckArray[i].Num(); j++)
		{
			/*DrawDebugSphere
			(
				GetWorld(),
				DupleCheckArray[i][j]->GetActorLocation(),
				250.0f,
				5.0f,
				RandomColor,
				false,
				150.0f
			);*/
			DupleCheckArray[i][j]->RoomNumber = i;
		}
		FDungeonRoom NewRoom = FDungeonRoom(DupleCheckArray[i], i);
		DungeonRooms.Add(NewRoom);
	}

}

bool AED_DungeonGenerator::CheckVaildGrid(FVector Index)
{
	int32 CheckX = Index.X;
	int32 CheckY = Index.Y;
	int32 CheckZ = Index.Z;
	if (CheckX<0 || CheckX>(DungeonTileNumX - 1))
		return false;
	if (CheckY<0 || CheckY>(DungeonTileNumY - 1))
		return false;
	if (DungeonMap.DungeonData[CheckX][CheckY][CheckZ] == nullptr)
		return false;
	if (DungeonMap.DungeonData[CheckX][CheckY][CheckZ]->bIsRoom==false)
		return false;

	return true;
}

bool AED_DungeonGenerator::CheckVaildIndex(FVector Index)
{
	int32 CheckX = Index.X;
	int32 CheckY = Index.Y;
	int32 CheckZ = Index.Z;
	if (CheckX<0 || CheckX>(DungeonTileNumX - 1))
		return false;
	if (CheckY<0 || CheckY>(DungeonTileNumY - 1))
		return false;
	return true;

}

void AED_DungeonGenerator::MakePathRoomToRoom()
{
	for (int i = 0; i < DungeonRooms.Num(); i++)
	{
		for (int j = 0; j < DungeonRooms[i].RoomToRoomPaths.Num(); j++)
		{
			if (DungeonMap.GetDungeonCell(DungeonRooms[i].RoomToRoomPaths[j]) == nullptr)
			{
				FVector TempSpawnLocation = GetLocationFromIndex(DungeonRooms[i].RoomToRoomPaths[j]);
				AED_DungeonTile* NewTile = GetWorld()->SpawnActor<AED_DungeonTile>(BaseTileClass, TempSpawnLocation, FRotator::ZeroRotator);
				DungeonMap.SetDungeonTile(DungeonRooms[i].RoomToRoomPaths[j], NewTile);
				DungeonMap.VaildDungeonCell.Add(DungeonMap.GetDungeonCell(DungeonRooms[i].RoomToRoomPaths[j]));
				DungeonMap.VaildPaths.Add(NewTile);
				NewTile->bIsPath = true;
				NewTile->bIsRoom = false;
			}
		}
	}
	int32 VaildTileNum = DungeonMap.VaildDungeonCell.Num();
	for (int32 i = 0; i < VaildTileNum; i++)
	{
		AED_DungeonTile* TempTile = DungeonMap.VaildDungeonCell[i];
		if (TempTile != nullptr)
		{
			TempTile->CheckDirectionWall();
			TempTile->CheckNeighborWall();
		}
	}
}

void AED_DungeonGenerator::MakePathByIndexByIndex(TArray<FVector> InPaths)
{

		for (int j = 0; j < InPaths.Num(); j++)
		{
			if (DungeonMap.GetDungeonCell(InPaths[j]) == nullptr)
			{
				FVector TempSpawnLocation = GetLocationFromIndex(InPaths[j]);
				AED_DungeonTile* NewTile = GetWorld()->SpawnActor<AED_DungeonTile>(BaseTileClass, TempSpawnLocation, FRotator::ZeroRotator);
				DungeonMap.SetDungeonTile(InPaths[j], NewTile);
				DungeonMap.VaildDungeonCell.Add(DungeonMap.GetDungeonCell(InPaths[j]));
				DungeonMap.VaildPaths.Add(NewTile);

				NewTile->bIsPath = true;
				NewTile->bIsRoom = false;
			}
		}
	
	int32 VaildTileNum = DungeonMap.VaildDungeonCell.Num();
	for (int32 i = 0; i < VaildTileNum; i++)
	{
		AED_DungeonTile* TempTile = DungeonMap.VaildDungeonCell[i];
		if (TempTile != nullptr)
		{
			TempTile->CheckDirectionWall();
			TempTile->CheckNeighborWall();
		}
	}

}

void AED_DungeonGenerator::FindNearestDoors()
{

	for (int i = 0; i < DungeonRooms.Num(); i++)
	{
		//DungeonRooms[i].EdgeVectors.SetNum(DungeonRooms[i].ConnectRoomsNum.Num());
		
		for (int j = 0; j < DungeonRooms[i].ConnectRoomsNum.Num(); j++)
		{
			FVector TargetPos =DungeonRooms[DungeonRooms[i].ConnectRoomsNum[j]].GetCenterPos();
		    FVector StartIndex = DungeonRooms[i].GetNearestTileIndex(TargetPos);
			FVector StartIndexLocation = DungeonMap.GetDungeonCell(StartIndex)->GetActorLocation();
			FVector TargetIndex = DungeonRooms[DungeonRooms[i].ConnectRoomsNum[j]].GetNearestTileIndex(StartIndexLocation);
			TPair<FVector, FVector> InEdges(StartIndex, TargetIndex);
			DungeonRooms[i].EdgeVectors.Add(InEdges);

		}
	}

	for (int i = 0; i < DungeonRooms.Num(); i++)
	{
		TArray<FVector> PathVectors;

		for (const TPair<FVector, FVector>& Piar : DungeonRooms[i].EdgeVectors)
		{

			FVector KeyIndex = Piar.Key;
			FVector ValueIndex = Piar.Value;

			PathVectors = GetPaths(KeyIndex, ValueIndex);
			for (int j = 0; j < PathVectors.Num(); j++)
			{
				DungeonRooms[i].RoomToRoomPaths.Add(PathVectors[j]);
			}
		}
	}

	for (int i = 0; i < DungeonRooms.Num(); i++)
	{
		for (int j = 0; j < DungeonRooms[i].RoomToRoomPaths.Num(); j++)
		{
			FVector TempSpawnLocation = GetLocationFromIndex(DungeonRooms[i].RoomToRoomPaths[j]);
		/*	DrawDebugSphere
			(
				GetWorld(),
				TempSpawnLocation,
				50.0f,
				5.0f,
				FColor::Yellow,
				false,
				150.0f
			);*/
		}
	}



}

TArray<FVector> AED_DungeonGenerator::GetPaths(FVector StartIndex, FVector TargetIndex)
{
	int count = 0;
	TArray<TArray<FDungeonCell>> TempDungeons;
	TempDungeons.SetNum(DungeonMap.D_SizeX);
	for (int i = 0; i < TempDungeons.Num(); i++)
	{
		TempDungeons[i].SetNum(DungeonMap.D_SizeY);
	}
	for (int i = 0; i < TempDungeons.Num(); i++)
	{
		for (int j = 0; j < TempDungeons[i].Num();j++)
		{
			TempDungeons[i][j] = FDungeonCell();
			TempDungeons[i][j].Index = FVector(i, j, 0);

			if (DungeonMap.GetDungeonCell(FVector(i, j, 0)) != nullptr)
			{
				TempDungeons[i][j].IsRoom = DungeonMap.GetDungeonCell(FVector(i, j, 0))->bIsRoom;
			}
		}
	}
	FVector CheckDirect[] = { FVector(0,1,0),FVector(1,0,0),FVector(0,-1,0),FVector(-1,0,0) };
	TArray<FDungeonCell> FinalNodeList;
	TArray<FDungeonCell> OpenList;
	TArray<FDungeonCell> ClosedList;
	FDungeonCell CurNode;
	FDungeonCell StartNode = TempDungeons[StartIndex.X][StartIndex.Y];
	FDungeonCell TargetNode = TempDungeons[TargetIndex.X][TargetIndex.Y];
	OpenList.Add(StartNode);
	TArray<FVector> RetVectors;

	while (OpenList.Num() > 0)
	{

		CurNode = OpenList[0];
		for (int i = 0; i < OpenList.Num(); i++)
		{
			if (OpenList[i].GetF() <= CurNode.GetF() && OpenList[i].H < CurNode.H)
			{
				CurNode = OpenList[i];
			}
		}
		OpenList.Remove(CurNode);
		ClosedList.Add(CurNode);
		if (CurNode == TargetNode)
		{
			FDungeonCell TargetCurNode = CurNode;
			while (TargetCurNode != StartNode)
			{
				///// 문제되는부분
				FinalNodeList.Add(TargetCurNode);
				TargetCurNode = TempDungeons[TargetCurNode.ParentNodeIndex.X][TargetCurNode.ParentNodeIndex.Y];
				//TargetCurNode = *(TargetCurNode.ParentCell);
			}
			FinalNodeList.Add(StartNode);
			Algo::Reverse(FinalNodeList);

			for (int i = 0; i < FinalNodeList.Num(); i++)
			{
				RetVectors.Add(FinalNodeList[i].Index);
			}

			return RetVectors;
		}
		FVector CheckIndex;
		for (int i = 0; i < 4; i++)
		{
			CheckIndex = CurNode.Index + CheckDirect[i];
			if (CheckVaildIndex(CheckIndex))
			{
				FDungeonCell CheckCell = TempDungeons[CheckIndex.X][CheckIndex.Y];

				//if (CheckCell.IsRoom == true)
				//{
				//	continue;
				//}

				if (!ClosedList.Contains(CheckCell))
				{

					/*	if (TempDungeons[CurNode.Index.X][CheckIndex.Y].IsRoom || TempDungeons[CheckIndex.X][CurNode.Y].IsRoom)
						{
							continue;
						}*/

					int32 MoveCost = CurNode.G + (CurNode.Index.X - CheckIndex.X == 0 || CurNode.Index.Y - CheckIndex.Y == 0 ? 10 : 14);

					if (MoveCost < CheckCell.G || !OpenList.Contains(CheckCell))
					{
						CheckCell.G = MoveCost;
						CheckCell.H = FMath::Abs((CheckCell.Index.X - TargetNode.Index.X) +
							FMath::Abs(CheckCell.Index.Y - TargetNode.Index.Y)) * 10;
						CheckCell.ParentNodeIndex = CurNode.Index;
						CheckCell.ParentCell = &(CurNode);
						TempDungeons[CheckIndex.X][CheckIndex.Y].ParentCell = &(CurNode);;
						TempDungeons[CheckIndex.X][CheckIndex.Y].ParentNodeIndex = CurNode.Index;
						OpenList.Add(CheckCell);
					}
				}
			}
		}
	}


	return RetVectors;

}

TArray<class AED_DungeonTile*> AED_DungeonGenerator::FindBFS(AED_DungeonTile* StartTile)
{
	TArray<AED_DungeonTile*> RetTiles;
	TQueue<AED_DungeonTile*> BfsQueue;
	if (StartTile->bIsVisited == true)
	{
		return RetTiles;
	}
	

	BfsQueue.Enqueue(StartTile);
	RetTiles.Add(StartTile);
	StartTile->bIsVisited = true;
	TArray<FVector> DirVectors = {FVector(0, -1, 0),FVector(0,+1,0),FVector(-1,0,0),FVector(+1,0,0)};
	while (!BfsQueue.IsEmpty())
	{
		AED_DungeonTile* CurrentTile = *(BfsQueue.Peek());
		BfsQueue.Pop();
		for (int i = 0; i < DirVectors.Num(); i++)
		{
			FVector CheckIndex = CurrentTile->TileIndex + DirVectors[i];

			if (CheckVaildGrid(CheckIndex) == false)
			{
				continue;
			}
			if (RetTiles.Contains(DungeonMap.GetDungeonCell(CheckIndex)) == false)
			{
				if (DungeonMap.GetDungeonCell(CheckIndex)->bIsVisited == false)
				{
					BfsQueue.Enqueue(DungeonMap.GetDungeonCell(CheckIndex));
					RetTiles.Add(DungeonMap.GetDungeonCell(CheckIndex));
					DungeonMap.GetDungeonCell(CheckIndex)->bIsVisited = true;
				}
			}

		}
		
	}


	return RetTiles;
}

TArray<class AED_DungeonTile*> AED_DungeonGenerator::DungeonRoomFindBFS(AED_DungeonTile* StartTile)
{
	TArray<AED_DungeonTile*> RetTiles;
	TQueue<AED_DungeonTile*> BfsQueue;
	if (StartTile->bIsVisited == true)
	{
		return RetTiles;
	}
	for (int i = 0; i < DungeonMap.VaildDungeonCell.Num(); i++)
	{
		DungeonMap.VaildDungeonCell[i]->bIsVisited = false;
	}

	BfsQueue.Enqueue(StartTile);
	RetTiles.Add(StartTile);
	StartTile->bIsVisited = true;
	TArray<FVector> DirVectors = { FVector(0, -1, 0),FVector(0,+1,0),FVector(-1,0,0),FVector(+1,0,0) };
	while (!BfsQueue.IsEmpty())
	{
		AED_DungeonTile* CurrentTile = *(BfsQueue.Peek());
		BfsQueue.Pop();
		for (int i = 0; i < DirVectors.Num(); i++)
		{
			FVector CheckIndex = CurrentTile->TileIndex + DirVectors[i];

			if (CheckVaildGrid(CheckIndex) == false)
			{
				continue;
			}
			if (RetTiles.Contains(DungeonMap.GetDungeonCell(CheckIndex)) == false)
			{
				if (DungeonMap.GetDungeonCell(CheckIndex)->bIsVisited == false)
				{
					BfsQueue.Enqueue(DungeonMap.GetDungeonCell(CheckIndex));
					RetTiles.Add(DungeonMap.GetDungeonCell(CheckIndex));
					DungeonMap.GetDungeonCell(CheckIndex)->bIsVisited = true;
				}
			}

		}



	}


	return RetTiles;
}

void AED_DungeonGenerator::GenerateBossRoom()
{
	if (BossRoomBP == nullptr)
	{
		return;
	}

	FBox BossRoomBox = GetRoomByValue(4, 3); // 4 x 3

	TArray<FVector> TempVectors;
	TempVectors = GetIndexFromCube(BossRoomBox);
	TArray<AED_DungeonTile*> TempRoomTiles;
	TempRoomTiles.Empty();
	for (int j = 0; j < TempVectors.Num(); j++)
	{
		FVector SpawnLocation = GetLocationFromIndex(TempVectors[j]);
		AED_DungeonTile* NewTile = GetWorld()->SpawnActor<AED_DungeonTile>(BaseTileClass, SpawnLocation, FRotator::ZeroRotator);

		DungeonMap.SetDungeonTile(TempVectors[j], NewTile);
		DungeonMap.VaildDungeonCell.Add(DungeonMap.GetDungeonCell(TempVectors[j]));
		TempRoomTiles.Add(NewTile);
		NewTile->RoomNumber = 99;
		NewTile->bIsRoom = true;
		NewTile->SetBossRoom();
	
	}
	FDungeonRoom TempDungeonRoom = FDungeonRoom(TempRoomTiles, 99);
	DungeonRooms.Add(TempDungeonRoom);
	FVector SpawnVector = BossRoomBox.GetCenter() - FVector(900.f, 1200.f, 0);// -FVector(-1200, -900, 0);
	ABossRoom* TempBossRoom = GetWorld()->SpawnActor<ABossRoom>(BossRoomBP, SpawnVector, FRotator::ZeroRotator);
	FVector CheckVector = TempBossRoom->GetActorLocation() + TempBossRoom->EntryVector;

	TempBossRoom->DoorTile = DungeonMap.GetDungeonCell(GetIndexFromLocation(CheckVector));

	if (TempBossRoom->DoorTile)
	{
		TArray<FVector> NearestTiles;
		for (int i = 0; i < DungeonRooms.Num(); i++)
		{
			NearestTiles.Add(DungeonRooms[i].GetNearestTileIndex(GetIndexFromLocation(CheckVector)));
		}

		FVector closeIndex;
		float ShortDistance = TNumericLimits<float>::Max();
		
		for (int i = 0; i < NearestTiles.Num(); i++)
		{
			UE_LOG(LogTemp, Log, TEXT("NearestTiles X : %f , Y : %f , Z : %f"), NearestTiles[i].X, NearestTiles[i].Y, NearestTiles[i].Z);
			
			if (DungeonMap.GetDungeonCell(NearestTiles[i])->RoomNumber == 99)
			{
				continue;
			}
			float distanceSquared = FVector::DistSquared(CheckVector, DungeonMap.GetDungeonCell(NearestTiles[i])->GetActorLocation());
			if (distanceSquared<ShortDistance)
			{
				closeIndex = NearestTiles[i];
				ShortDistance = distanceSquared;
			}
		}
		
		TArray<FVector> Paths = GetPaths(GetIndexFromLocation(CheckVector), closeIndex);
		MakePathByIndexByIndex(Paths);
	}
	AED_DungeonTile* LeftEntry = DungeonMap.GetDungeonCell(GetIndexFromLocation(CheckVector + FVector(0, -600, 0)));
	AED_DungeonTile* RightEntry = DungeonMap.GetDungeonCell(GetIndexFromLocation(CheckVector + FVector(0, +600, 0)));

	LeftEntry->BossRoomEntry();
	RightEntry->BossRoomEntry();
}

void AED_DungeonGenerator::FindStartPoint()
{

	//AED_DungeonTile* StartPoint;
	float MaxDistanceSqared = -1;

	AED_DungeonTile* BossRoomTile= nullptr;

	for (int i = 0; i < DungeonRooms.Num(); i++)
	{
		if (DungeonRooms[i].RoomNum == 99)
		{
			BossRoomTile = DungeonRooms[i].DungeonCells[0];
			
		}
	}
	if (BossRoomTile != nullptr)
	{

		FVector BossRoomLocation = BossRoomTile->GetActorLocation();
		TArray<TPair<AED_DungeonTile*, float>> DistanceToTile;
		for (int i = 0; i < DungeonRooms.Num(); i++)
		{
			if (DungeonRooms[i].RoomNum == 99)
			{
				continue;
			}

			AED_DungeonTile* Key = DungeonRooms[i].GetCenterTile();
			float KeyDistance = CalculateDistance(Key->GetActorLocation(), BossRoomLocation);
			
			DistanceToTile.Add(TPair<AED_DungeonTile*, float>(Key, KeyDistance));
		}
		for (int i = 0; i < DistanceToTile.Num(); i++)
		{
			UE_LOG(LogTemp, Log, TEXT("Tile Room Num : %d , Distance : %f "), DistanceToTile[i].Key, DistanceToTile[i].Value);
		}
		DistanceToTile.Sort([](const TPair<AED_DungeonTile*, float>& ip1, const TPair<AED_DungeonTile*, float>& ip2)
			{
				return ip1.Value < ip2.Value;
			});

		for (int i = 0; i < DistanceToTile.Num(); i++)
		{
			UE_LOG(LogTemp, Log, TEXT("Tile Room Num : %d , Distance : %f "), DistanceToTile[i].Key, DistanceToTile[i].Value);
		}
		if (DistanceToTile.Num() > 4)
		{
			StartPoint = DistanceToTile[DistanceToTile.Num() - 3].Key;
		}
		else
		{
			StartPoint = DistanceToTile[DistanceToTile.Num() / 2].Key;
		}

		AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
		//CurrentGameMode->SetPlayerDefaults;
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		PlayerController->GetPawn()->SetActorLocation(StartPoint->GetActorLocation()+FVector(300,300,200));
	}
}

float AED_DungeonGenerator::CalculateDistance(const FVector& Start, const FVector& Target)
{
	return FVector::DistSquared(Start, Target);
}

void AED_DungeonGenerator::OnChandlaier()
{
	int32 ChandlairMinNum = 0;
	int32 TempTileNum = 0;
	for (int i = 0; i < DungeonRooms.Num(); i++)
	{
		TempTileNum = DungeonRooms[i].DungeonCells.Num()-1;
		ChandlairMinNum = TempTileNum /3;

		for (int j = 0; j < ChandlairMinNum; j++)
		{
			int32 RandNum = FMath::RandRange(0, TempTileNum);
			if (DungeonRooms[i].DungeonCells[RandNum] != nullptr)
			{
				DungeonRooms[i].DungeonCells[RandNum]->ChandalierOn();
			}
		}

	}

}

void AED_DungeonGenerator::GenerateProps()
{
	int32 ChandlairMinNum = 0;
	int32 TempTileNum = 0;
	//for (int i = 0; i < DungeonRooms.Num(); i++)
	//{
	//	TempTileNum = DungeonRooms[i].DungeonCells.Num() - 1;
	//	ChandlairMinNum = TempTileNum / 2;

	//	for (int j = 0; j < ChandlairMinNum; j++)
	//	{
	//		int32 RandNum = FMath::RandRange(0, TempTileNum);
	//		if (DungeonRooms[i].DungeonCells[RandNum] != nullptr)
	//		{
	//			DungeonRooms[i].DungeonCells[RandNum]->GenerateOutlineTorch();
	//		}
	//	}

	//}
	TArray<AED_DungeonTile*> OutLines;
	int TorchDivideNum = 4;
	for (int i = 0; i < DungeonRooms.Num(); i++)
	{
		TempTileNum = DungeonRooms[i].DungeonCells.Num() - 1;
		

		for (int j = 0; j < TempTileNum; j++)
		{
			if (DungeonRooms[i].DungeonCells[j]->bIsOutline==true)
			{
				OutLines.Add(DungeonRooms[i].DungeonCells[j]);
			}
		}
	}
	for (int i = 0; i < OutLines.Num(); i++)
	{
		if (i % TorchDivideNum == 0)
		{
			OutLines[i]->GenerateOutlineTorch();
		}
	}


	for (int i = 0; i < DungeonMap.VaildPaths.Num(); i++)
	{
		if (i % 3 == 0)
		{
			DungeonMap.VaildPaths[i]->GenerateOutlineTorch();
		}
	}
	int PropMinNum;
	for (int i = 0; i < DungeonRooms.Num(); i++)
	{
		TempTileNum = DungeonRooms[i].DungeonCells.Num() - 1;
		PropMinNum = TempTileNum / 3;

		for (int j = 0; j < PropMinNum; j++)
		{
			int32 RandNum = FMath::RandRange(0, TempTileNum);
			if (DungeonRooms[i].DungeonCells[RandNum] != nullptr)
			{
				DungeonRooms[i].DungeonCells[RandNum]->GenerateRandomProps();
			}
		}
	}

	SpawnEnemys();


}

void AED_DungeonGenerator::SpawnEnemys()
{
	int MinMonster = 2;
	int MaxMonster = 6;
	int TempTileNum;
	for (int i = 0; i < DungeonRooms.Num(); i++)
	{
		if (DungeonRooms[i].RoomNum == 99)
		{
			continue;
		}

		TempTileNum = DungeonRooms[i].DungeonCells.Num() - 1;
		MaxMonster = TempTileNum / 3;
		int32 RandNum = FMath::RandRange(MinMonster, MaxMonster);

		for (int j = 0; j < RandNum; j++)
		{
			int32 RandTile = FMath::RandRange(0, TempTileNum);

			if (DungeonRooms[i].DungeonCells[RandTile] != nullptr)
			{
				int32 WhatMonster = FMath::RandRange(0, 1);
				if (WhatMonster == 0)
				{
					GetWorld()->SpawnActor<AED_EnemyCharacter>(MonsterDatas->NormalGoblin,
						DungeonRooms[i].DungeonCells[RandTile]->GetActorLocation()+FVector(300,300,20), FRotator::ZeroRotator);
				}
				else
				{
					GetWorld()->SpawnActor<AED_EnemyCharacter>(MonsterDatas->RedGoblin,
						DungeonRooms[i].DungeonCells[RandTile]->GetActorLocation() + FVector(300, 300, 20), FRotator::ZeroRotator);
				}
			}
		}
	}


}

bool AED_DungeonGenerator::CheckTileInsideFBox(const FBox& InBox, const FVector& InTilePos)
{
	return InBox.IsInside(InTilePos);
}

void AED_DungeonGenerator::SettingDungeonFBox()
{


#if ENABLE_DRAW_DEBUG	

	FVector Center = GetActorLocation() + FVector((DungeonTileGapX * (DungeonTileNumX - 1)) / 2, (DungeonTileGapY * (DungeonTileNumY - 1)) / 2, (DungeonTileGapZ * (DungeonTileDepth - 1)) / 2);
	UE_LOG(LogTemp, Log, TEXT("Dungeon Cube Center Point :X  %f Y : %f  Z : %f"), Center.X, Center.Y, Center.Z);

	FColor DrawColor = FColor::Green;  //bResult ? FColor::Green : FColor::Red;	
	FColor DrawRedColor = FColor::Red;  //bResult ? FColor::Green : FColor::Red;	
	float DebugLifeTime = 150.0f;
	FVector ExtentBox = FVector((DungeonTileGapX * DungeonTileNumX) / 2, (DungeonTileGapY * DungeonTileNumY) / 2, (DungeonTileGapZ * DungeonTileDepth) / 2);
	FBox DungeonBox = FBox(FVector(0, 0, 0), FVector((DungeonTileGapX * (DungeonTileNumX - 1)), (DungeonTileGapY * (DungeonTileNumY - 1)), (DungeonTileGapZ * (DungeonTileDepth - 1))));

	TArray<FVector> DungeonVertices = GetBoxVertices(DungeonBox);

	//DrawDebugBox
	//(
	//	GetWorld(),//그릴월드	
	//	Center,//위치	
	//	ExtentBox,
	//	DrawColor, //색깔	
	//	false,//지속여부		
	//	DebugLifeTime //지속시간
	//);
	TArray<FBox> Boxs;
	MinPointX = DungeonBox.Min.X;
	MinPointY = DungeonBox.Min.Y;
	MinPointZ = DungeonBox.Min.Z;
	//MaxPointX = DungeonTileGapX * (DungeonTileNumX-1);
	//MaxPointY = DungeonTileGapY * (DungeonTileNumY-1);
	//MaxPointZ = DungeonTileGapZ * (DungeonTileDepth-1);
	MaxPointX = DungeonBox.Max.X;
	MaxPointY = DungeonBox.Max.Y;
	MaxPointZ = DungeonBox.Max.Z;
	for (int32 i = 0; i < MaxRoom; i++)
	{
		int32 RandomXNum = FMath::RandRange(RoomMinTileNumX, RoomMaxTileNumX);
		int32 RandomYNum = FMath::RandRange(RoomMinTileNumY, RoomMaxTileNumY);

		FVector RandomCenter = UKismetMathLibrary::RandomPointInBoundingBox(Center, ExtentBox);
		int32 ClampCenterX = ((int32)RandomCenter.X / (int32)DungeonTileGapX) * DungeonTileGapX;
		int32 ClampCenterY = ((int32)RandomCenter.Y / (int32)DungeonTileGapY) * DungeonTileGapY;
		int32 ClampCenterZ = ((int32)RandomCenter.Z / (int32)DungeonTileGapZ) * DungeonTileGapZ;
		FVector RoomCenter = FVector(ClampCenterX, ClampCenterY, ClampCenterZ);

		//FVector RoomCenter = UKismetMathLibrary::RandomPointInBoundingBox(Center, ExtentBox);
		//FVector ExtentDungeonBox = FVector((DungeonTileGapX * RandomXNum) / 2, (DungeonTileGapY * RandomYNum) / 2, DungeonTileGapZ / 2);
		FVector ExtentDungeonBox = FVector((DungeonTileGapX * RandomXNum) , (DungeonTileGapY * RandomYNum) , DungeonTileGapZ );

		FVector Min = RoomCenter - ExtentDungeonBox;
		FVector Max = RoomCenter + ExtentDungeonBox;

		FBox RoomBox(Min, Max);
		Boxs.Add(RoomBox);
		float RandomPointX = (FMath::RandRange(MinPointX, MaxPointX) / DungeonTileGapX) * DungeonTileGapX;
		float RandomPointY = (FMath::RandRange(MinPointY, MaxPointY) / DungeonTileGapY) * DungeonTileGapY;
		float RandomPointZ = (FMath::RandRange(MinPointZ, MaxPointZ) / DungeonTileGapZ) * DungeonTileGapZ;
#pragma region CommentRegion
		//UE_LOG(LogTemp, Log, TEXT("MinMaxx Point :  %f , Max : %f , Z : %f"), MinPointX, MaxPointX, DungeonTileGapZ);
		//UE_LOG(LogTemp, Log, TEXT("MinMaxY Point :: %f , Max : %f , Z : %f"), MinPointY, MaxPointY, DungeonTileGapZ);
		//UE_LOG(LogTemp, Log, TEXT("MinMaxZ Point :: %f , Max : %f , Z : %f"), MinPointZ, MaxPointZ, DungeonTileGapZ);

		//UE_LOG(LogTemp, Log, TEXT("Point X : %f , Y : %f , Z : %f"), DungeonTileGapX, DungeonTileGapY, DungeonTileGapZ);

		//UE_LOG(LogTemp, Log, TEXT("Point X : %f , Y : %f , Z : %f"), RandomPointX, RandomPointY, RandomPointZ);
		UE_LOG(LogTemp, Log, TEXT("RandomCenter X : %f , Y : %f , Z : %f"), RandomCenter.X, RandomCenter.Y, RandomCenter.Z);
		UE_LOG(LogTemp, Log, TEXT("RoomCenter X : %f , Y : %f , Z : %f"), RoomCenter.X, RoomCenter.Y, RoomCenter.Z);
#pragma endregion


	}
	for (int32 i = 0; i < Boxs.Num(); i++)
	{
		FColor DColor = FColor::Green;
		for (int32 j = 0; j < Boxs.Num(); j++)
		{
			if (i == j)
			{
				continue;
			}
			if (Boxs[i].Intersect(Boxs[j]))
			{
				DColor = FColor::Red;
				if (DuplicateBoxs.Contains(Boxs[j]) == false)
				{
					DuplicateBoxs.Add(Boxs[j]);
				}

			}
		}

		FVector TempRoomMin = Boxs[i].Min;
		FVector TempRoomMax = Boxs[i].Max;

		if ((TempRoomMin.X < DungeonBox.Min.X) || (DungeonBox.Max.X < TempRoomMax.X))
		{
			DColor = FColor::Purple;
		}

		if ((TempRoomMin.Y < DungeonBox.Min.Y) || (DungeonBox.Max.Y < TempRoomMax.Y))
		{
			DColor = FColor::Purple;
		}

		//DrawDebugBox 
		//(
		//	GetWorld(),//그릴월드	
		//	Boxs[i].GetCenter(),//위치	
		//	Boxs[i].GetExtent(),
		//	DColor, //색깔	
		//	false,//지속여부		
		//	DebugLifeTime //지속시간
		//);
	}
#endif



}

void AED_DungeonGenerator::MergeBoxs()
{

}

