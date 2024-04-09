// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ExeDungeonPlayerController.generated.h"

class UInputMappingContext;

/**
 *
 */
UCLASS()
class EXEDUNGEON_API AExeDungeonPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
	
public:
	AExeDungeonPlayerController();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameOverCpp"))
	void K2_OnGameOver();
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameClearCpp"))
	void K2_OnGameClear();

protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

public:
	UFUNCTION()
	void GameOver();
	UFUNCTION()
	void GameClear();

	// Begin Actor interface
protected:

	virtual void BeginPlay() override;

	// End Actor interface


};
