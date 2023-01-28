// Fill out your copyright notice in the Description page of Project Settings.

#include "USGameMode.h"

AUSGameMode::AUSGameMode()
{
    PlayerControllerClass = AUSPlayerController::StaticClass();
    DefaultPawnClass = AUSCharacter::StaticClass();
    //auto Character =
    //    ConstructorHelpers::FObjectFinder<AUSCharacter>(TEXT("AUSCharacter'/Game/BP_USCharacter.BP_USCharacter'"));
    //if (Character.Object)
    //{
    //    auto Object = Character.Object;
    //    DefaultPawnClass = Object->GetClass()->StaticClass();
    //}
}
