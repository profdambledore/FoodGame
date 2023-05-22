// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu/MainMenuSelection.h"
#include "MainMenu/MainMenuCharacter.h"

// Sets default values
AMainMenuCharacter::AMainMenuCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainMenuCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

