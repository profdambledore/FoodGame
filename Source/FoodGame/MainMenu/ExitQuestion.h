// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"

#include "ExitQuestion.generated.h"

UCLASS()
class FOODGAME_API UExitQuestion : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void CancelButtonReleased();

	UFUNCTION()
		void ExitButtonReleased();

public:
	// Components
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* Blur = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UImage* BackingBox = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* QuestionText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* ExitText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UTextBlock* CancelText = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* ExitButton = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UButton* CancelButton = nullptr;
};
