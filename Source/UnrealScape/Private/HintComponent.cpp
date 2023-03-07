// Fill out your copyright notice in the Description page of Project Settings.

#include "HintComponent.h"
//
//#include "NiagaraComponent.h"
//#include "NiagaraFunctionLibrary.h"

// Sets default values for this component's properties
UHintComponent::UHintComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // Get the hint system class
    ConstructorHelpers::FObjectFinder<UNiagaraSystem> HintAsset(TEXT("/Game/FX/FX_Hint"));
    if (HintAsset.Succeeded())
    {
        HintSystem = HintAsset.Object;
    }
}

// Called when the game starts
void UHintComponent::BeginPlay()
{
    Super::BeginPlay();

    // ...
}

// Called every frame
void UHintComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UHintComponent::CreateHint(FVector Location, bool bClearHints)
{
    if (bClearHints)
    {
        ClearHints();
    }

    UNiagaraComponent* Component = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, HintSystem, Location);
    if (!Component)
    {
        return;
    }
    Hints.Add(Component);
}

void UHintComponent::RemoveHint(int Index)
{
    Hints[Index]->Deactivate();
    Hints.Remove(Hints[Index]);
}

void UHintComponent::ClearHints()
{
    for (int i = Hints.Num() - 1; i >= 0; i--)
    {
        UNiagaraComponent* Hint = Hints[i];
        Hint->Deactivate();
        Hints.Remove(Hint);
    }
}