// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MainMenuData.generated.h"

UCLASS()
class FOODGAME_API UMainMenuData : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};

UENUM(BlueprintType)
enum EStoreType
{
	DriveThrough UMETA(Display Name = "Drive Through"),
	Mall UMETA(Display Name = "Mall"),
};
