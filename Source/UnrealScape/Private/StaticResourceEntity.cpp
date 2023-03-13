// Fill out your copyright notice in the Description page of Project Settings.

#include "StaticResourceEntity.h"

// Sets default values
AStaticResourceEntity::AStaticResourceEntity()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetupAttachment(RootComponent);

    InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>(TEXT("InteractiveComponent"));
    InteractiveComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AStaticResourceEntity::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AStaticResourceEntity::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AStaticResourceEntity::Interact_Implementation(const FInteractOption& Option) {}
void AStaticResourceEntity::Interrupt_Implementation()
{
    GetWorld()->GetTimerManager().ClearTimer(GatherTimerHandle);
    InteractiveComponent->Complete.Broadcast();
}
void AStaticResourceEntity::OnGather_Implementation() {}

void AStaticResourceEntity::StartGather()
{
    GetWorld()->GetTimerManager().SetTimer(GatherTimerHandle, this, &AStaticResourceEntity::OnGather, GatherRate, true);
}

void AStaticResourceEntity::EndGather()
{
    GetWorld()->GetTimerManager().ClearTimer(GatherTimerHandle);
}
FVector AStaticResourceEntity::GetFloor_Implementation()
{
    FVector Origin;
    FVector Extent;
    GetActorBounds(true, Origin, Extent);
    return Origin;
}

APlayerController* AStaticResourceEntity::GetPlayer_Implementation()
{
    UWorld* World = GetWorld();
    APlayerController* Controller = UGameplayStatics::GetPlayerController(World, 0);
    return Controller;
}

FString AStaticResourceEntity::GetName_Implementation()
{
    return InteractiveComponent->Name;
}

EEntityType AStaticResourceEntity::GetType_Implementation()
{
    return InteractiveComponent->Type;
}

TArray<FInteractOption> AStaticResourceEntity::GetOptions_Implementation(bool bVisibleOnly)
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

float AStaticResourceEntity::GetInteractDistance_Implementation()
{
    return InteractiveComponent->InteractDistance;
}

UInteractiveComponent* AStaticResourceEntity::GetInteractiveComponent_Implementation()
{
    return InteractiveComponent;
}

UMeshComponent* AStaticResourceEntity::GetMeshComponent_Implementation()
{
    return StaticMeshComponent;
}

void AStaticResourceEntity::SetOptionVisibility_Implementation(const FString& OptionName, bool bVisibility = true)
{
    for (FInteractOption& Option : InteractiveComponent->Options)
    {
        if (Option.Name == OptionName)
        {
            Option.bVisible = bVisibility;
        }
    }
}