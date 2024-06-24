// Copyright Epic Games, Inc. All Rights Reserved.

#include "SecondGameGameMode.h"
#include "SecondGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASecondGameGameMode::ASecondGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
