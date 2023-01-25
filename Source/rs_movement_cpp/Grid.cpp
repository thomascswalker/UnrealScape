// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"

// Sets default values
AGrid::AGrid(int _sizeX, int _sizeY) {
  // Set this actor to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  sizeX = _sizeX;
  sizeY = _sizeY;

  for (int x = 0; x < sizeX; x++) {
    for (int y = 0; y < sizeY; y++) {
      ATile tile;

      tile.gridPosition.X = x;
      tile.gridPosition.Y = y;

      tiles.Add(std::make_unique<ATile>(tile));
    }
  }
}

// Called when the game starts or when spawned
void AGrid::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void AGrid::Tick(float DeltaTime) { Super::Tick(DeltaTime); }
