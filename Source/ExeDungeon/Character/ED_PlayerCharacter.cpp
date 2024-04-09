// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ED_PlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EDComboActionData.h"
#include "Physics/EDCollision.h"
#include "Item/ED_ItemDatabase.h"
#include "Inventory/InventoryWidget.h"
#include <Components/Viewport.h>
#include "Item/ED_Item.h"
#include <UI/ED_HpBarWidget.h>
#include "CharacterStat/ED_CharacterStatComponent.h"
#include "UI/ED_HUDWidget.h"
#include "UI/ED_MainWidget.h"
#include "Interface/ED_InteractInterface.h"
#include "Item/ED_LootBoxUI.h"
#include "Equipment/ED_EquipmentWindow.h"
#include "Components/TextBlock.h"
#include "ED_AnimInstance.h"
#include "UI/ED_CastBarWidget.h"
#include "ExeDungeonPlayerController.h"
#include "Interface/ED_GameInterface.h"






// Sets default values
AED_PlayerCharacter::AED_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_EDCAPSULE);

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	PlayerMesh->SetOnlyOwnerSee(true);

	PlayerMesh->SetupAttachment(GetCapsuleComponent());
	PlayerMesh->bCastDynamicShadow = false;
	PlayerMesh->CastShadow = false;
	PlayerMesh->SetCollisionProfileName(TEXT("NoCollision"));
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));

	AutoPossessPlayer = EAutoReceiveInput::Disabled;

	// Create a CameraComponent	
	//FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	////FirstPersonCameraComponent->SetupAttachment(PlayerMesh);
	//FirstPersonCameraComponent->bUsePawnControlRotation = true;
	//FirstPersonCameraComponent->bEditableWhenInherited = true;

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Dungeon/Animation/PlayerAnimation/Blueprint/ABP_PlayerAnimInstance.ABP_PlayerAnimInstance_C"));
	if (AnimInstanceClassRef.Class)
	{
		PlayerMesh->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Dungeon/Input/IMC_Default.IMC_Default'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}


	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Dungeon/Input/Actions/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Dungeon/Input/Actions/IA_Look.IA_Look'"));
	if (nullptr != InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Dungeon/Input/Actions/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}	

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionInventoryRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Dungeon/Input/Actions/IA_Inventory.IA_Inventory'"));
	if (nullptr != InputActionInventoryRef.Object)
	{
		InventoryAction = InputActionInventoryRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionInteractRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Dungeon/Input/Actions/IA_Interact.IA_Interact'"));
	if (nullptr != InputActionInteractRef.Object)
	{
		InteractAction = InputActionInteractRef.Object;
	}	
	static ConstructorHelpers::FObjectFinder<UInputAction> EquipmentActionInteractRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Dungeon/Input/Actions/IA_Equipment.IA_Equipment'"));
	if (nullptr != EquipmentActionInteractRef.Object)
	{
		EquipmentAction = EquipmentActionInteractRef.Object;
	}
	// Weapon Component
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	WeaponMesh->SetupAttachment(PlayerMesh, TEXT("RightWeapon"));
}

// Called when the game starts or when spawned
void AED_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{

		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		UE_LOG(LogTemp, Log, TEXT("BEGINPLAY"));
	}

	Inventory.SetNum(30);
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		FItemData NullItem = FItemData();
		//Inventory.Add(NullItem);
		Inventory[i] = NullItem;
	}
	if (MainWidgetClass)
	{
		MainWidget = CreateWidget<UED_MainWidget>(Cast<APlayerController>(GetController()), MainWidgetClass);
	}

	if (MainWidget != nullptr)
	{
		//if (InventroyWidgetClass)
		//{
		//	//InventoryWidget = CreateWidget<UInventoryWidget>(Cast<APlayerController>(GetController()), InventroyWidgetClass);
		//	//InventoryWidget->SetPlayer(this);
		//	InventoryWidget->Init();
		//	InventoryWidget->AddToViewport(0);
		//	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		//}
		//if (InteractWidgetClass)
		//{
		//	InteractWidget = CreateWidget(Cast<APlayerController>(GetController()), InteractWidgetClass);
		//	InteractWidget->AddToViewport(0);
		//	InteractWidget->SetVisibility(ESlateVisibility::Collapsed);
		//}
		MainWidget->Player = this;
		MainWidget->Init();
		MainWidget->AddToViewport(0);
	
		LootBox = MainWidget->LootBox;
		
	}
	auto Anim = Cast<UED_AnimInstance>(GetMesh1P()->GetAnimInstance());
	if (Anim)
	{
		Anim->ChangeEquipAnim(ECurrentEquipItem::None);
		ComboAttackMontage = ComboPunchMontage;
		ComboActionData = PunchComboActionData;
	}

	if (GetMesh1P())
	{
		GetMesh1P()->HideBoneByName("Head",PBO_None);
		GetMesh1P()->HideBoneByName("Neck",PBO_None);
	}

}

