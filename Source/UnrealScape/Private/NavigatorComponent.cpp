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

    // ...
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

    // Trace location
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(ControlledPawn);
    FHitResult HitResult;
    const bool BlockingHit = UKismetSystemLibrary::SphereTraceSingle(
        this, PawnLocation, PawnLocation, 25.f, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore,
        EDrawDebugTrace::None, HitResult, true, FLinearColor::Red, FLinearColor::Green, 5.f);

    // If we hit any tiles, get the tile index
    ATile* Tile = Cast<ATile>(HitResult.GetActor());
    if (Tile)
    {
        CurrentTile = CurrentGrid->GetTileInfoFromTileActor(Tile);
    }
}

void UNavigatorComponent::Navigate(const FTileInfo& TargetTile)
{
    Spline->ClearSplinePoints();

    UpdateCurrentGrid();
    UpdateCurrentTile();

    if (GEngine)
    {
        FString Message = FString::Printf(L"Target: %s", *TargetTile.GridIndex.ToString());
        GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Yellow, Message);
    }

    // Request Path
    TArray<FTileInfo> Path = CurrentGrid->RequestPath(CurrentTile, TargetTile);
    if (Path.Num() == 0)
    {
        if (GEngine)
        {
            FString Message = FString::Printf(L"Path not found.");
            GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, Message);
        }
        return;
    }

    // Convert path points to spline points
    for (FTileInfo& Tile : Path)
    {
        Spline->AddSplineWorldPoint(Tile.WorldPosition);
        if (GEngine)
        {
            int Count = Spline->GetNumberOfSplinePoints();
            FString Message = FString::Printf(L"Spline Points: %i", Count);
            GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, Message);
        }

        DrawDebugSphere(GetWorld(), Tile.WorldPosition, 10.f, 12, FColor::Cyan, true);
        Goal = Tile.WorldPosition;
    }
    Length = Spline->GetSplineLength();
    Spline->Duration = Length;
    CurrentTime = 0.f;
    NextPoint = Spline->GetLocationAtTime(CurrentTime, ESplineCoordinateSpace::World);
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
    }

    if (ControlledPawn != nullptr)
    {
        if (GEngine)
        {
            FString Message = FString::Printf(L"Current Point: %f", CurrentTime);
            GEngine->AddOnScreenDebugMessage(32, 1.f, FColor::Yellow, Message);
        }
        FVector WorldDirection = (NextPoint - PlayerLocation).GetSafeNormal();

        // Move
        ControlledPawn->AddMovementInput(WorldDirection, 1.0, true);

        // Rotate
        // https://stackoverflow.com/questions/58719951/how-can-i-create-the-equivalent-of-unity-lookat-in-unreal-blueprint
        FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, NextPoint);
        Rotation.Pitch = 0.0;
        Rotation.Roll = 0.0;
        ControlledPawn->SetActorRotation(Rotation);
    }
}
