// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ParentItem.h"

#include "Components/BoxComponent.h"

#include "ParentContainer.generated.h"

/**
 * 
 */
UCLASS()
class FOODGAME_API AParentContainer : public AParentItem
{
	GENERATED_BODY()
	

public:
	// Sets default values for this actor's properties
	AParentContainer();

	// Setup the Container
	void SetupContainer(FItemData Item, int StartAmount);

	UFUNCTION()
		void OnCRBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool AddItemToContainer(AParentItem* ItemToAdd);
	bool RemoveItemFromContainer(class APlayerCharacter CharacterToGiveItem);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* ContainerRange;

	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container Data")
		FItemData ContainedItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container Data")
		int Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Container Data")
		TArray<FString> AcceptedItems;
};
