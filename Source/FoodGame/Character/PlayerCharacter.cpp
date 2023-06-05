#include "Items/ParentItem.h"
#include "Items/ParentStation.h"
#include "Items/Plate.h"
#include "Items/ParentContainer.h"
#include "Equipment/ParentSink.h"
#include "Character/PlayerHUD.h"
#include "Character/OrderManager.h"
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
	FirstPersonCamera->bUsePawnControlRotation = true;

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
	InteractablesRange->SetCollisionProfileName(FName("PlayerOverlap"));

	ItemPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Left Hand"));
	ItemPosition->SetRelativeLocation(FVector(InteractRange, -40.0f, 20.0f));

	PlacingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Placing Mesh"));
	PlacingMesh->SetVisibility(false, false);
	PlacingMesh->SetCollisionProfileName(FName("NoCollision"));

	// Get material object and store it
	ConstructorHelpers::FObjectFinder<UMaterial>MaterialObject(TEXT("/Game/Data/M_PlacerMaterial"));
	if (MaterialObject.Succeeded()) { PlacerMaterial = MaterialObject.Object; }

	// UI
	// Find UI object and store it
	static ConstructorHelpers::FClassFinder<UUserWidget>UIClass(TEXT("/Game/Character/WBP_PlayerHUD"));
	if (UIClass.Succeeded()) {
		PlayerUI = CreateWidget<UPlayerHUD>(GetWorld(), UIClass.Class);
	};

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

	// Get reference to the players controller
	PC = Cast<APlayerController>(GetController());

	// Add the UI to the player's viewport
	if (PlayerUI) {
		PlayerUI->AddToViewport();
		PlayerUI->Owner = this;
	}

	// Find the order manager in the world and set the reference
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOrderManager::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0) {
		OM = Cast<AOrderManager>(FoundActors[0]);
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InteractablesInRange.Num() != 0 && HeldItem == nullptr) {
		TraceForItems();
	}
	// Place mode trace
	else if (PlacingMesh->IsVisible()) {
		PlacingMesh->SetWorldTransform(TraceForHologram());
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
	// Find the actor in the array and remove it
	if (OtherActor->IsA(AParentItem::StaticClass())) {
		InteractablesInRange.Remove(OtherActor);
		if (InteractablesInRange.Num() == 0) { InteractableLookingAt = nullptr; }
	}
}

void APlayerCharacter::Interact()
{
	// TEST FUNCTION -- REMOVE LATER
	TArray<FStackAsID> sAID;

	if (HeldItem) {
		TArray<AParentItem*> toArray = HeldItem->StackedItems.Array();
		for (int i = 0; i < HeldItem->StackedItems.Array().Num(); i++) {
			TArray<FString> StackedIDs;
			StackedIDs.Add(toArray[i]->Data.ID);
			TArray<AParentItem*> toArraytwo = toArray[i]->StackedItems.Array();
			for (int j = 0; j < toArraytwo.Num(); j++) {
				StackedIDs.Add(toArraytwo[j]->Data.ID);
			}
			sAID.Add(FStackAsID{ StackedIDs });
		}
		OM->CompareOrder(sAID);
	}
}

// --- Actions ---
void APlayerCharacter::PrimaryActionPress()
{
	GetWorld()->GetTimerManager().ClearTimer(PrimaryActionHandle);

	if (HeldItem != nullptr) {
		bPrimaryActionPressed = true;
		if (PrimaryActionState != EActionState::Pressed) {	
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
				CollectItem(newItem);
			}
		}
	}
}

void APlayerCharacter::PrimaryActionRelease()
{
	if (HeldItem != nullptr && bPrimaryActionPressed) {
		bPrimaryActionPressed = false;
		PrimaryActionState = EActionState::Disabled;

		PlaceItem();

		// If hologram, end hologram
		if (PlacingMesh->IsVisible()) {
			PlacingMesh->SetVisibility(false, false);
		}

		GetWorld()->GetTimerManager().ClearTimer(PrimaryActionHandle);
	}
}

void APlayerCharacter::PrimaryActionTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("Timer"));
	if (bPrimaryActionPressed) {
		// We are holding, set state to Held
		PrimaryActionState = EActionState::Held;

		// Begin hologram if an item is held
		if (HeldItem != nullptr) {
			PlacingMesh->SetVisibility(true, false);
			PlacingMesh->SetStaticMesh(HeldItem->ItemMesh->GetStaticMesh());
			PlacingMesh->SetMaterial(0, PlacerMaterial);
		}
	}
	else {
		// We have pressed, set state to Pressed
		PrimaryActionState = EActionState::Pressed;
	}
}

