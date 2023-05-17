// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture2D.h"
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

//UENUM() // Deprecate
//enum EUsageRule
//{
	//Pickup UMETA(Display Name = "Pickup"),
	//Chop UMETA(Display Name = "Chop"),
//};

UENUM() 
enum EConditionState
{
	Clean UMETA(Display Name = "Clean"),
	Dirty UMETA(Display Name = "Dirty"),
	Wet UMETA(Display Name = "Wet"),
};

////struct FItemUsage
//{
//	GENERATED_USTRUCT_BODY();

//public:
	// The input name (MoveX, CameraY, Interact)
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//	FString Input;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//	TEnumAsByte<EUsageRule> Rule;

	// The name of the action (Chop, Pick up)
//	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//	FString Action;
//};

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
		bool bBurnable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bStackable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bBinnable;

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