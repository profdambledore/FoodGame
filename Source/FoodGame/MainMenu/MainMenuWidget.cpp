// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu/ExitQuestion.h"
#include "MainMenu/MainMenuWidget.h"

void UMainMenuWidget::NativeConstruct()
{
	// Add OnReleased binding to button
	//if (Button) {
		//Button->OnReleased.AddDynamic(this, &UPauseSelectionWidget::ButtonOnReleased);
	//}

	ExitQuestion->SetVisibility(ESlateVisibility::Collapsed);
}

void UMainMenuWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

}

void UMainMenuWidget::ShowExitQuestion()
{
}

void UMainMenuWidget::HideExitQuestion()
{
}
