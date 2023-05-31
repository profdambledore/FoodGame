// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu/MainMenuWidget.h"
#include "MainMenu/MainMenuSelection.h"
#include "MainMenu/MainMenuCharacter.h"

// Sets default values
AMainMenuCharacter::AMainMenuCharacter()
{
	// Find UI object and store it
	static ConstructorHelpers::FClassFinder<UUserWidget>UIClass(TEXT("/Game/MainMenu/WBP_MainMenu"));
	if (UIClass.Succeeded()) {
		MainMenuUI = CreateWidget<UMainMenuWidget>(GetWorld(), UIClass.Class);
	};

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMainMenuCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Complete the reference to the player controller and show the mouse cursor
	PC = Cast<APlayerController>(GetController());

	// Add the widget to the players viewport
	if (PC != nullptr) {
		PC->bShowMouseCursor = true;
		MainMenuUI->AddToViewport();
		MainMenuUI->Owner = this;

		// Also setup the level select
		MainMenuUI->SetupLevelSelect();
	}

	// Focus the 'Options' main menu focusable if one is found and hide the back button
	if (MainMenuFocusables.Contains(EMainMenuState::Options)) {
		PC->SetViewTarget(MainMenuFocusables.FindRef(EMainMenuState::Options));
		MainMenuUI->ToogleBackButton(false);
	}
}

// Called every frame
void AMainMenuCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainMenuCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMainMenuCharacter::SetOptionsFocused()
{
	// Focus the 'Options' main menu focusable if one is found
	PC->SetViewTargetWithBlend(MainMenuFocusables.FindRef(EMainMenuState::Options), 1.5f, EViewTargetBlendFunction::VTBlend_EaseInOut, 1.0f, false);

	// Set the index of the switcher to the Options widget
	MainMenuUI->MainMenuSwitcher->SetActiveWidgetIndex(0);
	MainMenuUI->ToogleBackButton(false);
}

void AMainMenuCharacter::SetLevelSelectFocused()
{
	// Focus the current last selected restaurant
	SetCurrentCamera(SelectionMap.FindRef(RestaurantSelected)->SwapToRestaurantCamera());

	// Set the index of the switcher to the Level Select widget
	MainMenuUI->MainMenuSwitcher->SetActiveWidgetIndex(1);
	MainMenuUI->ToogleBackButton(true);
}

void AMainMenuCharacter::SetCreditsFocused()
{
	// Focus the 'Options' main menu focusable if one is found
	PC->SetViewTargetWithBlend(MainMenuFocusables.FindRef(EMainMenuState::Credits), 1.5f, EViewTargetBlendFunction::VTBlend_EaseInOut, 1.0f, false);

	// Set the index of the switcher to the Level Select widget, and show the back button
	MainMenuUI->MainMenuSwitcher->SetActiveWidgetIndex(3);
	MainMenuUI->ToogleBackButton(true);
}

// Level Select Functions
void AMainMenuCharacter::SetRestaurantCamera(TEnumAsByte<ERestaurantType> NewSelection)
{
	// Set the new active section to the input
	RestaurantSelected = NewSelection;
	CurrentCamera = ECameraType::Restaurant;

	// Get the camera from the selection and then call ChangeCurrentCamera
	SelectionMap.FindRef(RestaurantSelected)->SwapToRestaurantCamera();
	SetCurrentCamera(SelectionMap.FindRef(RestaurantSelected));
}

void AMainMenuCharacter::SetFoodCamera(TEnumAsByte<EFoodType> NewFood)
{
	// Set the new food selection to the input
	FoodSelected = NewFood;
	CurrentCamera = ECameraType::Food;

	// Update the active camera on the current selection and then call SetCurrentCamera
	SelectionMap.FindRef(RestaurantSelected)->SwapToFoodCamera();
	SelectionMap.FindRef(RestaurantSelected)->SetNewFoodSelection(NewFood);
	SetCurrentCamera(SelectionMap.FindRef(RestaurantSelected));
}

void AMainMenuCharacter::SetCurrentCamera(AActor* NewSelection)
{
	PC->SetViewTargetWithBlend(NewSelection, 1.5f, EViewTargetBlendFunction::VTBlend_EaseInOut, 1.0f, false);
}

