// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "Engine/Texture2D.h"

#include "ItemData.generated.h"

UCLASS()
class FOODGAME_API UItemData : public UBlueprintFunctionLibrary
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
struct FItem : public FTableRowBase
{
	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float ItemWeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FItemUsage> UsageRules;
};

