// Fill out your copyright notice in the Description page of Project Settings.

#include "USGameMode.h"

AUSGameMode::AUSGameMode()
{
    PlayerControllerClass = AUSPlayerController::StaticClass();
    DefaultPawnClass = AUSCharacter::StaticClass();
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_USCharacter"));
    if (PlayerPawnBPClass.Class != nullptr)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }

    PlayerControllerClass = AUSPlayerController::StaticClass();
}
