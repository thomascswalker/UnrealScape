// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnEntity.h"

// Sets default values for this character's properties
APawnEntity::APawnEntity()
{
    PrimaryActorTick.bCanEverTick = true;

    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    if (SkeletalMeshComponent)
    {
        SkeletalMeshComponent->SetupAttachment(RootComponent);
    }

    NavigatorComponent = CreateDefaultSubobject<UNavigatorComponent>(TEXT("NavigationComponent"));
    if (NavigatorComponent)
    {
        AddOwnedComponent(NavigatorComponent);
    }
    
    DialogComponent = CreateDefaultSubobject<UDialogComponent>(TEXT("DialogComponent"));
    if (DialogComponent)
    {
        AddOwnedComponent(DialogComponent);
    }
    
    InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>(TEXT("InteractiveComponent"));
    if (InteractiveComponent)
    {
        AddOwnedComponent(InteractiveComponent);
    }
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
    return InteractiveComponent->Name;
}

EEntityType APawnEntity::GetType_Implementation()
{
    return InteractiveComponent->Type;
}

TArray<FInteractOption> APawnEntity::GetOptions_Implementation(bool bVisibleOnly)
{
    if (!bVisibleOnly)
    {
        return InteractiveComponent->Options;
    }
    TArray<FInteractOption> VisibleOptions;
    for (FInteractOption& Option : InteractiveComponent->Options)
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
    return InteractiveComponent->InteractDistance;
}

UInteractiveComponent* APawnEntity::GetInteractiveComponent_Implementation()
{
    return InteractiveComponent;
}


void APawnEntity::SetOptionVisibility_Implementation(const FString& OptionName, bool bVisibility)
{
    for (FInteractOption& Option : InteractiveComponent->Options)
    {
        if (Option.Name == OptionName)
        {
            Option.bVisible = bVisibility;
        }
    }
}