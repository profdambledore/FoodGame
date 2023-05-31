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

	// Options Functions
	UFUNCTION(BlueprintCallable)
		void SetOptionsFocused();

	UFUNCTION(BlueprintCallable)
		void SetLevelSelectFocused();

	UFUNCTION(BlueprintCallable)
		void SetCreditsFocused();

	// Level Select Functions
	UFUNCTION(BlueprintCallable)
		void SetRestaurantCamera(TEnumAsByte<ERestaurantType> NewSelection);

	UFUNCTION(BlueprintCallable)
		void SetSectionCamera();

	UFUNCTION()
		void SetCurrentCamera(AActor* NewSelection);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "User Interface")
		class UMainMenuWidget* MainMenuUI = nullptr;

	float BlendTime = 1.5f;

	// References
	APlayerController* PC;

	// Options Properties
	// The state of the main menu, what object should be focused
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TEnumAsByte<EMainMenuState> MainMenuState;

	// Actors focusable in the main menu (no need to add a level select focusable)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<TEnumAsByte<EMainMenuState>, AActor*> MainMenuFocusables;

	// Level Select Properties
	// TMap of all level selections available in the main menu
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<TEnumAsByte<ERestaurantType>, class AMainMenuSelection*> SelectionMap;

	// Current Level Selections
	TEnumAsByte<ERestaurantType> RestaurantSelected;
	TEnumAsByte<EFoodType> FoodSelected;

	TEnumAsByte<ECameraType> CurrentCamera;
};