// Called every frame
void AED_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	InteractCheck();
}

// Called to bind functionality to input
void AED_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AED_PlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AED_PlayerCharacter::Look);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AED_PlayerCharacter::Attack);

		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Completed, this, &AED_PlayerCharacter::ToggleInventory);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AED_PlayerCharacter::Interact);

		EnhancedInputComponent->BindAction(EquipmentAction, ETriggerEvent::Completed, this, &AED_PlayerCharacter::ToggleEquipment);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

}

void AED_PlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
	if (MovementVector.X != 0.f|| MovementVector.Y!=0.F)
	{
		InteractInterup();
	}
}

void AED_PlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AED_PlayerCharacter::Attack()
{
	if (MainWidget->InventoryWidget->IsVisible() == true)
	{
		return;
	}

	ProcessComboCommand();
	InteractInterup();
}

void AED_PlayerCharacter::ProcessComboCommand()
{
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimer.IsValid())
	{
		HasNextComboCommand = false;

	}
	else
	{
		HasNextComboCommand = true;
	}

}

void AED_PlayerCharacter::ComboActionBegin()
{
	// Combo Status;

	CurrentCombo = 1;

	
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Anim Setting
	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	UAnimInstance* AnimInstance = GetMesh1P()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboAttackMontage,AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AED_PlayerCharacter::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboAttackMontage);

	ComboTimer.Invalidate();
	SetComboCheckTimer();


}

void AED_PlayerCharacter::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;

}

void AED_PlayerCharacter::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = Stat->GetTotalStat().AttackSpeed;
	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimer, this, &AED_PlayerCharacter::ComboCheck, ComboEffectiveTime, false);
	}

}

void AED_PlayerCharacter::ComboCheck()
{
	ComboTimer.Invalidate();
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh1P()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, ComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, ComboAttackMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
	}
}

void AED_PlayerCharacter::InteractDone()
{
	UED_AnimInstance* AnimInstance = Cast<UED_AnimInstance>(GetMesh1P()->GetAnimInstance());
	if (InteractTarget)
	{
		InteractTarget->InteractAction(this);
		InteractTarget = nullptr;
	}
	AnimInstance->StopAllMontages(0.0f);
	GetWorld()->GetTimerManager().ClearTimer(InteractTimerHandle);
	InteractTimerHandle.Invalidate();
	CastBarWidget->StopUpdate();

	//GetWorld()->GetTimerManager().GetTimerElapsed(InteractTimerHandle);
}

void AED_PlayerCharacter::InteractInterup()
{
	bool bTimerActive = GetWorldTimerManager().IsTimerActive(InteractTimerHandle);
	if (bTimerActive)
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractTimerHandle);
		if (InteractTarget)
		{
			InteractTarget = nullptr;
		}
		if (CastBarWidget)
		{
			if (CastBarWidget->bIsCasting == true)
			{
				CastBarWidget->StopUpdate();
			}
		}
		UED_AnimInstance* AnimInstance = Cast<UED_AnimInstance>(GetMesh1P()->GetAnimInstance());
		AnimInstance->StopAllMontages(0.0f);
	}
}

float AED_PlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	InteractInterup();

	return DamageAmount;
}

void AED_PlayerCharacter::HealHp(float HealAmount)
{
	Stat->HealHp(HealAmount);
}

void AED_PlayerCharacter::StartInteracting()
{
	auto target = Cast<IED_InteractInterface>(InteractHitResult.GetActor());

	
}

