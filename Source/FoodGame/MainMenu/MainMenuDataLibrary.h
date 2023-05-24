// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MainMenuDataLibrary.generated.h"

UCLASS()
class FOODGAME_API UMainMenuDataLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};

UENUM()
enum ECameraType
{
	Restaurant UMETA(Display Name = "Restaurant"),
	Food UMETA(Display Name = "Food"),
};

UENUM()
enum ERestaurantType
{
	Chain UMETA(Display Name = "Chain"),
	TownCenter UMETA(Display Name = "Town Center"),
	Tutorial UMETA(Display Name = "Tutorial"),
};

UENUM()
enum EFoodType
{
	Burger UMETA(Display Name = "Burger"),
	Pizza UMETA(Display Name = "Pizza"),
};
