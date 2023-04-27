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
#include "DrawDebugHelpers.h"

#include "Item/ItemData.h"

#include "PlayerCharacter.generated.h"

class AParentItem;

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

	void PrimaryActionPress();
	void PrimaryActionRelease();
	void PrimaryActionTimer();

	//void SecondaryActionPress();
	//void SecondaryActionRelease();

	// Camera functions
	void SwitchCamera();

	UFUNCTION()
		void OnIRBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnIREndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Items
	bool CheckCanCollectItem(float NewItemWeight);
	void CollectItem(AParentItem* NewItem);
	void PlaceItem(int PlaceItemIndex);

public:	
	// References
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		APlayerController* PC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UInspectWidget* IW = nullptr;

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
	// Interact
	float InteractRange = 170.0f;
	TArray<AActor*> InteractablesInRange;
	AActor* InteractableLookingAt = nullptr;
	bool bInteractWidgetPlaced = false;

	bool bPlaceMode = false;

	// Trace Data
	FHitResult TraceHit = FHitResult(ForceInit);
	FVector TraceStart;
	FVector TraceEnd;
	ECollisionChannel TraceChannel;
	bool bTrace;

	// Camera
	bool bInThirdPerson;

	// Items
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hands")
		TArray<AParentItem*> HeldItems;
	int CurrentHeldItem = 0;

	float CurrentWeight = 0.0f;
	float MaxWeight = 3.0f;

	// Actions
	float ActionPressTime = 0.5f;

	// Primary Action
	TEnumAsByte<EActionState> PrimaryActionState = EActionState::Disabled;
	bool bPrimaryActionPressed = false;
	FTimerHandle PrimaryActionHandle;

	
};
