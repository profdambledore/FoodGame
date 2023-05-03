// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/StaticMeshComponent.h"

#include "Items/ItemDataLibrary.h"

#include "ParentItem.generated.h"

UCLASS()
class FOODGAME_API AParentItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParentItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void SetupItem(FItemData NewItem);

	UFUNCTION(BlueprintCallable)
		float GetItemWeight();

	UFUNCTION(BlueprintCallable)
		FString GetItemName();

	UFUNCTION(BlueprintCallable)
		void ToggleItemCollision(bool bSetCollisionOn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* ItemMesh;

	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FItemData Data;

	AActor* AttachedTo = nullptr;

	// Array of all items attached to the plate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plate Data")
		TArray<AParentItem*> AttachedItems;
};
