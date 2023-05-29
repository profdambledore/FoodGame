// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu/MainMenuCharacter.h"
#include "MainMenu/MainMenuSelection.h"

// Sets default values
AMainMenuSelection::AMainMenuSelection()
{
	Root = CreateDefaultSubobject<USceneComponent>(FName("Root"));
	RootComponent = Root;

	RestaurantCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Restaurant Camera"));

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMainMenuSelection::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainMenuSelection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

