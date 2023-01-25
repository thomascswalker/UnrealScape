// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Algo/Reverse.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"


#include <vector>

#include "Public/TileInfo.h"

#include "Grid.generated.h"

UCLASS()
class RS_MOVEMENT_CPP_API AGrid : public AActor
{
    GENERATED_BODY()

    FVector WorldOrigin;
    FVector2d WorldSize;

    UClass* TileClass = ATile::StaticClass();

public:
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Grid")
    TArray<FTileInfo> Tiles;

    UPROPERTY(EditAnywhere, Category = "Grid")
    int SizeX = 5;

    UPROPERTY(EditAnywhere, Category = "Grid")
    int SizeY = 5;

    UPROPERTY(EditAnywhere, Category = "Grid")
    float TileSize = 50.0;

    // Sets default values for this actor's properties
    AGrid();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Grid")
    void ConstructTileActors(const FVector CenteredLocation);
};