void AED_PlayerCharacter::Interact()
{

	//아이템일경우 (몬스터 시체)
	//if (Cast<AED_Item>(InteractHitResult.GetActor()))
	//{
	//	FItemData* Data = ItemDatabase->Items.FindByPredicate([&](const FItemData& ItemData)
	//	{
	//			return ItemData.Class == InteractHitResult.GetActor()->GetClass();
	//	});
	//	Inventory.Emplace(*Data);
	//	InteractHitResult.GetActor()->Destroy();
	//}
	//횃불 또는 문 일경우


	AED_Item* Target = Cast<AED_Item>(InteractHitResult.GetActor());
	if (Target)
	{
		if (Inventory.Contains(Target->ThisData))
		{
			if (Target->ThisData.bIsStackable == EStackType::Stackable)
			{
				for (int32 i = 0; i < Inventory.Num(); i++)
				{					
					if (Inventory[i] == Target->ThisData)
					{
						Inventory[i].Amount += Target->ThisData.Amount;
						MainWidget->InventoryWidget->RefreshInventory(Inventory);
						InteractHitResult.GetActor()->Destroy();
						return;
					}			
				}
			}
		}
		else
		{
			for (int32 i = 0; i < Inventory.Num(); i++)
			{
				if (Inventory[i].ItemType == EItemType::None)
				{
					Inventory[i] = Target->ThisData;
					MainWidget->InventoryWidget->RefreshInventory(Inventory);
					InteractHitResult.GetActor()->Destroy();
					return;
				}
			}
		}

	}
		
	auto target = Cast<IED_InteractInterface>(InteractHitResult.GetActor());
	UED_AnimInstance* AnimInstance = Cast<UED_AnimInstance>(GetMesh1P()->GetAnimInstance());

	if (target)
	{
		

			InteractTarget = target;
	//	AnimInstance->SetCast(true);
		if (InteractTarget->GetInteractTime() > 0)
		{
			AnimInstance->Montage_Play(CastingMontage, 1.0f);
			CastBarWidget->StartUpdate();
			GetWorld()->GetTimerManager().SetTimer(InteractTimerHandle, FTimerDelegate::CreateLambda(
				[&]()
				{
					InteractDone();
				}
			), (InteractTarget->GetInteractTime()), false);
		}
		else
		{
			target->InteractAction(this);
		}

	}
	if (target == nullptr)
	{
		if (LootBox->IsVisible())
		{
	
			ToggleLootBox();
		}

	}
}

