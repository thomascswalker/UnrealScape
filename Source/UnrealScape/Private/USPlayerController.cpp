// Fill out your copyright notice in the Description page of Project Settings.

#include "USPlayerController.h"

AUSPlayerController::AUSPlayerController()
{
    bShowMouseCursor = true;
    bAttachToPawn = true;

    Spline = CreateDefaultSubobject<USplineComponent>(TEXT("MovementSpline"));
    Spline->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    Spline->ClearSplinePoints();
    Spline->bDrawDebug = true;
}

void AUSPlayerController::Tick(float DeltaTime)
{
    if (Spline->GetNumberOfSplinePoints() == 0)
    {
        return;
    }

    // Get current player location
    APawn* ControlledPawn = GetPawn();
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
        CurrentTime += DistanceBetweenPoints;
        NextPoint = Spline->GetLocationAtTime(CurrentTime, ESplineCoordinateSpace::World);
    }

    // Move towards mouse pointer or touch
    if (ControlledPawn != nullptr)
    {
        FVector WorldDirection = (NextPoint - PlayerLocation).GetSafeNormal();
        ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
    }
}

void AUSPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction("LeftClick", IE_Pressed, this, &AUSPlayerController::Navigate);
}

void AUSPlayerController::Navigate()
{
    Spline->ClearSplinePoints();
    FHitResult HitResult;
    if (!LineTraceUnderMouseCursor(HitResult))
    {
        return;
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, TEXT("Clicked!"));
    }

    // Get the target tile
    AGrid* Grid = GetCurrentGrid();

    // Get the current tile
    FTileInfo CurrentTile = GetCurrentTile();
    FTileInfo TargetTile = GetTileUnderCursor();

    if (GEngine)
    {
        FString Message = FString::Printf(L"Current: %s\nTarget: %s", *CurrentTile.GridIndex.ToString(),
                                          *TargetTile.GridIndex.ToString());
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, Message);
    }

    // Request Path
    TArray<FTileInfo> Path = Grid->RequestPath(CurrentTile, TargetTile);
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
        Goal = Tile.WorldPosition;
    }
    Length = Spline->GetSplineLength();
    Spline->Duration = Length;
    CurrentTime = 0.f;
    NextPoint = Spline->GetLocationAtTime(CurrentTime, ESplineCoordinateSpace::World);
}

bool AUSPlayerController::LineTraceUnderMouseCursor(FHitResult& HitResult)
{
    FVector WorldLocation;
    FVector WorldDirection;
    bool Result = DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

    if (!Result)
    {
        return false;
    }

    FVector Start = WorldLocation;
    FVector End = (WorldDirection * 10000.f) + Start;

    TArray<AActor*> ActorsToIgnore;
    const bool BlockingHit = UKismetSystemLibrary::LineTraceSingle(
        this, Start, End, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore,
        EDrawDebugTrace::None, HitResult, true, FLinearColor::Red, FLinearColor::Green, 1.f);

    return BlockingHit;
}

AGrid* AUSPlayerController::GetCurrentGrid()
{
    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrid::StaticClass(), Actors);
    for (auto Actor : Actors)
    {
        AGrid* Grid = Cast<AGrid>(Actor);
        return Grid;
    }

    UE_LOG(LogTemp, Warning, TEXT("No grid found!"));
    return nullptr;
}

FTileInfo& AUSPlayerController::GetCurrentTile()
{
    // Get the current grid
    AGrid* Grid = GetCurrentGrid();

    // Get current player location
    APawn* ControlledPawn = GetPawn();
    FVector PlayerLocation = ControlledPawn->GetActorLocation();

    // Trace location
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);
    FHitResult HitResult;
    const bool BlockingHit = UKismetSystemLibrary::SphereTraceSingle(
        this, PlayerLocation, PlayerLocation + FVector(0.f, 0.f, 50.f), 25.f,
        UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true,
        FLinearColor::Red, FLinearColor::Green, 1.f);

    // If we hit any tiles, get the tile index
    ATile* Tile = Cast<ATile>(HitResult.GetActor());
    return Grid->GetTileInfoFromTileActor(Tile);
}

FTileInfo& AUSPlayerController::GetTileUnderCursor()
{
    // Get the current grid
    AGrid* Grid = GetCurrentGrid();

    // Trace under cursor
    FHitResult HitResult;
    LineTraceUnderMouseCursor(HitResult);

    // Do we hit a tile?
    AActor* Actor = HitResult.GetActor();
    ATile* Tile = Cast<ATile>(Actor);

    // Get the respective tile info
    return Grid->GetTileInfoFromTileActor(Tile);
}
