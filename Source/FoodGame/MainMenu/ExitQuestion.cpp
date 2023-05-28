// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu/ExitQuestion.h"

void UExitQuestion::NativeConstruct()
{
	// Add OnReleased binding to button
	//if (Button) {
		//Button->OnReleased.AddDynamic(this, &UPauseSelectionWidget::ButtonOnReleased);
	//}
}

void UExitQuestion::CancelButtonReleased()
{
	// Hide the question
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void UExitQuestion::ExitButtonReleased()
{
	// Exit the game
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
