// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "..\Tile.h"
#include "CoreMinimal.h"
#include "Math/Vector.h"
#include "TileInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct RS_MOVEMENT_CPP_API FTileInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "TileInfo")
    FVector2D Parent;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "TileInfo")
    ATile* Actor;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "TileInfo")
    FVector2D GridPosition;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "TileInfo")
    FVector WorldPosition;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "TileInfo")
    int G = 0;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "TileInfo")
    int H = 0;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "TileInfo")
    bool Walkable = false;

    int GetF() { return G + H; }

	FORCEINLINE bool operator==(const FTileInfo& Other) const { return WorldPosition == Other.WorldPosition; }
};
