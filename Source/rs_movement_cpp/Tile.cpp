// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"

ATile::ATile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to
    // improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    UBoxComponent* BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    BoxComponent->SetVisibility(true);
    RootComponent = BoxComponent;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

inline int ATile::GetFCost()
{
    return G + H;
}
