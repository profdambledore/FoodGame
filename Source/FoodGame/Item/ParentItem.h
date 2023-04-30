// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/StaticMeshComponent.h"

#include "Item/ItemData.h"

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
		void SetupItem(FItem NewItem);

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
		FItem Data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FItemUsage> UsagePoints;

	AActor* AttachedTo = nullptr;

};
