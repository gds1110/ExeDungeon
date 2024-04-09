// Copyright Epic Games, Inc. All Rights Reserved.


#include "ExeDungeonPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "UI/ED_HUDWidget.h"

AExeDungeonPlayerController::AExeDungeonPlayerController()
{
	

}


void AExeDungeonPlayerController::GameOver()
{

	K2_OnGameOver();
}

void AExeDungeonPlayerController::GameClear()
{
	K2_OnGameClear();

}

void AExeDungeonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//// get the enhanced input subsystem
	//if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	//{
	//	// add the mapping context so we get controls
	//	Subsystem->AddMappingContext(InputMappingContext, 0);

	//	UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	//}

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);


}