// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"

// Sets default values
AGrid::AGrid()
{
    PrimaryActorTick.bCanEverTick = true;

    WorldSize.X = SizeX * TileSize;
    WorldSize.Y = SizeY * TileSize;
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AGrid::OnConstruction(const FTransform& Transform)
{
    if (Tiles.Num() > 0)
    {
        for (ATile* Tile : Tiles)
        {
            if (IsValid(Tile))
            {
                Tile->K2_DestroyActor();
            }

            if (Tiles.Contains(Tile))
            {
                Tiles.Remove(Tile);
            }
        }
    }

    // Set this actor to call Tick() every frame.  You can turn this off to
    // improve performance if you don't need it.
    UClass* TileClass = ATile::StaticClass();
    UWorld* World = GetWorld();

    // Construct new tiles for the grid
    for (int X = 0; X < SizeX; X++)
    {
        for (int Y = 0; Y < SizeY; Y++)
        {
            FVector WorldPosition;
            WorldPosition.X = X * TileSize;
            WorldPosition.Y = Y * TileSize;

            FTransform SpawnTransform = FTransform(WorldPosition);

            ATile* Tile = World->SpawnActor<ATile>(TileClass, SpawnTransform);
            if (IsValid(Tile))
            {
                Tile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

                // 2D index of the tile within the grid
                Tile->GridPosition.X = X;
                Tile->GridPosition.Y = Y;

                // 3D position of the tile in the world
                Tile->WorldPosition.X = WorldPosition.X;
                Tile->WorldPosition.Y = WorldPosition.Y;

                Tiles.Add(Tile);
            }
        }
    }
}
