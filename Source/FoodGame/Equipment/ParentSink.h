// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParentSink.generated.h"

UCLASS()
class FOODGAME_API AParentSink : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParentSink();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to add an item to the sink
	bool GetCanAddPlate();

	// Called to add an item to the sink
	void RemovePlateFromSink(class APlayerCharacter* PlayerToAttachTo);

	// Called to update the status of the plate in the top set index to Clean
	void CleanPlate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* SinkMesh;

	// Set of all plates in the sink
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSet<class APlate*> PlatesInSink;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MaxPlatesInSink = 2;

};
