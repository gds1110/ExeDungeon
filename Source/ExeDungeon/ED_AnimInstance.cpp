// Fill out your copyright notice in the Description page of Project Settings.


#include "ED_AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UED_AnimInstance::UED_AnimInstance()
{
	MovingThreshould = 3.0f;
}


void UED_AnimInstance::SetCast(bool isCast)
{

	bIsCasting = isCast;
	bIsIdle = !isCast;
}

void UED_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UED_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;

	}

}
