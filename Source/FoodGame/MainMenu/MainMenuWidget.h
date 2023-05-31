// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/WidgetSwitcher.h"

#include "MainMenuWidget.generated.h"

UCLASS()
class FOODGAME_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void SynchronizeProperties() override;
	
	UFUNCTION(BlueprintCallable)
		void ShowExitQuestion();

	UFUNCTION(BlueprintCallable)
		void HideExitQuestion();

	UFUNCTION(BlueprintImplementableEvent)
		void ToogleBackButton(bool bShow);

	UFUNCTION(BlueprintImplementableEvent)
		void SetupLevelSelect();

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UWidgetSwitcher* MainMenuSwitcher = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		class UExitQuestion* ExitQuestion = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class AMainMenuCharacter* Owner = nullptr;
};
