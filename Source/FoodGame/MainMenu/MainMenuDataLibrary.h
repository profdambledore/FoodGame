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

// The state the main menu is in
UENUM(BlueprintType)
enum EMainMenuState
{
	Options UMETA(Display Name = "Options"),
	LevelSelect UMETA(Display Name = "Level Select"),
	Settings UMETA(Display Name = "Settings"),
	Credits UMETA(Display Name = "Credits"),
};

// The camera that should be active when in the Level Select option
UENUM(BlueprintType)
enum ECameraType
{
	Restaurant UMETA(Display Name = "Restaurant"),
	Food UMETA(Display Name = "Food"),
};

// The restaurant selection that should be focused
UENUM(BlueprintType)
enum ERestaurantType
{
	DriveThru UMETA(Display Name = "Drive Thru"),
	//TownCenter UMETA(Display Name = "Town Center"),
	Tutorial UMETA(Display Name = "Tutorial"),
};

// The food type that should be displayed by the selection and used when a level is selected
UENUM(BlueprintType)
enum EFoodType
{
	Burger UMETA(Display Name = "Burger"),
	Pizza UMETA(Display Name = "Pizza"),
};
