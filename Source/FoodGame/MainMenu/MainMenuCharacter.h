// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Components


	// Selections
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<int, class AMainMenuSelection*> SelectionMap;

	// Current Selections
	int CurrentRestraunt = 0;
	int CurrentFoodType = 0;
};
