// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// Unreal Includes
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/Material.h"
#include "DrawDebugHelpers.h"

#include "Items/ItemDataLibrary.h"

#include "PlayerCharacter.generated.h"

// Forward Declarations
class AParentItem;
class AParentStation;
class APlate;
class AParentContainer;

UCLASS()
class FOODGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Axis
	void MoveX(float AxisValue);
	void MoveY(float AxisValue);
	void CameraX(float AxisValue);
	void CameraY(float AxisValue);
	void ChangeItem(float AxisValue);

	// Action
	void Interact();
	void TogglePlaceMode();

	// Primary Action Functions
	void PrimaryActionPress();
	void PrimaryActionRelease();
	void PrimaryActionTimer();

	// Secondary Action Functions
	void SecondaryActionPress();
	void SecondaryActionRelease();

	// Camera functions
	void SwitchCamera();

	// Item Overlap Functions
	UFUNCTION()
		void OnIRBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnIREndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Items Functions
	// Called to check if the player can collect the item
	bool CheckCanCollectItem(float NewItemWeight);

	// Called to collect the item and attach it to the player
	void CollectItem(AParentItem* NewItem);

	// Called to place an item from the player's 'inventory' in the world
	void PlaceItem(int PlaceItemIndex);

	// Trace Functions
	// Trace function for interacting with objects
	void InteractTrace();

	// Trace function used to guide the PlacerMesh to a location in the world
	FTransform PlaceTrace();

	// Called to place an item in the world (a default if no items/stations/ect are hit)
	void AttachAt(FVector Location);

public:
	// References
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		APlayerController* PC;

	// Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* FirstPersonCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UCameraComponent* ThirdPersonCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		USpringArmComponent* PlayerCameraSpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		USphereComponent* InteractablesRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		USceneComponent* ItemPosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		UStaticMeshComponent* PlacingMesh;

protected:
	// Pointer to Hologram Material
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data Table")
		UMaterial* PlacerMaterial = nullptr;

	// Interact Properties
	// How far the player can interact in UnrealUnits
	float InteractRange = 170.0f;

	// All interactables in the player's overlap range, used to toggle trace
	TArray<AActor*> InteractablesInRange;

	// The actor the trace is currently hitting
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hands")
		AActor* InteractableLookingAt = nullptr;

	// Bool to designate if the player is in Place Mode (able to place items in their 'inventory')
	bool bPlaceMode = false;

	// Trace Data
	FHitResult TraceHit = FHitResult(ForceInit);
	FVector TraceStart;
	FVector TraceEnd;
	ECollisionChannel TraceChannel;
	bool bTrace;

	// Pointer to last hit station (TO:DO - Replace with a global pointer between all items, not just station)
	class AParentStation* LastHitStation = nullptr;

	// Camera
	// Bool to designate if the player is in first or third person
	bool bInThirdPerson;

	// Items
	// Array of all items currently in the player's 'inventory'
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hands")
		TArray<AParentItem*> HeldItems;

	// The index of the current held item
	int CurrentHeldItem = 0;

	// The current weight of all items in the player's 'inventory'
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weight")
		float CurrentWeight = 0.0f;

	// The maximum weight of items that the player can have in the their 'inventory'
	float MaxWeight = 3.0f;

	// Actions
	// How long it is needed to hold an action button before it is treated as a hold rather than a press
	float ActionPressTime = 0.5f;

	// Primary Action
	// The current state of the Primary Action (Disabled, Pressed or Held)
	TEnumAsByte<EActionState> PrimaryActionState = EActionState::Disabled;

	// Bool to designate if the Primary Action button is pressed or not
	bool bPrimaryActionPressed = false;

	// Timer handle for Holding the Primary Action
	FTimerHandle PrimaryActionHandle;

};
