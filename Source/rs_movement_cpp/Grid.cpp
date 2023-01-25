// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"

// Sets default values
AGrid::AGrid(int _SizeX, int _SizeY)
{
    // Set this actor to call Tick() every frame.  You can turn this off to
    // improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SizeX = _SizeX;
    SizeY = _SizeY;

    WorldSize.X = SizeX * TileSize;
    WorldSize.Y = SizeY * TileSize;

    // Construct new tiles for the grid
    for (int X = 0; X < SizeX; X++)
    {
        for (int Y = 0; Y < SizeY; Y++)
        {
            ATile* Tile = NewObject<ATile>(this);

            Tile->GridPosition.X = X;
            Tile->GridPosition.Y = Y;

            Tiles.Add(Tile);
        }
    }
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
