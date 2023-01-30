// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Grid.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SplineComponent.h"
#include "CoreMinimal.h"
#include "Kismet/KismetMathLibrary.h"

#include "NavigatorComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALSCAPE_API UNavigatorComponent : public UActorComponent
{
    GENERATED_BODY()

    FVector Goal;
    FVector NextPoint;
    float Length;
    float CurrentTime = 0.f;

public:
    // Sets default values for this component's properties
    UNavigatorComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Navigation")
    float MovementSpeed = 2.f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Navigation")
    USplineComponent* Spline;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Navigation")
    float GoalThreshold = 10.f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Navigation")
    float DistanceThreshold = 50.f;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Navigation")
    float DistanceBetweenPoints = 50.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Navigation")
    AGrid* CurrentGrid;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Navigation")
    FTileInfo CurrentTile;

    UFUNCTION(BlueprintCallable, Category = "Navigation")
    void UpdateCurrentGrid();

    UFUNCTION(BlueprintCallable, Category = "Navigation")
    void UpdateCurrentTile();

    UFUNCTION(BlueprintCallable, Category = "Navigation")
    void Navigate(const FTileInfo& TargetTile);

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;
};
