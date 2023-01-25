// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Math/Vector.h"
#include "Tile.generated.h"

UCLASS()
class RS_MOVEMENT_CPP_API ATile : public AActor {
  GENERATED_BODY()

public:
  UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile Properties")
  FVector2D gridPosition;

  UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile Properties")
  FVector worldPosition;

  UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile Properties")
  int g = 0;

  UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile Properties")
  int h = 0;

  UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Tile Properties")
  bool walkable = false;

  // Sets default values for this actor's properties
  ATile();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  int getFCost();
};
