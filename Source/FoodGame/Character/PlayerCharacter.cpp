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
	InteractablesRange->SetSphereRadius(InteractRange - 50);
	InteractablesRange->SetupAttachment(GetMesh(), "");

	ItemPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Left Hand"));
	ItemPosition->SetRelativeLocation(FVector(InteractRange, -40.0f, 20.0f));

	PlacingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Placing Mesh"));
	PlacingMesh->SetVisibility(false, false);

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
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InteractablesInRange.Num() != 0 && !bPlaceMode) {
		// Trace interactables infront of the player
		// Generate information for the trace
		TraceStart = FirstPersonCamera->GetComponentLocation() + (FirstPersonCamera->GetForwardVector() * 50);
		TraceEnd = (TraceStart + (FirstPersonCamera->GetForwardVector() * InteractRange));
		TraceChannel = ECC_GameTraceChannel1;

		FCollisionQueryParams TraceParams(FName(TEXT("Interact Trace")), true, NULL);
		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = true;

		bTrace = GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, TraceChannel, TraceParams);
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 5.f, ECC_GameTraceChannel1, 1.f);
		if (bTrace) {
			if (TraceHit.Actor->IsA(AParentItem::StaticClass())) {
				// Also set InteractableLookingAt to OtherActor
				InteractableLookingAt = TraceHit.Actor.Get();
			}
			else {
				InteractableLookingAt = nullptr;
			}
		}
	}
	// Place mode trace
	else if (PlacingMesh->IsVisible()) {
		// Trace to see where to place the hologram
		TraceStart = FirstPersonCamera->GetComponentLocation() + (FirstPersonCamera->GetForwardVector() * 50);
		TraceEnd = (TraceStart + (FirstPersonCamera->GetForwardVector() * InteractRange));
		TraceChannel = ECC_GameTraceChannel1;
		FVector dropLoc;

		FCollisionQueryParams TraceParams(FName(TEXT("Drop Trace")), true, NULL);
		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = true;

		bTrace = GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, TraceChannel, TraceParams);
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 5.f, ECC_GameTraceChannel1, 1.f);
		if (bTrace) {
			PlacingMesh->SetWorldLocation(TraceHit.Location);
		}
		else {
			PlacingMesh->SetWorldLocation(TraceEnd);
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
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &APlayerCharacter::PrimaryActionPress);
	PlayerInputComponent->BindAction("PrimaryAction", IE_Released, this, &APlayerCharacter::PrimaryActionRelease);
	PlayerInputComponent->BindAction("PlaceMode", IE_Pressed, this, &APlayerCharacter::TogglePlaceMode);
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

void APlayerCharacter::ChangeItem(float AxisValue)
{
	if (AxisValue == 1.0f) {
		if (HeldItems.Num() == 0) { 
			// Do nothing
		}
		else if (HeldItems.Num() == 1) {
			CurrentHeldItem = 0;
		}
		else if (CurrentHeldItem + 1 >= HeldItems.Num()) {
			CurrentHeldItem = 0;
		}
		else {
			CurrentHeldItem++;
		}
	}
	else if (AxisValue == -1.0f) {
		if (HeldItems.Num() == 0) {
			// Do nothing
		}
		else if (HeldItems.Num() == 1) {
			CurrentHeldItem = 0;
		}
		else if (CurrentHeldItem - 1 <= -1) {
			CurrentHeldItem = 0;
		}
		else {
			CurrentHeldItem--;
		}
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

void APlayerCharacter::Interact()
{
	
}

void APlayerCharacter::TogglePlaceMode()
{
	bPlaceMode = !bPlaceMode;
}

// --- Actions ---
void APlayerCharacter::PrimaryActionPress()
{
	if (bPlaceMode == true) {
		bPrimaryActionPressed = true;
		if (PrimaryActionState == EActionState::Pressed) {
			// Disable and set arm state to Disabled
			PrimaryActionState = EActionState::Disabled;
		}
		else if (PrimaryActionState != EActionState::Pressed) {
			GetWorld()->GetTimerManager().ClearTimer(PrimaryActionHandle);
			GetWorld()->GetTimerManager().SetTimer(PrimaryActionHandle, FTimerDelegate::CreateUObject(this, &APlayerCharacter::PrimaryActionTimer), ActionPressTime, false, ActionPressTime);
		}
	}
	else {
		// Check if there is an interactable being looked at
		if (InteractableLookingAt) {
			// Check if the interactable is an item
			if (InteractableLookingAt->IsA(AParentItem::StaticClass())) {
				// Cast to AParentItem and check the weight
				AParentItem* newItem = Cast<AParentItem>(InteractableLookingAt);
				if (CheckCanCollectItem(newItem->GetItemWeight())) {
					CollectItem(newItem);
				}
			}
		}
	}
}

void APlayerCharacter::PrimaryActionRelease()
{
	if (bPlaceMode == true) {
		bPrimaryActionPressed = false;
		PrimaryActionState = EActionState::Disabled;

		PlaceItem(0);

		// If hologram, end hologram
		if (PlacingMesh->IsVisible()) {
			PlacingMesh->SetVisibility(false, false);
		}
	}
}

void APlayerCharacter::PrimaryActionTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("Timer"));
	if (bPrimaryActionPressed) {
		// We are holding, set state to Held
		PrimaryActionState = EActionState::Held;

		// Begin hologram
		PlacingMesh->SetVisibility(true, false);
		PlacingMesh->SetStaticMesh(HeldItems[0]->ItemMesh->GetStaticMesh());

	}
	else {
		// We have pressed, set state to Pressed
		PrimaryActionState = EActionState::Pressed;
	}
}

// --- Items --- 
bool APlayerCharacter::CheckCanCollectItem(float NewItemWeight)
{
	if (CurrentWeight + NewItemWeight > MaxWeight) {
		return false;
	}
	else {
		return true;
	}
}

void APlayerCharacter::CollectItem(AParentItem* NewItem)
{
	// Add the new item to the array
	NewItem->ToggleItemCollision(false);
	NewItem->AttachToComponent(ItemPosition, FAttachmentTransformRules::SnapToTargetIncludingScale);
	HeldItems.Add(NewItem);

}

void APlayerCharacter::PlaceItem(int PlaceItemIndex)
{
	TraceStart = FirstPersonCamera->GetComponentLocation() + (FirstPersonCamera->GetForwardVector() * 50);
	TraceEnd = (TraceStart + (FirstPersonCamera->GetForwardVector() * InteractRange));
	TraceChannel = ECC_GameTraceChannel1;
	FVector placeLoc;

	FCollisionQueryParams TraceParams(FName(TEXT("Drop Trace")), true, NULL);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;

	bTrace = GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, TraceChannel, TraceParams);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 5.f, ECC_GameTraceChannel1, 1.f);
	if (bTrace) {
		placeLoc = TraceHit.Location;
	}
	else {
		placeLoc = TraceEnd;
	}

	// Place item - TO:DO - Replace "0" with DropItemIndex when working
	HeldItems[0]->ToggleItemCollision(true);
	HeldItems[0]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	HeldItems[0]->SetActorLocation(placeLoc);
	HeldItems.Remove(0);
}
	

