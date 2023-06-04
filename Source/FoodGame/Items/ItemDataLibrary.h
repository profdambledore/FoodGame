// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture2D.h"
#include "Materials/Material.h"
#include "Components/SceneComponent.h"

#include "ItemDataLibrary.generated.h"

UCLASS()
class FOODGAME_API UItemDataLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};

UENUM()
enum EActionState
{
	Disabled UMETA(Display Name = "Disabled"),
	Pressed UMETA(Display Name = "Pressed"),
	Held UMETA(Display Name = "Held"),
};

UENUM()
enum ECookerType
{
	Grill UMETA(Display Name = "Grill"),
	PizzaOven UMETA(Display Name = "PizzaOven"),
};

UENUM() 
enum EConditionState
{
	Clean UMETA(Display Name = "Clean"),
	Dirty UMETA(Display Name = "Dirty"),
	Wet UMETA(Display Name = "Wet"),
};

UENUM()
enum ETraceQuery
{
	HitActor UMETA(Display Name = "Hit Actor"),
	HitItem UMETA(Display Name = "Hit Item"),
	HitChopping UMETA(Display Name = "Hit Chopping Board"),
	HitPlate UMETA(Display Name = "Hit Plate"),
	HitContainer UMETA(Display Name = "Hit Container"),
	HitSink UMETA(Display Name = "Hit Sink"),
};

UENUM()
enum EOrderableType
{
	OrderMain UMETA(Display Name = "Main"),
	OrderAddOn UMETA(Display Name = "Add On"),
	OrderSide UMETA(Display Name = "Side"),
	OrderDrink UMETA(Display Name = "Drink"),
};

UENUM()
enum EOrderableRule
{
	MustBeFirst UMETA(Display Name = "Must Be First"),
	MustBeLast UMETA(Display Name = "Must Be Last"),
	CantBeBounds UMETA(Display Name = "Can't Be Bounds"),
	AnyOrder UMETA(Display Name = "Any Order"),
};


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<class AParentItem> Class;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UMaterial* Material;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bBurnable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bStackable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bBinnable;

	// Unneeded, Deprecate
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bStoreable;
};

USTRUCT(BlueprintType)
struct FItemSlot
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Slot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> AcceptedItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemInSlot;
};

USTRUCT(BlueprintType)
struct FRecipe_Chop : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString ContextItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FString> InputItems;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FString> OutputItems;
};

USTRUCT(BlueprintType)
struct FRecipe_Cook : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TEnumAsByte<ECookerType> CookerType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString InputItems;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float CookTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString OutputItems;
};

// Ordering Structs
USTRUCT(BlueprintType)
struct FStackItem : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TEnumAsByte<EOrderableRule> Rules;
};

USTRUCT(BlueprintType)
struct FStackAsID
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FString> ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bChecked = false;
};

USTRUCT(BlueprintType)
struct FOrderable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FStackItem> Items;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TEnumAsByte<EOrderableType> Type;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Price;
};

USTRUCT(BlueprintType)
struct FOrder
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FOrderable> ItemsInOrder;
};