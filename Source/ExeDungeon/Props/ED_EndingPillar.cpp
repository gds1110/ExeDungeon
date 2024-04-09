// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/ED_EndingPillar.h"
#include "Interface/ED_GameInterface.h"

void AED_EndingPillar::InteractAction()
{
	IED_GameInterface* CurGameMode = Cast<IED_GameInterface>(GetWorld()->GetAuthGameMode());
	if (CurGameMode)
	{
		CurGameMode->GameClear();
	}
}

void AED_EndingPillar::InteractAction(AED_PlayerCharacter* player)
{
	IED_GameInterface* CurGameMode = Cast<IED_GameInterface>(GetWorld()->GetAuthGameMode());
	if (CurGameMode)
	{
		CurGameMode->GameClear();
	}
}

FName AED_EndingPillar::GetInterName()
{
    return InterName;
}

bool AED_EndingPillar::GetIsInteract()
{
    return true;
}
