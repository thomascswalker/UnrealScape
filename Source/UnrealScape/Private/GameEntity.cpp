// Fill out your copyright notice in the Description page of Project Settings.

#include "GameEntity.h"

// Sets default values
AGameEntity::AGameEntity()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGameEntity::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AGameEntity::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AGameEntity::Interact_Implementation(const FOption& Option) {}

FVector AGameEntity::GetFloor_Implementation()
{
    FVector Origin;
    FVector Extent;
    GetActorBounds(true, Origin, Extent);
    return Origin;
}

APlayerController* AGameEntity::GetPlayer_Implementation()
{
    UWorld* World = GetWorld();
    APlayerController* Controller = UGameplayStatics::GetPlayerController(World, 0);
    return Controller;
}

FString AGameEntity::GetName_Implementation()
{
    return Name;
}

EEntityType AGameEntity::GetType_Implementation()
{
    return Type;
}

TArray<FOption> AGameEntity::GetOptions_Implementation()
{
    return Options;
}

void AGameEntity::SetOptionName(const FString& OldName, const FString& NewName)
{
    for (FOption& Option : Options)
    {
        if (Option.Name == OldName)
        {
            Option.Name = NewName;
        }
    }
}
