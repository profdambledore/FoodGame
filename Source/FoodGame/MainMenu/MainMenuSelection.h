// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "MainMenu/MainMenuDataLibrary.h"
#include "Camera/CameraComponent.h"

#include "MainMenuSelection.generated.h"

UCLASS()
class FOODGAME_API AMainMenuSelection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainMenuSelection();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		AActor* SwapToRestaurantCamera();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		AActor* SwapToFoodCamera();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UCameraComponent* RestaurantCamera;

	// Identifier
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<ERestaurantType> SelectionIdentifier;

	// Food Meshes
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<TEnumAsByte<EFoodType>, UStaticMesh*> FoodMeshMap;

};
