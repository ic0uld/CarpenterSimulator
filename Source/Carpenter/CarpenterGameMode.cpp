// Copyright Epic Games, Inc. All Rights Reserved.

#include "CarpenterGameMode.h"
#include "CarpenterHUD.h"
#include "CarpenterCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACarpenterGameMode::ACarpenterGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ACarpenterHUD::StaticClass();
}
