#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "CharacterStat/ED_CharacterStatComponent.h"
#include "Templates/SubclassOf.h"
#include "ED_ItemDatabase.generated.h"

class AED_Item;

UENUM()
enum class EItemQuality : uint8
{
	Common UMETA(DisplayName = "Common"),
	Quality UMETA(DisplayName = "Common"),
	Unique UMETA(DisplayName = "Unique")
};
USTRUCT()
struct FItemStatistics
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float ArmorRating;
	UPROPERTY(EditAnywhere)
	float DamageRating;
	UPROPERTY(EditAnywhere)
	float RestorationAmount;
	UPROPERTY(EditAnywhere)
	float SellValue;
};

UENUM(BlueprintType)
enum class EEquipSlotType : uint8
{
	Weapon,
	Armor,
	Gem,
	Scroll,
	Accessory,
	None,
};


USTRUCT()
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FText Name;
	UPROPERTY(EditAnywhere)
	FText Description;
	UPROPERTY(EditAnywhere)
	FText InteractionText;
	UPROPERTY(EditAnywhere)
	FText UsageText;
};
UENUM(BlueprintType)
enum class EItemType : uint8
{
	None,
	Weapon,
	Armor,
	Charm,
	Scroll,
	Potion,
	Gold
};
UENUM(BlueprintType)
enum class EItemLocated : uint8
{
	Inventory,
	RootBox,
	Equipment,
	Ground
};

UENUM(BlueprintType)
enum class ECurrentEquipItem : uint8
{
	None,
	TwoHand,
	OneHand,
};
UENUM(BlueprintType)
enum class EStackType : uint8
{
	NoneStack,
	Stackable,
};


USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	public:
	bool operator==(const FItemData& Other)const
	{
		return (ID == Other.ID)&&(Other.ItemName==ItemName);
	}

	UPROPERTY(EditAnywhere, Category ="Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ItemName;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EStackType bIsStackable;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	TSubclassOf<AActor> ItemClass;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FText Description;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	int32 Amount =1;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	USkeletalMesh* ItemSkelMesh;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FED_CharacterStat ModifierStat;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EEquipSlotType SlotType;
};


UCLASS()
class UItemUIObject : public UObject
{
	GENERATED_BODY()
	
public:
	FItemData ItemData;

};


UCLASS()
class EXEDUNGEON_API UED_ItemDatabase : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly)
	TArray<FItemData> Items;
};
