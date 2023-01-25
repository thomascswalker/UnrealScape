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

void AGrid::ConstructTileActors(const FVector CenteredLocation)
{
    if (Tiles.Num() > 0)
    {
        return;
    }

    for (int Y = 0; Y < SizeY; Y++)
    {
        for (int X = 0; X < SizeX; X++)
        {
            FVector SpawnLocation = FVector(X * TileSize, Y * TileSize, 0.f);

            FTransform SpawnTransform;
            SpawnTransform.SetLocation(SpawnLocation);

            // Construct the tile
            //AActor* Actor = UGameplayStatics::BeginDeferredActorSpawnFromClass(this, TileClass, SpawnTransform);
            //ATile* Tile = Cast<ATile>(Actor);
            FTileInfo TileInfo;

            // If it's constructed successfully, set the grid and world positions
            //if (Tile)
            //{
            //TileInfo.Actor = Tile;

            TileInfo.GridPosition.X = X;
            TileInfo.GridPosition.Y = Y;

            TileInfo.WorldPosition.X = SpawnTransform.GetLocation().X;
            TileInfo.WorldPosition.Y = SpawnTransform.GetLocation().Y;
            //}

            // Add to Tiles array and attach to this
            //Tile->FinishSpawning(SpawnTransform);
            Tiles.Add(TileInfo);
            //Tile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
        }
    }
}
