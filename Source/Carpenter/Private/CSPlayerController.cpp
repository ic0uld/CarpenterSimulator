// Fill out your copyright notice in the Description page of Project Settings.


#include "CSPlayerController.h"

#include "Actors/CSComputer.h"
#include "Blueprint/UserWidget.h"
#include "Carpenter/CarpenterCharacter.h"
#include "Components/CSInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void ACSPlayerController::ToggleItemMenu()
{

	if (ItemMenuInstance && ItemMenuInstance->IsInViewport())
	{
		ItemMenuInstance->RemoveFromParent();
		ItemMenuInstance = nullptr;
					
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
		
		return;
	}
	

	
	ACarpenterCharacter* PlayerCharacter = CastChecked<ACarpenterCharacter>(GetPawn());
	

	if (PlayerCharacter)
	{
		ACSComputer* Computer = Cast<ACSComputer>(PlayerCharacter->GetUsableInView());
			if (Computer)
			{
				
				ItemMenuInstance = CreateWidget<UUserWidget>(this, ItemMenuInstanceClass);
				if (ItemMenuInstance)
				{
					ItemMenuInstance->AddToViewport(100);

					bShowMouseCursor = true;
					SetInputMode(FInputModeUIOnly());
		
				}
		}
		
	}
	
}

void ACSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("ItemMenu", IE_Pressed, this, &ACSPlayerController::ToggleItemMenu);
}

void ACSPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChanged.Broadcast(InPawn);
}

void ACSPlayerController::BeginPlayingState()
{
	BlueprintBeginPlayingState();
}

void ACSPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	OnPlayerStateReceived.Broadcast(PlayerState);
}
