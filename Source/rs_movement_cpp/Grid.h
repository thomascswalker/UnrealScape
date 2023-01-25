// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include <vector>

#include "Tile.h"

#include "Grid.generated.h"

UCLASS()
class RS_MOVEMENT_CPP_API AGrid : public AActor
{
    GENERATED_BODY()

    TArray<ATile*> Tiles;

    FVector WorldOrigin;
    FVector2d WorldSize;

public:
    UPROPERTY(EditAnywhere, Category = "Grid Properties")
    int SizeX = 0;

    UPROPERTY(EditAnywhere, Category = "Grid Properties")
    int SizeY = 0;

    UPROPERTY(EditAnywhere, Category = "Grid Properties")
    float TileSize = 50.0;

    // Sets default values for this actor's properties
    AGrid() = default;
    AGrid(int _SizeX, int _SizeY);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