void AED_PlayerCharacter::InteractCheck()
{
	playerFronVector = GetActorLocation() + (GetActorForwardVector() * 100.0f);
	Cast<APlayerController>(GetController())->GetPlayerViewPoint(ViewVector, ViewRotation);
	FVector VecDirection = ViewRotation.Vector() * 500;
	InteractVectorEnd = ViewVector + VecDirection;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(InteractHitResult, ViewVector, InteractVectorEnd, ECollisionChannel::ECC_EngineTraceChannel4, QueryParams);
	//DrawDebugLine(GetWorld(), ViewVector, InteractVectorEnd, FColor::Emerald, true, -1, 0, 10);
	if (MainWidget->InteractWidget != nullptr)
	{
		auto InteractHitActor = Cast<IED_InteractInterface>(InteractHitResult.GetActor());
		if (InteractHitActor)
		{
			if (InteractHitActor->GetIsInteract() == true)
			{
				FString InteractTxt = InteractHitActor->GetInterName().ToString() + " Press 'F'";
				UTextBlock* InterBlock = Cast<UTextBlock>(MainWidget->InteractWidget->GetWidgetFromName(TEXT("InteractText")));
				InterBlock->SetText(FText::FromString(InteractTxt));
				MainWidget->InteractWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			MainWidget->InteractWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		if (InteractHitActor == nullptr && InteractTarget != nullptr)
		{
			InteractInterup();
		}
	}
}

void AED_PlayerCharacter::ToggleInventory()
{
	if (MainWidget->InventoryWidget == NULL)
	{
		return;
	}

	if (!MainWidget->InventoryWidget->IsVisible())
	{
		MainWidget->InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		MainWidget->InventoryWidget->RefreshInventory(Inventory);

		Cast<APlayerController>(GetController())->SetInputMode(FInputModeGameAndUI());
		Cast<APlayerController>(GetController())->SetCinematicMode(true, true, true);
		Cast<APlayerController>(GetController())->bShowMouseCursor = true;
		
	}
	else
	{
		MainWidget->InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);

		Cast<APlayerController>(GetController())->SetInputMode(FInputModeGameOnly());
		Cast<APlayerController>(GetController())->SetCinematicMode(false, true, true);
		Cast<APlayerController>(GetController())->bShowMouseCursor = false;
	}
}

void AED_PlayerCharacter::ToggleLootBox()
{
	if (!LootBox->IsVisible())
	{
		LootBox->SetVisibility(ESlateVisibility::Visible);
		

		Cast<APlayerController>(GetController())->SetInputMode(FInputModeGameAndUI());
		Cast<APlayerController>(GetController())->SetCinematicMode(true, true, true);
		Cast<APlayerController>(GetController())->bShowMouseCursor = true;

	}
	else
	{
		LootBox->SetVisibility(ESlateVisibility::Collapsed);

		Cast<APlayerController>(GetController())->SetInputMode(FInputModeGameOnly());
		Cast<APlayerController>(GetController())->SetCinematicMode(false, true, true);
		Cast<APlayerController>(GetController())->bShowMouseCursor = false;
	}

}

void AED_PlayerCharacter::TestAddItemInventory()
{
	

}
void AED_PlayerCharacter::ToggleEquipment()
{
	if (!MainWidget->EquipWidget->IsVisible())
	{
		MainWidget->EquipWidget->SetVisibility(ESlateVisibility::Visible);


		Cast<APlayerController>(GetController())->SetInputMode(FInputModeGameAndUI());
		Cast<APlayerController>(GetController())->SetCinematicMode(true, true, true);
		Cast<APlayerController>(GetController())->bShowMouseCursor = true;

	}
	else
	{
		MainWidget->EquipWidget->SetVisibility(ESlateVisibility::Collapsed);

		Cast<APlayerController>(GetController())->SetInputMode(FInputModeGameOnly());
		Cast<APlayerController>(GetController())->SetCinematicMode(false, true, true);
		Cast<APlayerController>(GetController())->bShowMouseCursor = false;
	}
}

void AED_PlayerCharacter::RefreshInventory()
{
	MainWidget->InventoryWidget->RefreshInventory(Inventory);
}

void AED_PlayerCharacter::SwapInvenItem(int from, int to)
{
	Inventory.Swap(from,to);
	//MainWidget->InventoryWidget->RefreshInventory(Inventory);
}

void AED_PlayerCharacter::EquipWeapon(FItemData InData)
{

	if (InData.ItemSkelMesh != nullptr)
	{
		WeaponMesh->SetSkeletalMesh(InData.ItemSkelMesh);
		// Animation 변경
		auto Anim = Cast<UED_AnimInstance>(GetMesh1P()->GetAnimInstance());
		if (Anim)
		{
			Anim->ChangeEquipAnim(ECurrentEquipItem::TwoHand);
		}

		ComboAttackMontage = ComboTwoHandMontage;
		ComboActionData = TwohandComboActionData;

	}
	switch (InData.ItemType)
	{
		case EItemType::Weapon:
			WeaponSlot = InData;
			Stat->SetWeaponModifierStat(InData.ModifierStat);

			break;

		case EItemType::Armor:
			ArmorSlot = InData;
			Stat->SetArmorModifierStat(InData.ModifierStat);

			break;

	default:
		break;
	}
	MainWidget->RefreshEquipWindow();

	//RefreshRquip
}

void AED_PlayerCharacter::SetupHUDWidget(UED_HUDWidget* InWidget)
{
	if (InWidget)
	{
		//InWidget->SetMaxHP(Stat->GetTotalStat().MaxHp);
		UE_LOG(LogTemp, Log, TEXT("SetupHudWidget Hp : %f"), Stat->GetCurrentHp());
		InWidget->SetMaxHp(Stat->GetCurrentHp());
		InWidget->UpdateHpBar(Stat->GetCurrentHp());

		Stat->OnHpChanged.AddUObject(InWidget, &UED_HUDWidget::UpdateHpBar);
		CastBarWidget = InWidget->CastBar;
	}
}

void AED_PlayerCharacter::RefreshEquipmentWindow()
{
	MainWidget->RefreshEquipWindow();
}

void AED_PlayerCharacter::SetDead()
{
	Super::SetDead();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);

		IED_GameInterface* CurGameMode = Cast<IED_GameInterface>(GetWorld()->GetAuthGameMode());
		if (CurGameMode)
		{
			CurGameMode->OnPlayerDead();
		}
	}
}

bool AED_PlayerCharacter::InventoryAddItem(FItemData _Item)
{
	if (_Item.ItemType!=EItemType::None)
	{
		if (Inventory.Contains(_Item))
		{
			if (_Item.bIsStackable == EStackType::Stackable)
			{
				for (int32 i = 0; i < Inventory.Num(); i++)
				{
					if (Inventory[i] == _Item)
					{
						Inventory[i].Amount += _Item.Amount;
						MainWidget->InventoryWidget->RefreshInventory(Inventory);
						return true;
					}
				}
			}
		}
		else
		{
			for (int32 i = 0; i < Inventory.Num(); i++)
			{
				if (Inventory[i].ItemType == EItemType::None)
				{
					Inventory[i] = _Item;
					MainWidget->InventoryWidget->RefreshInventory(Inventory);
				
					return true;
				}
			}
		}

	}


	return false;
}

void AED_PlayerCharacter::EquipWeaponClear()
{

	WeaponMesh->SetSkeletalMesh(nullptr);
	Stat->SetWeaponModifierStat(FED_CharacterStat());
	//주먹자세로 변경
	auto Anim = Cast<UED_AnimInstance>(GetMesh1P()->GetAnimInstance());
	if (Anim)
	{
		Anim->ChangeEquipAnim(ECurrentEquipItem::None);
	}
	ComboAttackMontage = ComboPunchMontage;
	ComboActionData = PunchComboActionData;
}

