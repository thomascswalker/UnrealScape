// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnEntity.h"

// Sets default values for this character's properties
APawnEntity::APawnEntity()
{
    // Navigation
    NavigatorComponent = CreateDefaultSubobject<UNavigatorComponent>(TEXT("NavigationComponent"));
    AddOwnedComponent(NavigatorComponent);

    // Dialog
    DialogComponent = CreateDefaultSubobject<UDialogComponent>(TEXT("DialogComponent"));
    AddOwnedComponent(DialogComponent);

    // Interaction
    InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>(TEXT("InteractiveComponent"));
    AddOwnedComponent(InteractiveComponent);
}

// Called when the game starts or when spawned
void APawnEntity::BeginPlay() {}

// Called every frame
void APawnEntity::Tick(float DeltaTime) {}

void APawnEntity::Interact_Implementation(const FInteractOption& Option) {}

FVector APawnEntity::GetFloor_Implementation()
{
    FVector Origin;
    FVector Extent;
    GetActorBounds(true, Origin, Extent);
    return Origin;
}

APlayerController* APawnEntity::GetPlayer_Implementation()
{
    UWorld* World = GetWorld();
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
    return PlayerController;
}

FString APawnEntity::GetName_Implementation()
{
    return Name;
}

EEntityType APawnEntity::GetType_Implementation()
{
    return Type;
}

TArray<FInteractOption> APawnEntity::GetOptions_Implementation(bool bVisibleOnly)
{
    if (!bVisibleOnly)
    {
        return Options;
    }
    TArray<FInteractOption> VisibleOptions;
    for (FInteractOption& Option : Options)
    {
        if (Option.bVisible)
        {
            VisibleOptions.Add(Option);
        }
    }
    return VisibleOptions;
}

float APawnEntity::GetInteractDistance_Implementation()
{
    return InteractDistance;
}

UInteractiveComponent* APawnEntity::GetInteractiveComponent_Implementation()
{
    return InteractiveComponent;
}


void APawnEntity::SetOptionVisibility_Implementation(const FString& OptionName, bool bVisibility)
{
    for (FInteractOption& Option : Options)
    {
        if (Option.Name == OptionName)
        {
            Option.bVisible = bVisibility;
        }
    }
}