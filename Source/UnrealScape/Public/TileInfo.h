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
struct FTileInfo
{
public:
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "TileInfo")
    FVector2D Parent;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "TileInfo")
    ATile* Actor;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "TileInfo")
    FVector2D GridIndex;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "TileInfo")
    FVector WorldPosition;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "TileInfo")
    int G = 0;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "TileInfo")
    int H = 0;


    int GetF() const { return G + H; }
    bool Equals(const FTileInfo& Other) const { return GridIndex == Other.GridIndex; }

    bool operator==(const FTileInfo& Other) const { return Equals(Other); }
    bool operator!=(const FTileInfo& Other) const { return !Equals(Other); }
};
