// Fill out your copyright notice in the Description page of Project Settings.

#include "DialogComponent.h"

// Sets default values for this component's properties
UDialogComponent::UDialogComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UDialogComponent::BeginPlay()
{
    Super::BeginPlay();

    // Initialize the dialog asset for this component
    if (DialogAsset)
    {
        DialogAsset->Player = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
        DialogAsset->Npc = Cast<AGameEntity>(GetOuter());
    }
}

// Called every frame
void UDialogComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}
