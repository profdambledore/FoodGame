// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Plate.h"
#include "Character/PlayerCharacter.h"
#include "Equipment/ParentSink.h"

// Sets default values
AParentSink::AParentSink()
{
	SinkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sink Mesh"));
	RootComponent = SinkMesh;

	SinkRange = CreateDefaultSubobject<UBoxComponent>(TEXT("Crafting Range"));
	SinkRange->SetupAttachment(SinkMesh, "");

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AParentSink::BeginPlay()
{
	Super::BeginPlay();
	
	// Add Overlap Events
	SinkRange->OnComponentBeginOverlap.AddDynamic(this, &AParentSink::OnSRBeginOverlap);
	SinkRange->OnComponentEndOverlap.AddDynamic(this, &AParentSink::OnSREndOverlap);
}

// Called every frame
void AParentSink::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AParentSink::OnSRBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlate::StaticClass())) {
		PlatesInSink.Add(Cast<APlate>(OtherActor));
	}
}

void AParentSink::OnSREndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(APlate::StaticClass())) {
		for (int i = 0; i < PlatesInSink.Num(); i++) {
			if (PlatesInSink[i]->GetName() == OtherActor->GetName()) {
				PlatesInSink.RemoveAt(i);
			}
		}
	}
}

void AParentSink::CleanPlate()
{
	// Find the topmost plate in the set that is dirty and set it to clean
	// TO:DO - Change to Wet and update plate to dry
	if (PlatesInSink.Num() != 0) {
		bool bPlateFound = false;
		for (int i = 0; i < PlatesInSink.Num(); i++) {
			if (!bPlateFound) {
				if (PlatesInSink[i]->Condition == Dirty) {
					PlatesInSink[i]->Condition = Clean;
					bPlateFound = true;
				}
			}
		}
	}
}


