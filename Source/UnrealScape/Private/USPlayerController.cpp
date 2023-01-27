// Fill out your copyright notice in the Description page of Project Settings.

#include "USPlayerController.h"

AUSPlayerController::AUSPlayerController()
{
    bShowMouseCursor = true;
    bAttachToPawn = true;
}

void AUSPlayerController::Tick(float DeltaTime)
{
    //FHitResult HitResult;
    //if (LineTraceUnderMouseCursor(HitResult))
    //{
    //    AActor* Actor = HitResult.GetActor();
    //    ATile* Tile = Cast<ATile>(Actor);
    //    if (!Tile)
    //    {
    //        TargetTile = nullptr;
    //    }
    //    else if (Tile != TargetTile)
    //    {
    //        TargetTile = Tile;

    //        if (GEngine)
    //        {
    //            FString Message = FString::Printf(L"%s", *Tile->GetName());
    //            GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, Message);
    //        }
    //    }
    //}
}

void AUSPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindAction("LeftClick", IE_Pressed, this, &AUSPlayerController::OnLeftClick);
}

void AUSPlayerController::OnLeftClick()
{
    FHitResult HitResult;
    if (LineTraceUnderMouseCursor(HitResult))
    {
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

        FVector PlayerLocation = K2_GetActorLocation();
        for (FTileInfo& NextTile : Path)
        {
            //int Count = 0;
            //while (CurrentTile != NextTile)
            //{
            //    FVector Direction = NextTile.WorldPosition - CurrentTile.WorldPosition;
            //    Direction.Normalize();
            //    GetPawn()->AddMovementInput(Direction, 1.f, false);
            //    if (Count > 10)
            //    {
            //        break;
            //    }
            //}
            //CurrentTile = NextTile;
        }
    }
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
        FVector Origin;
        FVector Extent;
        Actor->GetActorBounds(false, Origin, Extent);
        AGrid* Grid = Cast<AGrid>(Actor);
        return Grid;
    }

    UE_LOG(LogTemp, Warning, TEXT("No grid found!"));
    return nullptr;
}

FTileInfo& AUSPlayerController::GetCurrentTile()
{
    FVector PlayerLocation = K2_GetActorLocation();
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);
    FHitResult HitResult;
    const bool BlockingHit = UKismetSystemLibrary::SphereTraceSingle(
        this, PlayerLocation, PlayerLocation + FVector(0.f, 0.f, 50.f), 25.f,
        UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true,
        FLinearColor::Red, FLinearColor::Green, 1.f);

    ATile* Tile = Cast<ATile>(HitResult.GetActor());
    AGrid* Grid = GetCurrentGrid();
    FTileInfo CurrentTile = Grid->GetTileInfoFromTileActor(Tile);
    int Index = Grid->GetTileIndexFromGridIndex(CurrentTile.GridIndex);
    return Grid->Tiles[Index];
}

FTileInfo& AUSPlayerController::GetTileUnderCursor()
{
    AGrid* Grid = GetCurrentGrid();
    FHitResult HitResult;
    LineTraceUnderMouseCursor(HitResult);
    AActor* Actor = HitResult.GetActor();
    ATile* Tile = Cast<ATile>(Actor);
    FTileInfo TargetTile = Grid->GetTileInfoFromTileActor(Tile);
    int Index = Grid->GetTileIndexFromGridIndex(TargetTile.GridIndex);
    return Grid->Tiles[Index];
}
