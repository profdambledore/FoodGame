// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"
#include "Engine/DataTable.h"

#include "ParentCooker.generated.h"

UCLASS()
class FOODGAME_API AParentCooker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParentCooker();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* OvenMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* CookingRange;

	// Data
		// Pointer to the recipe data table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table")
		UDataTable* Recipes = nullptr;

	// Pointer to the item data table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table")
		UDataTable* Items = nullptr;

};
