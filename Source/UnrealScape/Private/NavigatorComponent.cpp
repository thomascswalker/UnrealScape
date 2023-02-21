// Fill out your copyright notice in the Description page of Project Settings.

#include "NavigatorComponent.h"
#include "Engine/Public/VisualLogger/VisualLogger.h"

DEFINE_LOG_CATEGORY(LogNavigation);

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
}

// Called when the game starts
void UNavigatorComponent::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void UNavigatorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Get current player location
    APawn* ControlledPawn = Cast<APawn>(GetOwner());
    bInsideBuilding = GetFloorAbove() != EFloorLevel::Z0;

    // If there's zero points, we can just return
    if (Spline->GetNumberOfSplinePoints() == 0)
    {
        return;
    }
    // Otherwise we'll move the actor whatever the current direction along the path spline
    MoveActor();
}

FVector UNavigatorComponent::GetActorFeet()
{
    APawn* Pawn = Cast<APawn>(GetOwner());
    FVector Location = Pawn->GetActorLocation();
    FVector TraceOffset(0, 0, -200);
    TArray<AActor*> ActorsToIgnore;
    FHitResult HitResult;
    const bool BlockingHit = UKismetSystemLibrary::LineTraceSingle(
        this, Location, Location + TraceOffset, UEngineTypes::ConvertToTraceType(ECC_Terrain), false, ActorsToIgnore,
        EDrawDebugTrace::None, HitResult, true, FLinearColor::Red, FLinearColor::Green, 1.f);
    return HitResult.Location;
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

void UNavigatorComponent::Navigate(const FNavigationRequest& Request)
{
    APawn* ControlledPawn = Cast<APawn>(GetOwner());

    if (bIsMoving)
    {
        Stopped.Broadcast();
        bIsMoving = false;
    }

    Spline->ClearSplinePoints();

    UpdateCurrentGrid();
    UpdateCurrentTile();

    // Request Path
    if (Request.Path.Num() == 0)
    {
        return;
    }

    if (!ControlledPawn)
    {
#ifdef UE_BUILD_DEBUG
        WARNING(L"Invalid pawn owner of this component.");
#endif
        return;
    }

    // Convert path points to spline points
    FVector PawnLocation = CurrentTile.WorldPosition;
    Spline->AddSplineWorldPoint(PawnLocation);
    int Count = Spline->GetNumberOfSplinePoints();
    Spline->SetSplinePointType(Count - 1, ESplinePointType::Linear);
    for (const FTileInfo& Tile : Request.Path)
    {
        Spline->AddSplineWorldPoint(Tile.WorldPosition);
        Count = Spline->GetNumberOfSplinePoints();
        Spline->SetSplinePointType(Count - 1, ESplinePointType::Linear);
        Goal = Tile.WorldPosition;
    }
    Length = Spline->GetSplineLength();
    Spline->Duration = Length;
    CurrentTime = 0.f;
    NextPoint = Spline->GetLocationAtTime(CurrentTime, ESplineCoordinateSpace::World);

    bIsMoving = true;
}

bool UNavigatorComponent::CanMoveToLocation(FNavigationRequest& Request)
{
    APawn* Pawn = Cast<APawn>(GetOwner());
    if (!Pawn)
    {
        return false;
    }

    UpdateCurrentGrid();
    if (!CurrentGrid->IsWorldPositionValid(Request.End))
    {
        return false;
    }

#if ENABLE_VISUAL_LOG
    UE_VLOG_LOCATION(this, LogNavigation, Verbose, Request.End, 25.f, FColor::Yellow, TEXT("Target Location"));
#endif

    TOptional<FTileInfo> PossibleTargetTile = CurrentGrid->GetTileInfoFromLocation(Request.End);
    if (!PossibleTargetTile.IsSet())
    {
#ifdef UE_BUILD_DEBUG
        FATAL(FString::Printf(L"Tile at location %s not found", *Request.End.ToString()));
#endif
        return false;
    }

    FTileInfo TargetTile = PossibleTargetTile.GetValue();
#if ENABLE_VISUAL_LOG
    UE_VLOG_LOCATION(this, LogNavigation, Verbose, TargetTile.WorldPosition, 50.f, FColor::Red,
                     TEXT("Original Target Tile"));
#endif

    if (!CurrentGrid->IsWalkableLocation(Request.End))
    {
        TArray<FTileInfo> Neighbors;

        CurrentGrid->GetNeighbors(TargetTile, Neighbors, false);
        if (Neighbors.Num() == 0)
        {
#ifdef UE_BUILD_DEBUG
            WARNING(FString::Printf(L"Location %s has no valid neighbors", *Request.End.ToString()));
#endif
            return false;
        }

        FTileInfo ClosestNeighbor;
        float ClosestDistance = 1000000.f;

        for (auto& Neighbor : Neighbors)
        {
#if ENABLE_VISUAL_LOG
            UE_VLOG_LOCATION(this, LogNavigation, Verbose, Neighbor.WorldPosition, 50.f, FColor::Cyan,
                             TEXT("Possible Target"));
#endif

            float Distance = (GetActorFeet() - Neighbor.WorldPosition).Size2D();
            if (Distance < ClosestDistance)
            {
                ClosestDistance = Distance;
                ClosestNeighbor = Neighbor;
            }
        }

#if ENABLE_VISUAL_LOG
        UE_VLOG_LOCATION(this, LogNavigation, Verbose, ClosestNeighbor.WorldPosition, 50.f, FColor::Green,
                         TEXT("Final Target"));
#endif
        TargetTile = ClosestNeighbor;
    }

    if (bIsMoving)
    {
        Stopped.Broadcast();
    }

    Spline->ClearSplinePoints();

    UpdateCurrentGrid();
    UpdateCurrentTile();

    // Request Path
    TArray<FTileInfo> Path = CurrentGrid->RequestPath(CurrentTile, TargetTile);
    if (Path.Num() == 0)
    {
#ifdef UE_BUILD_DEBUG
        WARNING(FString::Printf(L"No path found from %s to %s", *CurrentTile.WorldPosition.ToString(),
                                *TargetTile.WorldPosition.ToString()));
#endif
        return false;
    }

    Request.Path = Path;
    return true;
}

void UNavigatorComponent::MoveActor()
{
    // Get current player location
    APawn* ControlledPawn = Cast<APawn>(GetOwner());
    if (!ControlledPawn)
    {
        return;
    }

    const FVector PlayerLocation = GetActorFeet();

    // Get distance to end
    float DistanceToGoal = (Goal - PlayerLocation).Size2D();

    // If we're within the goal threshold, clear all points
    if (DistanceToGoal <= GoalThreshold)
    {
        bIsMoving = false;
        Spline->ClearSplinePoints();
        ReachedDestination.Broadcast();
        return;
    }

    // Get the distance to the next point
    const float DistanceToNextPoint = (NextPoint - PlayerLocation).Size2D();

    // If we're within the threshold to the next point, we'll update bump to the next point,
    // update the current tile, and rotate the pawn
    if (DistanceToNextPoint <= PointThreshold)
    {
        CurrentTime += CurrentGrid->TileSize;
        NextPoint = Spline->GetLocationAtTime(CurrentTime, ESplineCoordinateSpace::World);
        UpdateCurrentTile();

        UE_VLOG(this, LogNavigation, Verbose, TEXT("Updated next point"));
    }

    FVector WorldDirection = (NextPoint - PlayerLocation).GetSafeNormal();

#if ENABLE_VISUAL_LOG
    UE_VLOG_LOCATION(this, LogNavigation, Verbose, PlayerLocation, 50.f, FColor::Red, TEXT("Current location"));
    UE_VLOG_LOCATION(this, LogNavigation, Verbose, NextPoint, 50.f, FColor::Green, TEXT("Next point"));
    UE_VLOG_ARROW(this, LogNavigation, Verbose, PlayerLocation,
                  PlayerLocation + (WorldDirection * CurrentGrid->TileSize), FColor::Red, TEXT("Movement vector"));
#endif

    UPawnMovementComponent* MovementComponent = Cast<UPawnMovementComponent>(ControlledPawn->GetMovementComponent());
    MovementComponent->RequestDirectMove(WorldDirection * MovementSpeed * 250.f, false);
}

EFloorLevel UNavigatorComponent::GetFloorAbove()
{
    APawn* ControlledPawn = Cast<APawn>(GetOwner());
    TArray<AActor*> ActorsToIgnore;
    FHitResult HitResult;
    const bool BlockingHit = UKismetSystemLibrary::LineTraceSingle(
        GetWorld(), ControlledPawn->GetActorLocation(), ControlledPawn->GetActorLocation() + FVector(0, 0, 200),
        UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true,
        FColor::Red, FColor::Green);
    if (!BlockingHit)
    {
        return EFloorLevel::Z0;
    }
    if (HitResult.GetActor()->ActorHasTag("B0"))
    {
        return EFloorLevel::B0;
    }
    else if (HitResult.GetActor()->ActorHasTag("Z0"))
    {
        return EFloorLevel::Z0;
    }
    else if (HitResult.GetActor()->ActorHasTag("Z1"))
    {
        return EFloorLevel::Z1;
    }
    else if (HitResult.GetActor()->ActorHasTag("Z2"))
    {
        return EFloorLevel::Z2;
    }

    return EFloorLevel::Z0;
}

#if ENABLE_VISUAL_LOG
void UNavigatorComponent::GrabDebugSnapshot(FVisualLogEntry* Snapshot) const
{
    IVisualLoggerDebugSnapshotInterface::GrabDebugSnapshot(Snapshot);
    const int32 CatIndex = Snapshot->Status.AddZeroed();
    FVisualLogStatusCategory& Category = Snapshot->Status[CatIndex];
    Category.Category = TEXT("Navigation");
    Category.Add(TEXT("Current Location"), *GetOwner()->GetActorLocation().ToString());
    Category.Add(TEXT("Next Location"), *NextPoint.ToString());
    Category.Add(TEXT("Goal Location"), *Goal.ToString());
}
#endif