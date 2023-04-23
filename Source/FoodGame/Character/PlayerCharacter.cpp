// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/ParentItem.h"
#include "Character/InspectWidget.h"
#include "Character/PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Component Set Up
	// Mesh
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh>PlayerMeshObject(TEXT("/Game/Assets/Mannequin/Character/Mesh/SK_Mannequin"));
	//if (PlayerMeshObject.Succeeded()) { GetMesh()->SetSkeletalMesh(PlayerMeshObject.Object); }

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// Cameras
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCamera->SetRelativeLocation(FVector(10.0f, 0.0f, 90.0f));

	PlayerCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Third Person Spring Arm"));
	PlayerCameraSpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	PlayerCameraSpringArm->TargetArmLength = 400.0f;
	PlayerCameraSpringArm->bUsePawnControlRotation = 1;

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Third Person Camera"));
	ThirdPersonCamera->SetupAttachment(PlayerCameraSpringArm, USpringArmComponent::SocketName);

	// Interaction
	InteractablesRange = CreateDefaultSubobject<USphereComponent>(TEXT("Interactables Collision"));
	InteractablesRange->SetSphereRadius(InteractRange * 2);
	InteractablesRange->SetupAttachment(GetMesh(), "");

	LeftHand = CreateDefaultSubobject<USceneComponent>(TEXT("Left Hand"));
	LeftHand->SetRelativeLocation(FVector(InteractRange, -40.0f, 20.0f));
	RightHand = CreateDefaultSubobject<USceneComponent>(TEXT("Right Hand"));
	RightHand->SetRelativeLocation(FVector(InteractRange, 40.0f, 20.0f));

	// Inspect
	InspectWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Inspect Widget"));
	static ConstructorHelpers::FClassFinder<UUserWidget>InspectWidgetClass(TEXT("/Game/Player/WBP_InspectWidget"));
	if (InspectWidgetClass.Succeeded()) { InspectWidgetComponent->SetWidgetClass(InspectWidgetClass.Class); };

	// Set the active camera
	ThirdPersonCamera->SetActive(false, false);
	FirstPersonCamera->SetActive(true, true);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add Overlap Events
	InteractablesRange->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnIRBeginOverlap);
	InteractablesRange->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnIREndOverlap);

	// Get the pointer to the InspectWidgetComponent class
	IW = Cast<UInspectWidget>(InspectWidgetComponent->GetUserWidgetObject());
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InteractablesInRange.Num() != 0) {
		// Trace interactables infront of the player
		// Generate information for the trace
		TraceStart = FirstPersonCamera->GetComponentLocation() + (FirstPersonCamera->GetForwardVector() * 50);
		TraceEnd = (TraceStart + (FirstPersonCamera->GetForwardVector() * InteractRange));

		FCollisionQueryParams TraceParams(FName(TEXT("Interact Trace")), true, NULL);
		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = true;

		bInteractTrace = GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, TraceChannel, TraceParams);
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 5.f, ECC_GameTraceChannel1, 1.f);
		if (bInteractTrace) {
			if (TraceHit.Actor->IsA(AParentItem::StaticClass())) {
				// Also set InteractableLookingAt to OtherActor
				InteractableLookingAt = TraceHit.Actor.Get();
				if (!bInteractWidgetPlaced) {
					//InteractWidget->SetComponentLocation(TraceHit.Location)
					if (IW) { 
						UpdateInspectWidget(true, TraceHit.Location);
						// UPDATE: When Interface implemented, change to casting to interface and call GetName (or function name that returns item name)
						AParentItem* itemHit = Cast<AParentItem>(InteractableLookingAt);
						IW->SetInspectingItem(itemHit->GetItemName(), itemHit->UsagePoints, false);
					}
					bInteractWidgetPlaced = true;
				}
			}
			else {
				UpdateInspectWidget(false, FVector(0.0f, 0.0f, 0.0f));
				InteractableLookingAt = nullptr;
				bInteractWidgetPlaced = false;
			}
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Add Axis Binds
	PlayerInputComponent->BindAxis("MoveX", this, &APlayerCharacter::MoveX);
	PlayerInputComponent->BindAxis("MoveY", this, &APlayerCharacter::MoveY);
	PlayerInputComponent->BindAxis("CameraX", this, &APlayerCharacter::CameraX);
	PlayerInputComponent->BindAxis("CameraY", this, &APlayerCharacter::CameraY);

	// Add Action Binds
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
	PlayerInputComponent->BindAction("Camera", IE_Pressed, this, &APlayerCharacter::SwitchCamera);
	PlayerInputComponent->BindAction("LeftArm", IE_Pressed, this, &APlayerCharacter::LeftArmPressed);
	PlayerInputComponent->BindAction("LeftArm", IE_Released, this, &APlayerCharacter::LeftArmReleased);
	PlayerInputComponent->BindAction("RightArm", IE_Pressed, this, &APlayerCharacter::RightArmPressed);
	PlayerInputComponent->BindAction("RightArm", IE_Released, this, &APlayerCharacter::RightArmReleased);
}

