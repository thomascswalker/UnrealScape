// Fill out your copyright notice in the Description page of Project Settings.

#include "StaticEntity.h"

// Sets default values
AStaticEntity::AStaticEntity()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetupAttachment(RootComponent);

    InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>(TEXT("InteractiveComponent"));
    InteractiveComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AStaticEntity::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AStaticEntity::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AStaticEntity::Interact_Implementation(const FInteractOption& Option) {}

FVector AStaticEntity::GetFloor_Implementation()
{
    FVector Origin;
    FVector Extent;
    GetActorBounds(true, Origin, Extent);
    return Origin;
}

APlayerController* AStaticEntity::GetPlayer_Implementation()
{
    UWorld* World = GetWorld();
    APlayerController* Controller = UGameplayStatics::GetPlayerController(World, 0);
    return Controller;
}

FString AStaticEntity::GetName_Implementation()
{
    return InteractiveComponent->Name;
}

EEntityType AStaticEntity::GetType_Implementation()
{
    return InteractiveComponent->Type;
}

TArray<FInteractOption> AStaticEntity::GetOptions_Implementation(bool bVisibleOnly)
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

float AStaticEntity::GetInteractDistance_Implementation()
{
    return InteractiveComponent->InteractDistance;
}

UInteractiveComponent* AStaticEntity::GetInteractiveComponent_Implementation()
{
    return InteractiveComponent;
}

UMeshComponent* AStaticEntity::GetMeshComponent_Implementation()
{
    return StaticMeshComponent;
}

void AStaticEntity::SetOptionVisibility_Implementation(const FString& OptionName, bool bVisibility = true)
{
    for (FInteractOption& Option : InteractiveComponent->Options)
    {
        if (Option.Name == OptionName)
        {
            Option.bVisible = bVisibility;
        }
    }
}