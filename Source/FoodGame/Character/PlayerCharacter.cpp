// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/ParentItem.h"
#include "Character/PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Find Player Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>PlayerMeshObject(TEXT("/Game/Assets/Mannequin/Character/Mesh/SK_Mannequin"));
	if (PlayerMeshObject.Succeeded()) { GetMesh()->SetSkeletalMesh(PlayerMeshObject.Object); }

	// Component Set Up
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCamera->SetRelativeLocation(FVector(10.0f, 0.0f, 90.0f));

	PlayerCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Third Person Spring Arm"));
	PlayerCameraSpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	PlayerCameraSpringArm->TargetArmLength = 400.0f;
	PlayerCameraSpringArm->bUsePawnControlRotation = 1;

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Third Person Camera"));
	ThirdPersonCamera->SetupAttachment(PlayerCameraSpringArm, USpringArmComponent::SocketName);

	LeftHandCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Left Hand Collision"));
	LeftHandCollision->SetSphereRadius(ArmRadius);
	LeftHandCollision->SetupAttachment(FirstPersonCamera, "");
	LeftHandCollision->SetRelativeLocation(FVector(ArmLength, -40.0f, -20.0f));

	RightHandCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Right Hand Collision"));
	RightHandCollision->SetSphereRadius(ArmRadius);
	RightHandCollision->SetupAttachment(FirstPersonCamera, "");
	RightHandCollision->SetRelativeLocation(FVector(ArmLength, 40.0f, -20.0f));


	// Set the active camera
	ThirdPersonCamera->SetActive(false, false);
	FirstPersonCamera->SetActive(true, true);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	LeftHandCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnLeftHandBeginOverlap);
	LeftHandCollision->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnLeftHandEndOverlap);
	LeftHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RightHandCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnRightHandBeginOverlap);
	RightHandCollision->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnRightHandEndOverlap);
	RightHandCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
void APlayerCharacter::Interact()
{
	// Trace items infront of the player
	// Generate information for the trace
	FHitResult TraceHit = FHitResult(ForceInit);
	FVector TraceStart = FirstPersonCamera->GetComponentLocation() + (FirstPersonCamera->GetForwardVector() * 100);
	FVector TraceEnd = (TraceStart + (FirstPersonCamera->GetForwardVector() * 150));
	ECollisionChannel TraceChannel = ECC_GameTraceChannel1;

	FCollisionQueryParams TraceParams(FName(TEXT("Interact Trace")), true, NULL);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;

	bool bInteractTrace = GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, TraceChannel, TraceParams);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 5.f, ECC_GameTraceChannel1, 1.f);
	if (bInteractTrace) {
		// Check to see if the collided pickup is a inventory pickup
		//if (TraceHit.Actor->GetClass() == AInventoryPickup::StaticClass()) {
		//	AInventoryPickup* pickupToCollect = Cast<AInventoryPickup>(TraceHit.Actor);
		//	if (pickupToCollect) { AddInventory(pickupToCollect->GetComponentClass()); pickupToCollect->CollectPickup(); }
		//}
		// Then check to see if it is a Item Pickup
		//else if (TraceHit.Actor->GetClass() == AItemPickup::StaticClass()) {
		//	AItemPickup* pickupToCollect = Cast<AItemPickup>(TraceHit.Actor);
		//	if (pickupToCollect && InventoryComponent) { InventoryComponent->AddItemToInventory(pickupToCollect->GetItemData());  pickupToCollect->CollectPickup(); };
		//}
	}
}

// --- Arms
void APlayerCharacter::LeftArmPressed()
{
	bLeftArmPressed = true;

	if (LeftArmState != EArmState::Pressed) {
		// Could use lambda here, but this will be used more than once
		ToggleEnableArm(LeftHandCollision, true);

		GetWorld()->GetTimerManager().ClearTimer(LeftArmHandle);
		GetWorld()->GetTimerManager().SetTimer(LeftArmHandle, FTimerDelegate::CreateUObject(this, &APlayerCharacter::LeftArmTimer), ArmPressTime, false, ArmPressTime);
	}
	else {
		// Disable and set arm state to Disabled
		LeftArmState = EArmState::Disabled;
		ToggleEnableArm(LeftHandCollision, true);
	}
}