void APlayerCharacter::SecondaryActionPress()
{
	// Tracing for any item actions
	// Such as - chopping, cleaning a plate in a sink, getting/placing an item in a container
	TraceStart = FirstPersonCamera->GetComponentLocation() + (FirstPersonCamera->GetForwardVector() * 50);
	TraceEnd = (TraceStart + (FirstPersonCamera->GetForwardVector() * InteractRange));
	TraceChannel = ECC_GameTraceChannel2;
	FVector placeLoc;

	FCollisionQueryParams TraceParams(FName(TEXT("Action Trace")), true, NULL);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;

	bTrace = GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, TraceChannel, TraceParams);
	if (bTrace) {
		switch (GetTraceHitClass(TraceHit.Actor.Get())) {
		case HitChopping:
		{
			AParentStation* HitStation = Cast<AParentStation>(TraceHit.Actor.Get());
			if (HitStation->CurrentRecipes.ID != "") {
				HitStation->CraftRecipe();
			}
			break;
		}
		case HitContainer:
		{
			AParentContainer* HitContainer = Cast<AParentContainer>(TraceHit.Actor.Get());
			// Check if the player is holding an item.  If they are, try to put the item in the container
			if (HeldItem != nullptr) {
				// Try to add the item to the container.  If we do, destroy the item
				if (HitContainer->AddItemToContainer(HeldItem)) {
					HeldItem->Destroy();
					HeldItem = nullptr;
				}
			}
			// If they aren't, collect an item from the container
			else {
				HitContainer->RemoveItemFromContainer(this);
			}
			break;
		}
		case HitSink:
		{
			// If we hit a sink, wash the topmost dirty plate in the set, if there is one in the sink
			AParentSink* HitSink = Cast<AParentSink>(TraceHit.Actor.Get());
			if (HitSink->PlatesInSink.Num() != 0) {
				HitSink->CleanPlate();
			}
			break;
		}
		default:
		{
			break;
		}
		}
	}
}

void APlayerCharacter::SecondaryActionRelease()
{
}

// --- Items --- 
void APlayerCharacter::CollectItem(AParentItem* NewItem)
{
	// Check if the NewItem is attached to another item.  
	if (NewItem->AttachedTo != nullptr) {
		// Check if the attached item is a plate. If it is, 'unattach' it in the plate's slot
		if (TraceHit.Actor->IsA(APlate::StaticClass())) {
			class APlate* attachedTo = Cast<APlate>(NewItem->AttachedTo);
			attachedTo->StackedItems.Remove(NewItem);
			NewItem->AttachedTo = nullptr;
		}
		// Else, it must be in a stack.
		else {
			class AParentItem* attachedTo = Cast<AParentItem>(NewItem->AttachedTo);
			attachedTo->DetachStack(NewItem);
		}
		
	}

	// Set the new item as the HeldItem pointer
	NewItem->ToggleItemCollision(false);
	NewItem->AttachToComponent(ItemPosition, FAttachmentTransformRules::SnapToTargetIncludingScale);
	HeldItem = NewItem;
	ToggleItemShown();
}

