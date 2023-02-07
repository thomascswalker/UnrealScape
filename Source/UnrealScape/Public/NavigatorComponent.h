// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SplineComponent.h"
#include "CoreMinimal.h"
#include "Grid.h"
#include "Kismet/KismetMathLibrary.h"

#include "NavigatorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMovingSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReachedDestinationSignature, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStoppedSignature);

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
    UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers")
    FMovingSignature Moving;

    UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers")
    FReachedDestinationSignature ReachedDestination;
    UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers")
    FStoppedSignature Stopped;

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
    FVector TraceOffset = FVector(0,0,-200);

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Navigation")
    AGrid* CurrentGrid;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Navigation")
    FTileInfo CurrentTile;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Navigation")
    bool bIsMoving = false;

    UFUNCTION(BlueprintCallable, Category = "Navigation")
    void UpdateCurrentGrid();

    UFUNCTION(BlueprintCallable, Category = "Navigation")
    void UpdateCurrentTile();

    UFUNCTION(BlueprintCallable, Category = "Navigation")
    void NavigateToTile(const FTileInfo& TargetTile);

    UFUNCTION(BlueprintCallable, Category = "Navigation")
    void NavigateToLocation(const FVector Location);

    UFUNCTION(BlueprintCallable, Category = "Navigation")
    FVector GetMovementVector();

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;
};
