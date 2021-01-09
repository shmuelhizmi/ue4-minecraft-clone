// Copyright Epic Games, Inc. All Rights Reserved.

#include "MCGameMode.h"
#include "MCHUD.h"
#include "MCCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMCGameMode::AMCGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AMCHUD::StaticClass();
}
