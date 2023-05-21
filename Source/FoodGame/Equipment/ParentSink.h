// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/BoxComponent.h"

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

	// Called to update the status of the plate in the top set index to Clean
	void CleanPlate();

	UFUNCTION()
		void OnSRBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnSREndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* SinkMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* SinkRange;

	// Set of all plates in the sink
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class APlate*> PlatesInSink;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
		//int MaxPlatesInSink = 2;

};
