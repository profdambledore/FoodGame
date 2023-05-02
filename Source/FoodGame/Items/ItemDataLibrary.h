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
enum EStationType
{
	ChoppingBoard UMETA(Display Name = "Chopping Board"),
	Oven UMETA(Display Name = "Oven"),
};

UENUM()
enum EUsageRule
{
	Pickup UMETA(Display Name = "Pickup"),
	Chop UMETA(Display Name = "Chop"),
};

USTRUCT(BlueprintType)
struct FItemUsage
{
	GENERATED_USTRUCT_BODY();

public:
	// The input name (MoveX, CameraY, Interact)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Input;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TEnumAsByte<EUsageRule> Rule;

	// The name of the action (Chop, Pick up)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Action;
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
		float ItemWeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FItemUsage> UsageRules;
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
struct FRecipe : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TEnumAsByte<EStationType> Station;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString ContextItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FString> InputItems;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FString> OutputItems;

};
