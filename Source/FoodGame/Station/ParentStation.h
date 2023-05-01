// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Item/ItemData.h"
#include "Components/BoxComponent.h"
#include "Engine/DataTable.h"

#include "ParentStation.generated.h"

class AParentItem;

UCLASS()
class FOODGAME_API AParentStation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParentStation();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetSlotContextItem(FString ID);
	FTransform GetSlotTransform();
	void AddContextItem(FString ID);
	void RemoveContextItem();

	UFUNCTION(BlueprintCallable)
		void FindRecipe();

	UFUNCTION()
		void CraftRecipe();

	UFUNCTION()
		void OnCRBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnCREndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* DefaultMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* CraftingRange;

	// Data for station type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TEnumAsByte<EStationType> StationType;

	// Data for context item slot.  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		FItemSlot ContextItemSlot;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		TArray<AParentItem*> ItemsInCraftingRange;
	//TArray<FString> ItemsInCraftingRange;

	// Pointer to the recipe data table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table")
		UDataTable* Recipes = nullptr;

	// Station Recipes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Found Recipe")
		FRecipe CurrentRecipes;
};
