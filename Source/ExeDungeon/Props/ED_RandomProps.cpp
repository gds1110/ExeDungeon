// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/ED_RandomProps.h"
#include "Engine/AssetManager.h"

AED_RandomProps::AED_RandomProps()
{

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PropMeshComp"));
	RootComponent = MeshComp;

	if (MeshComp)
	{
		MeshComp->SetHiddenInGame(true);
	}
}

void AED_RandomProps::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ensure(RandomMeshs.Num() > 0);
	int32 RandIndex = FMath::RandRange(0, RandomMeshs.Num() - 1);
	PropMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(RandomMeshs[RandIndex], FStreamableDelegate::CreateUObject(this, &AED_RandomProps::PropMeshLoadCompleted));
}

void AED_RandomProps::PropMeshLoadCompleted()
{
	if (PropMeshHandle.IsValid())
	{
		UStaticMesh* PropMesh = Cast<UStaticMesh>(PropMeshHandle->GetLoadedAsset());
		if (PropMesh)
		{
			MeshComp->SetStaticMesh(PropMesh);
			MeshComp->SetHiddenInGame(false);
		}
	}

	PropMeshHandle->ReleaseHandle();

}

bool AED_RandomProps::GetIsInteract()
{
	return false;
}
