#include "Items/ParentItem.h"
#include "Items/ParentStation.h"
#include "Items/Plate.h"
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
		InteractTrace();
	}
	// Place mode trace
	else if (PlacingMesh->IsVisible()) {
		PlacingMesh->SetWorldTransform(PlaceTrace());
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
	PlayerInputComponent->BindAction("SecondaryAction", IE_Pressed, this, &APlayerCharacter::SecondaryActionPress);
	PlayerInputComponent->BindAction("SecondaryAction", IE_Released, this, &APlayerCharacter::SecondaryActionRelease);
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
				if (CheckCanCollectItem(newItem->GetItemWeight()) == true) {
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

		PlaceItem(CurrentHeldItem);

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

		// Begin hologram if an item is held
		if (HeldItems.Num() != 0) {
			PlacingMesh->SetVisibility(true, false);
			PlacingMesh->SetStaticMesh(HeldItems[0]->ItemMesh->GetStaticMesh());

		}
	}
	else {
		// We have pressed, set state to Pressed
		PrimaryActionState = EActionState::Pressed;
	}
}

void APlayerCharacter::SecondaryActionPress()
{
	TraceStart = FirstPersonCamera->GetComponentLocation() + (FirstPersonCamera->GetForwardVector() * 50);
	TraceEnd = (TraceStart + (FirstPersonCamera->GetForwardVector() * InteractRange));
	TraceChannel = ECC_GameTraceChannel2;
	FVector placeLoc;

	FCollisionQueryParams TraceParams(FName(TEXT("Drop Trace")), true, NULL);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;

	bTrace = GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, TraceChannel, TraceParams);
	if (bTrace) {
		if (TraceHit.Actor->IsA(AParentStation::StaticClass())) {
			LastHitStation = Cast<AParentStation>(TraceHit.Actor.Get());
			if (LastHitStation->CurrentRecipes.ID != "") {
				LastHitStation->CraftRecipe();
			}
		}
	}
}

void APlayerCharacter::SecondaryActionRelease()
{
}

// --- Items --- 
bool APlayerCharacter::CheckCanCollectItem(float NewItemWeight)
{
	if (CurrentWeight + NewItemWeight > MaxWeight) {
		UE_LOG(LogTemp, Warning, TEXT("Cant Collect"));
		return false;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Can Collect"));
		return true;
	}
}

void APlayerCharacter::CollectItem(AParentItem* NewItem)
{
	// Check if the NewItem is attached to another item.  If it is, 'unattach' it in the slot
	if (NewItem->AttachedTo != nullptr) {
		class AParentItem* attachedTo = Cast<AParentItem>(NewItem->AttachedTo);
		NewItem->AttachedTo = nullptr;  attachedTo = nullptr;
	}

	// Add the new item to the array
	NewItem->ToggleItemCollision(false);
	NewItem->AttachToComponent(ItemPosition, FAttachmentTransformRules::SnapToTargetIncludingScale);
	HeldItems.Add(NewItem);
	CurrentWeight = CurrentWeight + NewItem->GetItemWeight();
}

void APlayerCharacter::PlaceItem(int PlaceItemIndex)
{
	if (HeldItems.Num() != 0) {
		TraceStart = FirstPersonCamera->GetComponentLocation() + (FirstPersonCamera->GetForwardVector() * 50);
		TraceEnd = (TraceStart + (FirstPersonCamera->GetForwardVector() * InteractRange));
		TraceChannel = ECC_GameTraceChannel2;
		FVector placeLoc;

		FCollisionQueryParams TraceParams(FName(TEXT("Drop Trace")), true, NULL);
		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = true;

		bTrace = GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, TraceChannel, TraceParams);
		if (bTrace) {
			// If it hits a plate, place it on the plate and attach it to that plate
			if (TraceHit.Actor->IsA(APlate::StaticClass())){
				// Cast to the hit plate
				APlate* HitPlate = Cast<APlate>(TraceHit.Actor);

				// De-attach from the character
				//HeldItems[0]->ToggleItemCollision(true);
				//HeldItems[0]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				HeldItems[0]->SetActorLocation(TraceHit.Location);
				HeldItems[0]->SetActorRotation(GetActorRotation() + FRotator(0.0f, 90.0f, 0.0f));
				CurrentWeight = CurrentWeight - HeldItems[0]->GetItemWeight();

				// Attach to the plate
				HitPlate->AttachedItems.Add(HeldItems[0]);
				HeldItems[0]->AttachToActor(HitPlate, FAttachmentTransformRules::KeepWorldTransform);
				HeldItems[0]->AttachedTo = HitPlate;
				HeldItems.RemoveAt(CurrentHeldItem);
			}
			else {
				// If it hits something, place it at the hit location
				HeldItems[0]->ToggleItemCollision(true);
				HeldItems[0]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				HeldItems[0]->SetActorLocation(TraceHit.Location);
				HeldItems[0]->SetActorRotation(GetActorRotation() + FRotator(0.0f, 90.0f, 0.0f));
				CurrentWeight = CurrentWeight - HeldItems[0]->GetItemWeight();
				HeldItems.RemoveAt(CurrentHeldItem);
			}
		}
		// Else, drop it in mid-air
		else {
			HeldItems[0]->ToggleItemCollision(true);
			HeldItems[0]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			HeldItems[0]->SetActorLocation(TraceEnd);
			CurrentWeight = CurrentWeight - HeldItems[0]->GetItemWeight();
			HeldItems.RemoveAt(CurrentHeldItem);
		}
		// Place item - TO:DO - Replace "0" with DropItemIndex when working
	}
}

// --- Tracing ---
void APlayerCharacter::InteractTrace()
{
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

	// If the trace hits something...
	if (bTrace) {
		// ... check to see if it is of class AParentItem
		if (TraceHit.Actor->IsA(AParentItem::StaticClass())) {
			// If it is, set InteractableLookingAt
			InteractableLookingAt = TraceHit.Actor.Get();
		}
	}
	// Else, set InteractableLookingAt to nullptr
	else {
		InteractableLookingAt = nullptr;
	}
}

FTransform APlayerCharacter::PlaceTrace()
{
	// Trace to see where to place the hologram
	TraceStart = FirstPersonCamera->GetComponentLocation() + (FirstPersonCamera->GetForwardVector() * 50);
	TraceEnd = (TraceStart + (FirstPersonCamera->GetForwardVector() * InteractRange));
	TraceChannel = ECC_GameTraceChannel2;

	FCollisionQueryParams TraceParams(FName(TEXT("Drop Trace")), true, NULL);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;

	bTrace = GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, TraceChannel, TraceParams);

	// If the trace hits something...
	if (bTrace) {
		// If it hits something, rteturn the hit location and make LastHitStation nullptr
		return FTransform(GetActorRotation() + FRotator(0.0f, 90.0f, 0.0f), TraceHit.Location, FVector(1.0f, 1.0f, 1.0f));
	}
	// Else, return the trace end
	else {
		UE_LOG(LogTemp, Warning, TEXT("End"));
		return FTransform(FRotator{}, TraceEnd, FVector(1.0f, 1.0f, 1.0f));
	}
}

