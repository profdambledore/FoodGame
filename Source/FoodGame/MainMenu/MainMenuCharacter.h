// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "MainMenu/MainMenuDataLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"

#include "MainMenuCharacter.generated.h"

class AMainMenuSelection;

UCLASS()
class FOODGAME_API AMainMenuCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainMenuCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void ChangeRestaurantCamera(TEnumAsByte<ERestaurantType> NewSelection);

	UFUNCTION(BlueprintCallable)
		void ChangeSectionCamera();

	UFUNCTION()
		void ChangeCurrentCamera(AActor* NewSelection);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Components

	// UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "User Interface")
		class UMainMenuWidget* MainMenuUI = nullptr;

	float BlendTime = 1.5f;

	// References
	APlayerController* PC;

	// Selections
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<TEnumAsByte<ERestaurantType>, class AMainMenuSelection*> SelectionMap;

	// Current Selections
	TEnumAsByte<ERestaurantType> RestaurantSelected;
	TEnumAsByte<EFoodType> FoodSelected;

	TEnumAsByte<ECameraType> CurrentCamera;
};
