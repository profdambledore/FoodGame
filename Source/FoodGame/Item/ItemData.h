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
enum EArmState
{
	Disabled UMETA(Display Name = "Disabled"),
	Pressed UMETA(Display Name = "Pressed"),
	Held UMETA(Display Name = "Held"),
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
		bool bRequiresTwoHands;
};