// --- Movement ---
void APlayerCharacter::MoveX(float AxisValue)
{
	if (AxisValue != 0) {
		// Move on the X axis based on the input's axis value
		AddMovementInput(GetActorForwardVector(), AxisValue, false);
	}
}

void APlayerCharacter::MoveY(float AxisValue)
{
	if (AxisValue != 0) {
		// Move on the Y axis based on the input's axis value
		AddMovementInput(GetActorRightVector(), AxisValue, false);
	}
}

void APlayerCharacter::CameraX(float AxisValue)
{
	if (AxisValue != 0) {
		// Rotate on the X axis based on the input's axis value
		AddControllerYawInput(AxisValue);
	}
}

void APlayerCharacter::CameraY(float AxisValue)
{
	if (AxisValue != 0) {
		// Rotate on the Y axis based on the input's axis value
		AddControllerPitchInput(AxisValue);
	}
}

// --- Camera ---
void APlayerCharacter::SwitchCamera()
{
	if (bInThirdPerson == false)
	{
		FirstPersonCamera->SetActive(false, false);
		ThirdPersonCamera->SetActive(true, true);

		bInThirdPerson = true;
	}
	else if (bInThirdPerson == true) {
		FirstPersonCamera->SetActive(true, true);
		ThirdPersonCamera->SetActive(false, false);

		bInThirdPerson = false;
	}
}

// --- Interact ---
void APlayerCharacter::OnIRBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the other actor is a class of AParentItem
	// This will need changing if other child classes are introduced later
	if (OtherActor->IsA(AParentItem::StaticClass())) {
		InteractablesInRange.Add(OtherActor);
	}
}

void APlayerCharacter::OnIREndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Find the actor in the array and remover it
	if (OtherActor->IsA(AParentItem::StaticClass())) {
		InteractablesInRange.Remove(OtherActor);
		if (InteractablesInRange.Num() == 0) { InteractableLookingAt = nullptr; }
	}
}

void APlayerCharacter::UpdateInspectWidget(bool bEnable, FVector Location)
{
	if (bEnable) {
		// Show the widget, set it to active and move it
		InspectWidgetComponent->SetActive(true);
		InspectWidgetComponent->SetVisibility(true, true);
		InspectWidgetComponent->SetWorldLocation(Location);
	}
	else {
		// Disable and hide the widget
		InspectWidgetComponent->SetActive(false);
		InspectWidgetComponent->SetVisibility(false, false);
	}
}

void APlayerCharacter::Interact()
{
	
}

// --- Arms --- 
void APlayerCharacter::LeftArmPressed()
{
	bLeftArmPressed = true;
	if (LeftArmState == EArmState::Pressed) {
		// Disable and set arm state to Disabled
		LeftArmState = EArmState::Disabled;
		DisableArm(true);
	}
	else if (InteractableLookingAt != nullptr) {
		if (LeftArmState != EArmState::Pressed) {
			GetWorld()->GetTimerManager().ClearTimer(LeftArmHandle);
			GetWorld()->GetTimerManager().SetTimer(LeftArmHandle, FTimerDelegate::CreateUObject(this, &APlayerCharacter::LeftArmTimer), ArmPressTime, false, ArmPressTime);

			EnableArm(true);
		}
	}
}

