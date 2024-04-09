// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ED_CharacterBase.h"
#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"
#include "Physics/EDCollision.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterStat/ED_CharacterStatComponent.h"
#include "NiagaraComponent.h"


// Sets default values
AED_CharacterBase::AED_CharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_EDCAPSULE);



	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;


	//stat
	Stat = CreateDefaultSubobject<UED_CharacterStatComponent>(TEXT("Stat"));

	EffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EffectCompo"));
	EffectComponent->SetupAttachment(RootComponent);

}


// Called when the game starts or when spawned
void AED_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
	if (Stat)
	{
		Stat->OnHpZero.AddUObject(this, &AED_CharacterBase::SetDead);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("%s No Stat Component"), *GetName());
	}
}

// Called every frame
void AED_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AED_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AED_CharacterBase::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);


	const float AttackRange = Stat->GetTotalStat().AttackRange;
	const float AttackRadius = Stat->GetAttackRadius();
	const float AttackDamage = Stat->GetTotalStat().Attack;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_EDACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}
	UE_LOG(LogTemp, Log, TEXT("Attack Test"));

#if ENABLE_DRAW_DEBUG

	/*FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 15.0f);*/

#endif
}

float AED_CharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->ApplyDamage(DamageAmount);

	if (EffectComponent)
	{
		EffectComponent->Activate();
	}

	return DamageAmount;
}

void AED_CharacterBase::NotifyComboActionEnd()
{
}

void AED_CharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	SetActorEnableCollision(false);
}

void AED_CharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(0.0f);

		AnimInstance->Montage_Play(DeadMontage, 1.0f);
	}
}

void AED_CharacterBase::SetupCharacterWidget(UED_Widget* InUserWidget)
{


}

int32 AED_CharacterBase::GetLevel()
{
	return Stat->GetCurrentLevel();
}

void AED_CharacterBase::SetLevel(int32 InNewLevel)
{
	Stat->SetLevelStat(InNewLevel);

}