void APlayerCharacter::LeftArmReleased()
{
	bLeftArmPressed = false;
	if (LeftArmState == EArmState::Held) {
		// Disable and set arm state to Disabled
		LeftArmState = EArmState::Disabled;
		ToggleEnableArm(LeftHandCollision, true);
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
		UE_LOG(LogTemp, Warning, TEXT("Pressed"));
		LeftArmState = EArmState::Pressed;
	}
}

void APlayerCharacter::OnLeftHandBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// First, check if we have something in the left hand
	if (!LeftHandItem) {
		// Then check if the collided actor is a item
		if (OtherActor->IsA(AParentItem::StaticClass())) {
			UE_LOG(LogTemp, Warning, TEXT("Class Check"));
			AParentItem* newItem = Cast<AParentItem>(OtherActor);
			// If it is a two handed item, check if the right hand is filled
			if (newItem->GetTwoHandedItem()) {
		
			}
			// Else, attach it to the hand and store pointer
			else {
				LeftHandItem = newItem;
				newItem->AttachToComponent(LeftHandCollision, FAttachmentTransformRules::SnapToTargetIncludingScale);
				newItem->ToggleItemCollision(false);
			}
		}
	}

}

void APlayerCharacter::OnLeftHandEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// May be needed later
}

void APlayerCharacter::RightArmPressed()
{
	bRightArmPressed = true;

	if (RightArmState != EArmState::Pressed) {
		// Could use lambda here, but this will be used more than once
		ToggleEnableArm(RightHandCollision, false);

		GetWorld()->GetTimerManager().ClearTimer(RightArmHandle);
		GetWorld()->GetTimerManager().SetTimer(RightArmHandle, FTimerDelegate::CreateUObject(this, &APlayerCharacter::RightArmTimer), ArmPressTime, false, ArmPressTime);
	}
	else {
		// Disable and set arm state to Disabled
		RightArmState = EArmState::Disabled;
		ToggleEnableArm(RightHandCollision, false);
	}
}

void APlayerCharacter::RightArmReleased()
{
	bRightArmPressed = false;
	if (RightArmState == EArmState::Held) {
		// Disable and set arm state to Disabled
		RightArmState = EArmState::Disabled;
		ToggleEnableArm(RightHandCollision, false);
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

void APlayerCharacter::OnRightHandBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// First, check if we have something in the right hand
	if (!RightHandItem) {
		// Then check if the collided actor is a item
		if (OtherActor->IsA(AParentItem::StaticClass())) {
			UE_LOG(LogTemp, Warning, TEXT("Class Check"));
			AParentItem* newItem = Cast<AParentItem>(OtherActor);
			// If it is a two handed item, check if the right hand is filled
			if (newItem->GetTwoHandedItem()) {

			}
			// Else, attach it to the hand and store pointer
			else {
				RightHandItem = newItem;
				newItem->AttachToComponent(RightHandCollision, FAttachmentTransformRules::SnapToTargetIncludingScale);
				newItem->ToggleItemCollision(false);
			}
		}
	}

}

void APlayerCharacter::OnRightHandEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// May be needed later
}

void APlayerCharacter::ToggleEnableArm(USphereComponent* ArmToToggle, bool bLeftHand)
{
	if (ArmToToggle->GetCollisionEnabled() == ECollisionEnabled::NoCollision) {
		// Enable the arm
		ArmToToggle->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ArmToToggle->SetHiddenInGame(false);
	}
	else {
		// Disable
		ArmToToggle->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ArmToToggle->SetHiddenInGame(true);
		// Check if the item is two handed
		if (LeftHandItem != nullptr) {
			if (LeftHandItem->GetTwoHandedItem()) {
				// If it is, drop it from both hands
				// Drop Item
				//LeftHandItem = nullptr;
				//RightHandItem = nullptr;
			}
			else if (bLeftHand) {
				// Drop Left Hand Item
				UE_LOG(LogTemp, Warning, TEXT("Deattaching"));
				if (LeftHandItem) {
					LeftHandItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					LeftHandItem->ToggleItemCollision(true);
					LeftHandItem = nullptr;
				}
			}
		}
		else {
			// Drop Right Hand Item
			UE_LOG(LogTemp, Warning, TEXT("Deattaching"));
			if (RightHandItem) {
				RightHandItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				RightHandItem->ToggleItemCollision(true);
				RightHandItem = nullptr;
			}
		}
	
	}
}

