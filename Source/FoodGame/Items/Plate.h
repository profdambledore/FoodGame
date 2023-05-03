// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ParentItem.h"
#include "Plate.generated.h"

UCLASS()
class FOODGAME_API APlate : public AParentItem
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
		void SetPlateDirty(bool bMakeDirty);

public:
	// Array of all items attached to the plate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plate Data")
		TArray<AParentItem*> AttachedItems;

	bool bDirty = false;
};
