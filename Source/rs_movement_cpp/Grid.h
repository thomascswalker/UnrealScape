// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <memory>
#include <vector>

#include "Tile.h"

#include "Grid.generated.h"

UCLASS()
class RS_MOVEMENT_CPP_API AGrid : public AActor {
  GENERATED_BODY()

  TArray<std::unique_ptr<ATile>> tiles;

public:
  UPROPERTY(EditAnywhere, Category = "Grid Properties")
  int sizeX = 0;

  UPROPERTY(EditAnywhere, Category = "Grid Properties")
  int sizeY = 0;

  // Sets default values for this actor's properties
  AGrid() = default;
  AGrid(int _sizeX, int _sizeY);

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;
};
