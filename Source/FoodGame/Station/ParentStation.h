// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Item/ItemData.h"
#include "Engine/DataTable.h"

#include "ParentStation.generated.h"

UCLASS()
class FOODGAME_API AParentStation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParentStation();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		FItemSlot GetStationSlot(FString Name);

	UFUNCTION(BlueprintCallable)
		FTransform GetSlotTransform(FString SlotName);

	UFUNCTION(BlueprintCallable)
		FRecipe FindRecipe();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* DefaultMesh;

	// Data for station type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slots")
		TEnumAsByte<EStationType> StationType;

	// Data for item slots.  If a slot has item item_all, all items can be placed there
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slots")
		TArray<FItemSlot> ItemSlots;

	// Pointer to the recipe data table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table")
		UDataTable* Recipes = nullptr;

	// Station Recipes
	TArray<FRecipe> StationRecipes;

	// Current Stored Recipe

};
