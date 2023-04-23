// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// Unreal Includes
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

#include "Item/ItemData.h"

#include "PlayerCharacter.generated.h"

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

	// Action
	void Interact();

	// Camera functions
	void SwitchCamera();

	// Arm functions
	void LeftArmPressed();
	void LeftArmReleased();
	void LeftArmTimer();

	void RightArmPressed();
	void RightArmReleased();
	void RightArmTimer();

	void EnableArm(bool bIsLeft);
	void DisableArm(bool bIsLeft);

	UFUNCTION()
		void OnIRBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnIREndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Inspect
	void UpdateInspectWidget(bool bEnable, FVector Location);

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
		USceneComponent* LeftHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
		USceneComponent* RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UWidgetComponent* InspectWidgetComponent;

	// Hand Pointers
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hands")
		class AParentItem* LeftHandItem = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Hands")
		class AParentItem* RightHandItem = nullptr;

protected:
	// Interact
	float InteractRange = 120.0f;
	TArray<AActor*> InteractablesInRange;
	AActor* InteractableLookingAt = nullptr;
	bool bInteractWidgetPlaced = false;

	// Trace Data
	FHitResult TraceHit = FHitResult(ForceInit);
	FVector TraceStart;
	FVector TraceEnd;
	ECollisionChannel TraceChannel = ECC_GameTraceChannel1;
	bool bInteractTrace;

	// Camera
	bool bInThirdPerson;

	// Arms
	float ArmPressTime = 0.5f;

	// Left Arm
	TEnumAsByte<EArmState> LeftArmState = EArmState::Disabled;
	bool bLeftArmPressed = false;
	FTimerHandle LeftArmHandle;

	// Right Arm
	TEnumAsByte<EArmState> RightArmState = EArmState::Disabled;
	bool bRightArmPressed = false;
	FTimerHandle RightArmHandle;
};
