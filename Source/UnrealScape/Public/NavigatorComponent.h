// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SplineComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Grid.h"
#include "Kismet/KismetMathLibrary.h"
#include "VisualLogger/VisualLoggerDebugSnapshotInterface.h"

#include "NavigatorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMovingSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReachedDestinationSignature, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStoppedSignature);
DECLARE_LOG_CATEGORY_EXTERN(Navigation, Log, All);

UENUM(BlueprintType)
enum class EFloorLevel : uint8
{
    B2 UMETA(DisplayName = "B2"),
    B1 UMETA(DisplayName = "B1"),
    B0 UMETA(DisplayName = "B0"),
    Z0 UMETA(DisplayName = "Z0"),
    Z1 UMETA(DisplayName = "Z1"),
    Z2 UMETA(DisplayName = "Z2"),
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALSCAPE_API UNavigatorComponent : public UActorComponent, public IVisualLoggerDebugSnapshotInterface
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
    FVector TraceOffset = FVector(0, 0, -200);
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Navigation")
    AGrid* CurrentGrid;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Navigation")
    FTileInfo CurrentTile;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Navigation")
    bool bIsMoving = false;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Navigation")
    bool bInsideBuilding = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Navigation")
    bool bDebugDraw = false;

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

    UFUNCTION(BlueprintCallable, Category = "Navigation")
    EFloorLevel GetCurrentFloor();

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;

#if ENABLE_VISUAL_LOG
    virtual void GrabDebugSnapshot(FVisualLogEntry* Snapshot) const override;
#endif
};
