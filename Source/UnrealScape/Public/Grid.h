// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "Algo/Reverse.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "TileInfo.h"
#include "USUtils.h"

#include "Grid.generated.h"

UCLASS()
class UNREALSCAPE_API AGrid : public AActor
{
    GENERATED_BODY()

    FVector WorldOrigin;
    FVector2d WorldSize;

    UClass* TileClass = ATile::StaticClass();

public:
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Navigation")
    TArray<FTileInfo> Tiles;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Navigation")
    int SizeX = 5;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Navigation")
    int SizeY = 5;

    UPROPERTY(EditAnywhere, Category = "Navigation")
    float TileSize = 50.0;

    // Sets default values for this actor's properties
    AGrid();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Navigation")
    void ConstructTileActors(const FVector CenteredLocation);

    bool IsGridIndexValid(FVector2D Index);
    bool IsGridIndexValid(int X, int Y);
    int GetTileIndexFromGridIndex(FVector2D Index);
    int GetTileIndexFromGridIndex(int X, int Y);
    TOptional<FTileInfo> GetTileInfoFromGridIndex(const FVector2D GridIndex);
    TOptional<FTileInfo> GetTileInfoFromLocation(const FVector Location);

    // Pathfinding functions
    UFUNCTION(BlueprintCallable, Category = "Navigation")
    bool IsWalkableLocation(const FVector& Location);
    bool IsWalkableTile(const FTileInfo& Tile);
    bool IsWalkableGridIndex(const FVector2D GridIndex);
    void GetNeighbors(const FTileInfo& Tile, TArray<FTileInfo>& Neighbors);
    int GetDistance(const FTileInfo& A, const FTileInfo& B);
    TArray<FTileInfo> Retrace(FTileInfo& Start, FTileInfo& End);

    UFUNCTION(BlueprintCallable, Category = "Navigation")
    TArray<FTileInfo> RequestPath(const FTileInfo& Start, const FTileInfo& End);
};