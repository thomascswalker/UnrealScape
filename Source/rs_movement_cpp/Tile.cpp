// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"

ATile::ATile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to
    // improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Construct the static mesh component
    UStaticMeshComponent* StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
    auto Mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
    if (Mesh.Object)
    {
        StaticMeshComponent->SetStaticMesh(Mesh.Object);
        StaticMeshComponent->SetRelativeScale3D(FVector(.5f, .5f, .1));
    }

    // Set this component as the root component
    RootComponent = StaticMeshComponent;
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
