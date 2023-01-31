// Fill out your copyright notice in the Description page of Project Settings.

#include "NavigatorComponent.h"

// Sets default values for this component's properties
// https://snorristurluson.github.io/ClickToMove/
// https://github.com/snorristurluson/BasicClickToMove/blob/master/Source/BasicClickToMove/NavigatorComponent.cpp
UNavigatorComponent::UNavigatorComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // Create spline
    Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
    Spline->ClearSplinePoints();
    Spline->bDrawDebug = true;
}

// Called when the game starts
void UNavigatorComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UNavigatorComponent::UpdateCurrentGrid()
{
    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrid::StaticClass(), Actors);
    for (auto Actor : Actors)
    {
        CurrentGrid = Cast<AGrid>(Actor);
        return;
    }
}

void UNavigatorComponent::UpdateCurrentTile()
{
    // Get current player location
    APawn* ControlledPawn = Cast<APawn>(GetOwner());
    FVector PawnLocation = ControlledPawn->GetActorLocation();
    PawnLocation.Z = 0.f;

    TOptional<FTileInfo> PossibleTile = CurrentGrid->GetTileInfoFromLocation(PawnLocation);
    if (PossibleTile.IsSet())
    {
        CurrentTile = PossibleTile.GetValue();
    }
}

void UNavigatorComponent::NavigateToTile(const FTileInfo& TargetTile)
{
    Spline->ClearSplinePoints();

    UpdateCurrentGrid();
    UpdateCurrentTile();

    // Request Path
    TArray<FTileInfo> Path = CurrentGrid->RequestPath(CurrentTile, TargetTile);
    if (Path.Num() == 0)
    {
        return;
    }

    // Convert path points to spline points
    for (FTileInfo& Tile : Path)
    {
        Spline->AddSplineWorldPoint(Tile.WorldPosition);
        int Count = Spline->GetNumberOfSplinePoints();
        Spline->SetSplinePointType(Count - 1, ESplinePointType::Linear);
        Goal = Tile.WorldPosition;
    }
    Length = Spline->GetSplineLength();
    Spline->Duration = Length;
    CurrentTime = 0.f;
    NextPoint = Spline->GetLocationAtTime(CurrentTime, ESplineCoordinateSpace::World);

    // Initial rotation
    APawn* ControlledPawn = Cast<APawn>(GetOwner());
    if (!ControlledPawn)
    {
        return;
    }
    const FVector PlayerLocation = ControlledPawn->GetActorLocation();
    FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, NextPoint);
    Rotation.Pitch = 0.0;
    Rotation.Roll = 0.0;
    ControlledPawn->SetActorRotation(Rotation);
}

// Called every frame
void UNavigatorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (Spline->GetNumberOfSplinePoints() == 0)
    {
        return;
    }

    // Get current player location
    APawn* ControlledPawn = Cast<APawn>(GetOwner());
    if (!ControlledPawn)
    {
        return;
    }
    const FVector PlayerLocation = ControlledPawn->GetActorLocation();

    // Get distance to end
    float DistanceToGoal = (Goal - PlayerLocation).Size2D();

    // If we're within the goal threshold, clear all points
    if (DistanceToGoal <= GoalThreshold)
    {
        Spline->ClearSplinePoints();
        return;
    }

    const float DistanceToNextPoint = (NextPoint - PlayerLocation).Size2D();
    if (DistanceToNextPoint <= DistanceThreshold)
    {
        CurrentTime += DistanceToNextPoint;
        NextPoint = Spline->GetLocationAtTime(CurrentTime, ESplineCoordinateSpace::World);
        UpdateCurrentTile();

        // Rotate
        FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, NextPoint);
        Rotation.Pitch = 0.0;
        Rotation.Roll = 0.0;
        ControlledPawn->SetActorRotation(Rotation);
    }

    if (ControlledPawn != nullptr)
    {
        FVector WorldDirection = (NextPoint - PlayerLocation).GetSafeNormal();

        // Move
        ControlledPawn->AddMovementInput(WorldDirection, MovementSpeed, true);
    }
}

void UNavigatorComponent::NavigateToLocation(const FVector Location)
{
    INFO(FString::Printf(L"Navigating to %s", *Location.ToString()));
    UpdateCurrentGrid();
    if (!CurrentGrid->IsWalkableLocation(Location))
    {
        WARNING(FString::Printf(L"Location %s not walkable", *Location.ToString()));
        return;
    }
    TOptional<FTileInfo> TargetTile = CurrentGrid->GetTileInfoFromLocation(Location);
    if (!TargetTile.IsSet())
    {
        FATAL(FString::Printf(L"Tile at location %s not found", *Location.ToString()));
    }
    else
    {
        NavigateToTile(TargetTile.GetValue());
    }
}