void APlayerCharacter::LeftArmReleased()
{
	bLeftArmPressed = false;
	if (LeftArmState == EArmState::Held) {
		// Disable and set arm state to Disabled
		LeftArmState = EArmState::Disabled;
		DisableArm(true);
	}
}

void APlayerCharacter::LeftArmTimer()
{
	if (bLeftArmPressed) {
		// We are holding, set state to Held
		UE_LOG(LogTemp, Warning, TEXT("Held"));
		LeftArmState = EArmState::Held;
	}
	else {
		// We have pressed, set state to Pressed
		UE_LOG(LogTemp, Warning, TEXT("LMB Pressed"));
		LeftArmState = EArmState::Pressed;
	}
}

void APlayerCharacter::RightArmPressed()
{
	bRightArmPressed = true;
	if (RightArmState == EArmState::Pressed) {
		// Disable and set arm state to Disabled
		RightArmState = EArmState::Disabled;
		DisableArm(false);
	}
	else if (InteractableLookingAt != nullptr) {
		if (RightArmState != EArmState::Pressed) {
		GetWorld()->GetTimerManager().ClearTimer(RightArmHandle);
		GetWorld()->GetTimerManager().SetTimer(RightArmHandle, FTimerDelegate::CreateUObject(this, &APlayerCharacter::RightArmTimer), ArmPressTime, false, ArmPressTime);

		EnableArm(false);
		}
	}
}

void APlayerCharacter::RightArmReleased()
{
	bRightArmPressed = false;
	if (RightArmState == EArmState::Held) {
		// Disable and set arm state to Disabled
		RightArmState = EArmState::Disabled;
		DisableArm(false);
	}
}

void APlayerCharacter::RightArmTimer()
{
	if (bRightArmPressed) {
		// We are holding, set state to Held
		UE_LOG(LogTemp, Warning, TEXT("Held"));
		RightArmState = EArmState::Held;
	}
	else {
		// We have pressed, set state to Pressed
		UE_LOG(LogTemp, Warning, TEXT("Pressed"));
		RightArmState = EArmState::Pressed;
	}
}

void APlayerCharacter::EnableArm(bool bIsLeft)
{
	// Check if there is an interactable being looked at
	if (InteractableLookingAt) {
		// Check is the interactable is an item
		if (InteractableLookingAt->IsA(AParentItem::StaticClass())) {
			// If it is, cast to AParentItem
			AParentItem* laItem = Cast<AParentItem>(InteractableLookingAt);
			// Then check if it is a two handed item.  Check if the right hand is filled
			if (laItem->GetTwoHandedItem()) {

			}
			// Else, check which hand to attach it to and store pointer
			else if (bIsLeft) {
				LeftHandItem = laItem;
				laItem->ToggleItemCollision(false);
				laItem->AttachToComponent(LeftHand, FAttachmentTransformRules::SnapToTargetIncludingScale);
				
			}
			else {
				RightHandItem = laItem;
				laItem->ToggleItemCollision(false);
				laItem->AttachToComponent(RightHand, FAttachmentTransformRules::SnapToTargetIncludingScale);
			}
		}
	}
	

}

void APlayerCharacter::DisableArm(bool bIsLeft)
{
	if (bIsLeft) {
		if (LeftHandItem) {
			LeftHandItem->ToggleItemCollision(true);
			LeftHandItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			LeftHandItem->SetActorLocation(FirstPersonCamera->GetComponentLocation() + (FirstPersonCamera->GetForwardVector() * 250));
			LeftHandItem = nullptr;
		}
	}
	else {
		if (RightHandItem) {
			RightHandItem->ToggleItemCollision(true);
			RightHandItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			RightHandItem->SetActorLocation(FirstPersonCamera->GetComponentLocation() + (FirstPersonCamera->GetForwardVector() * 250));
			RightHandItem = nullptr;
		}
	}
	
}

