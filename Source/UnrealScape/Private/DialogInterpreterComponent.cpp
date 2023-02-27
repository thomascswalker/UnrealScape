// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogInterpreterComponent.h"

// Sets default values for this component's properties
UDialogInterpreterComponent::UDialogInterpreterComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}

// Called when the game starts
void UDialogInterpreterComponent::BeginPlay()
{
    Super::BeginPlay();

    // ...
}

// Called every frame
void UDialogInterpreterComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UDialogInterpreterComponent::Start(TSubclassOf<UDialogAsset> DialogClass)
{
    if (!IsValid(DialogClass))
    {
        FATAL(TEXT("Invalid DialogAssetClass on NPC."));
        return;
    }
    if (CurrentDialog)
    {
        CurrentDialog->ConditionalBeginDestroy();
    }
    CurrentDialog = NewObject<UDialogAsset>(this, DialogClass, FName("Dialog"));
    if (!IsValid(CurrentDialog))
    {
        FString Message = FString::Printf(L"Unable to create DialogAsset from Class %s", *DialogClass->GetName());
        FATAL(Message);
        return;
    }
    CurrentDialog->Construct();
    bIsTalking = true;
    DialogStarted.Broadcast(CurrentDialog);
}

void UDialogInterpreterComponent::Stop()
{
    if (bIsTalking)
    {
        DialogStopped.Broadcast();
    }
    bIsTalking = false;
}