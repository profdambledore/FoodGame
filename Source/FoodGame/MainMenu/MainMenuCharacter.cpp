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
	}

	//MainMenuUI->EditorPlayer = this;

	
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

void AMainMenuCharacter::ChangeRestaurantCamera(TEnumAsByte<ERestaurantType> NewSelection)
{
}

void AMainMenuCharacter::ChangeSectionCamera()
{
}

