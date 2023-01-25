// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Math/Vector.h"
#include "Tile.generated.h"

UCLASS()
class RS_MOVEMENT_CPP_API ATile : public AActor
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile Properties")
    FVector2D GridPosition;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile Properties")
    FVector WorldPosition;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile Properties")
    int G = 0;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile Properties")
    int H = 0;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile Properties")
    bool Walkable = false;

    // Sets default values for this actor's properties
    ATile();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    int GetFCost();
};