void APlayerCharacter::PlaceItem()
{
	if (HeldItem != nullptr) {
		TraceStart = FirstPersonCamera->GetComponentLocation() + (FirstPersonCamera->GetForwardVector() * 50);
		TraceEnd = (TraceStart + (FirstPersonCamera->GetForwardVector() * InteractRange));
		TraceChannel = ECC_GameTraceChannel2;
		FVector placeLoc;

		FCollisionQueryParams TraceParams(FName(TEXT("Drop Trace")), true, NULL);
		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = true;

		bTrace = GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, TraceChannel, TraceParams);
		if (bTrace) {
			switch (GetTraceHitClass(TraceHit.Actor.Get())) {
			case HitItem:
			{
				// Cast to the item
				AParentItem* HitItem = Cast<AParentItem>(TraceHit.Actor);

				// Check if it has an attached actor,
				if (HitItem->AttachedTo != nullptr && HeldItem->Data.bStackable) {
					// If it is attached to something, check if it is another item
					if (HitItem->AttachedTo->IsA(AParentItem::StaticClass())) {
						// If it is, attach the held item to that item
						HeldItem->SetActorLocation(TraceHit.Location);
						HeldItem->SetActorRotation(GetActorRotation() + FRotator(0.0f, 90.0f, 0.0f));

						HitItem = Cast<AParentItem>(HitItem->AttachedTo);
						HitItem->StackedItems.Add(HeldItem);
						HeldItem->AttachToActor(HitItem, FAttachmentTransformRules::KeepWorldTransform);
						HeldItem->AttachedTo = HitItem;
						HeldItem = nullptr;
						ToggleItemShown();
					}
				}

				// Else, check if both items are stackable items
				else if (HitItem->Data.bStackable && HeldItem->Data.bStackable) {
					// If it is, attach the held item to that item
					HeldItem->SetActorLocation(TraceHit.Location);
					HeldItem->SetActorRotation(GetActorRotation() + FRotator(0.0f, 90.0f, 0.0f));

					HitItem->StackedItems.Add(HeldItem);
					HeldItem->AttachToActor(HitItem, FAttachmentTransformRules::KeepWorldTransform);
					HeldItem->AttachedTo = HitItem;
					HeldItem = nullptr;
					ToggleItemShown();
				}

				// Else, place it at the hit location
				else {
					PlaceAt(TraceHit.Location);
				}
				break;
			}
			case HitPlate:
			{
				// If it hits a plate place it on the plate and attach it to that plate
				// Cast to the hit plate
				APlate* HitPlate = Cast<APlate>(TraceHit.Actor);

				// De-attach from the character
				HeldItem->SetActorLocation(TraceHit.Location);
				HeldItem->SetActorRotation(GetActorRotation() + FRotator(0.0f, 90.0f, 0.0f));

				// Attach to the plate
				HitPlate->StackedItems.Add(HeldItem);
				HeldItem->AttachToActor(HitPlate, FAttachmentTransformRules::KeepWorldTransform);
				HeldItem->AttachedTo = HitPlate;
				HeldItem = nullptr;
				ToggleItemShown();
				break;
			}
			default:
				// If it hits something, place it at the hit location
				PlaceAt(TraceHit.Location);
				break;
			}
		}
		// Else, drop it in mid-air
		else {
			PlaceAt(TraceEnd);
		}
	}
}

// --- Tracing ---
void APlayerCharacter::TraceForItems()
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
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 5.f, ECC_GameTraceChannel1, 1.f);

	// If the trace hits something...
	if (bTrace) {
		switch (GetTraceHitClass(TraceHit.Actor.Get())) {
		case HitSink:
		{
			break;
		}

		default:
			// If it is, set InteractableLookingAt
			InteractableLookingAt = TraceHit.Actor.Get();
			break;
		}
	}
	// Else, set InteractableLookingAt to nullptr
	else {
		InteractableLookingAt = nullptr;
	}
}

FTransform APlayerCharacter::TraceForHologram()
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
		return FTransform(FRotator{}, TraceEnd, FVector(1.0f, 1.0f, 1.0f));
	}
}

void APlayerCharacter::PlaceAt(FVector Location)
{
	HeldItem->ToggleItemCollision(true);
	HeldItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	HeldItem->SetActorLocation(Location);
	HeldItem->SetActorRotation(GetActorRotation() + FRotator(0.0f, 90.0f, 0.0f));
	HeldItem = nullptr;
	ToggleItemShown();
}

ETraceQuery APlayerCharacter::GetTraceHitClass(AActor* TraceOutput)
{
	// Query what a trace hit, returning an enum
	if (TraceHit.Actor->IsA(AParentStation::StaticClass())) { return ETraceQuery::HitChopping; }
	else if (TraceHit.Actor->IsA(APlate::StaticClass())) { return ETraceQuery::HitPlate; }
	else if (TraceHit.Actor->IsA(AParentContainer::StaticClass())) {return ETraceQuery::HitContainer; }
	else if (TraceHit.Actor->IsA(AParentSink::StaticClass())) { return ETraceQuery::HitSink; }
	else if (TraceHit.Actor->IsA(AParentItem::StaticClass())) { return ETraceQuery::HitItem; }
	return ETraceQuery::HitActor;
}

