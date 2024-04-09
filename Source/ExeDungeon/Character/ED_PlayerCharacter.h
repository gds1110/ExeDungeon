// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ED_CharacterBase.h"
#include "GameFramework/Character.h"
#include <Item/ED_ItemDatabase.h>
#include "Interface/ED_HUDInterface.h"
#include "ED_PlayerCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UAnimInstance;
struct FInputActionValue;

class UED_EquipmentWindow;
class UInventoryWidget;
//class UED_ItemDatabase;


UCLASS()
class EXEDUNGEON_API AED_PlayerCharacter : public AED_CharacterBase, public IED_HUDInterface
{
	GENERATED_BODY()

public:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	USkeletalMeshComponent* PlayerMesh;
	
	/** First person camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EquipmentAction;

	
public:
	// Sets default values for this character's properties
	AED_PlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	void Attack();
public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return PlayerMesh; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboPunchMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboTwoHandMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UEDComboActionData> PunchComboActionData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UEDComboActionData> TwohandComboActionData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> CastingMontage;

	void ProcessComboCommand();

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void SetComboCheckTimer();
	void ComboCheck();

	int32 CurrentCombo = 0;
	FTimerHandle ComboTimer;
	bool HasNextComboCommand = false;

	void InteractDone();
	void InteractInterup();

	//Attack Hit Section
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
public:
	void HealHp(float HealAmount);
//Inventroy

public:
	UPROPERTY()
	TArray<FItemData> Inventory;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MainWidget)
	class UED_MainWidget* MainWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = MainWidget)
	TSubclassOf<class UED_MainWidget> MainWidgetClass;

	void StartInteracting();

	void Interact();

	void InteractCheck();
	UFUNCTION()
	void ToggleInventory();
	UFUNCTION()
	void ToggleLootBox();
	UFUNCTION()
	void ToggleEquipment();
	FVector ViewVector;
	FRotator ViewRotation;
	FVector InteractVectorEnd;
	FHitResult InteractHitResult;
	FVector playerFronVector;

	void TestAddItemInventory();
	void RefreshInventory();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LootBox)
	class UED_LootBoxUI* LootBox;
	UPROPERTY()
	TArray<FItemData> LootList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LootBox)
	class AED_RootBox* CurrentBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LootBox)
	TSubclassOf<class UED_LootBoxUI> LootBoxWidgetClass;
	// UI

	void SwapInvenItem(int from, int to);

	bool InventoryAddItem(FItemData _Item);
	
	class IED_InteractInterface* InteractTarget;

	FTimerHandle InteractTimerHandle;
public:
	void EquipWeapon(FItemData InData);
	void EquipWeaponClear();

	void RefreshEquipmentWindow();

	FItemData WeaponSlot;
	FItemData ArmorSlot;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HudWidget)
	class UED_HUDWidget* HUDWidget;

	class UED_CastBarWidget* CastBarWidget;
protected:
		virtual void SetupHUDWidget(class UED_HUDWidget* InWidget) override;
		// Item Section

		virtual void SetDead() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> WeaponMesh;
};
