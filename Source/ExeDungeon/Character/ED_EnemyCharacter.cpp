// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ED_EnemyCharacter.h"
#include "AI/ED_AIController.h"
#include "CharacterStat/ED_CharacterStatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Item/ED_RootBox.h"

AED_EnemyCharacter::AED_EnemyCharacter()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;

	AIControllerClass = AED_AIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bIsAttacking = false;

	static ConstructorHelpers::FClassFinder<AED_RootBox> RootBoxClassRef(TEXT("/Game/Dungeon/Blueprint/Item/BP_RootBox.BP_RootBox_C"));
	if (RootBoxClassRef.Class)
	{
		RootBoxClass = RootBoxClassRef.Class;
	}
}

void AED_EnemyCharacter::AttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	OnAttackFinished.ExecuteIfBound();
	bIsAttacking = false;
}

void AED_EnemyCharacter::SetDead()
{
	Super::SetDead();
	AED_AIController* EDAIController = Cast<AED_AIController>(GetController());
	if (EDAIController)
	{
		EDAIController->StopAI();
	}
	PlayDeadAnimation();
	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			FVector SpawnVector = FVector(GetActorLocation().X, GetActorLocation().Y, 0);
			AED_RootBox* TempRootBox = GetWorld()->SpawnActor<AED_RootBox>(RootBoxClass, SpawnVector,FRotator::ZeroRotator);
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void AED_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	Stat->SetEnemyLevelStat(1);
}

void AED_EnemyCharacter::EnemyAttack()
{


	// Movement Setting
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboAttackMontage, AttackSpeedRate);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AED_EnemyCharacter::AttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboAttackMontage);
	bIsAttacking = true;


}

float AED_EnemyCharacter::GetAIPatrolRadius()
{
	return 800.0f;
}

float AED_EnemyCharacter::GetAIDetectRange()
{
	return 400.0f;
}

float AED_EnemyCharacter::GetAIAttackRange()
{

	return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius() * 2;
}

float AED_EnemyCharacter::GetAITurnSpeed()
{
	return 2.0f;
}

void AED_EnemyCharacter::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AED_EnemyCharacter::AttackByAI()
{
	//attack;

	OnAttackFinished.ExecuteIfBound();
}

bool AED_EnemyCharacter::GetIsAttacked()
{

	return bIsAttacking;
}
