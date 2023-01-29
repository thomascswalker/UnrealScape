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

    // set default controller to our Blueprinted controller
    static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(
        TEXT("/Game/Blueprints/BP_USPlayerController"));
    if (PlayerControllerBPClass.Class != NULL)
    {
        PlayerControllerClass = PlayerControllerBPClass.Class;
    }
}
