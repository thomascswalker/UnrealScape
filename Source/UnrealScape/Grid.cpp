// Fill out your copyright notice in the Description page of Project Settings.

#include "Grid.h"

// Sets default values
AGrid::AGrid()
{
    PrimaryActorTick.bCanEverTick = true;
    WorldSize.X = SizeX * TileSize;
    WorldSize.Y = SizeY * TileSize;
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
    Super::BeginPlay();
}

void AGrid::OnConstruction(const FTransform& Transform)
{
    WorldSize.X = SizeX * TileSize;
    WorldSize.Y = SizeY * TileSize;
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AGrid::ConstructTileActors(const FVector CenteredLocation)
{
    if (Tiles.Num() > 0)
    {
        return;
    }

    FVector Offset = GetActorLocation();

    for (int Y = 0; Y < SizeY; Y++)
    {
        for (int X = 0; X < SizeX; X++)
        {
            float CX = (X * TileSize);  // - XOffset;
            float CY = (Y * TileSize);  // - YOffset;
            FVector SpawnLocation = FVector(CX, CY, 0.f);

            FTransform SpawnTransform;
            SpawnTransform.SetLocation(SpawnLocation);

            FTileInfo TileInfo;
            TileInfo.GridIndex.X = X;
            TileInfo.GridIndex.Y = Y;

            TileInfo.WorldPosition.X = SpawnLocation.X + (TileSize / 2.f) + Offset.X;
            TileInfo.WorldPosition.Y = SpawnLocation.Y + (TileSize / 2.f) + Offset.Y;

            Tiles.Add(TileInfo);
        }
    }
}

bool AGrid::IsGridIndexValid(FVector2D Index)
{
    return Index.X >= 0 && Index.Y >= 0 && Index.X < SizeX && Index.Y < SizeY;
}

bool AGrid::IsGridIndexValid(int X, int Y)
{
    return X >= 0 && Y >= 0 && X < SizeX && Y < SizeY;
}

int AGrid::GetTileIndexFromGridIndex(FVector2D GridIndex)
{
    int Index = (GridIndex.Y * SizeX) + GridIndex.X;
    if (Index > Tiles.Num() - 1)
    {
        WARNING(FString::Printf(L"Index %i not found", Index));
        return -1;
    }
    return Index;
}

int AGrid::GetTileIndexFromGridIndex(int X, int Y)
{
    return GetTileIndexFromGridIndex(FVector2D(X, Y));
}

TOptional<FTileInfo> AGrid::GetTileInfoFromGridIndex(FVector2D GridIndex)
{
    if (!IsGridIndexValid(GridIndex))
    {
        return FTileInfo();
    }
    int Index = GetTileIndexFromGridIndex(GridIndex);
    if (Index < 0 || Index > Tiles.Num() - 1)
    {
        WARNING(FString::Printf(L"Index %i not found", Index));
        return TOptional<FTileInfo>();
    }
    return Tiles[Index];
}

TOptional<FTileInfo> AGrid::GetTileInfoFromLocation(const FVector Location)
{
    // Get percentage of the full world size in X/Y
    FVector Offset = GetActorLocation();
    float PX = (Location.X - Offset.X + (TileSize / 2.f)) / (SizeX * TileSize);
    float PY = (Location.Y - Offset.Y + (TileSize / 2.f)) / (SizeY * TileSize);

    PX = std::clamp(PX, 0.f, 1.f);
    PY = std::clamp(PY, 0.f, 1.f);

    // Size * Percent = Tile Index
    float X = (SizeX - 1) * PX;
    float Y = (SizeX - 1) * PY;

    // Return actual tile
    int Index = GetTileIndexFromGridIndex(X, Y);
    if (Index < 0 || Index > Tiles.Num() - 1)
    {
        WARNING(FString::Printf(L"Index %i not found", Index));
        return TOptional<FTileInfo>();
    }
    return Tiles[Index];
}

inline bool AGrid::IsWalkableLocation(const FVector& Location)
{
    FVector Start = Location;
    FVector End = Start + FVector(0, 0, 25);

    TArray<AActor*> ActorsToIgnore;
    FHitResult HitResult;
    const bool BlockingHit = UKismetSystemLibrary::SphereTraceSingle(
        this, Start, End, 25.f, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, ActorsToIgnore,
        EDrawDebugTrace::None, HitResult, true, FLinearColor::Red, FLinearColor::Green, 2.f);

    return !BlockingHit;
}

bool AGrid::IsWalkableTile(const FTileInfo& Tile)
{
    return IsWalkableLocation(Tile.WorldPosition);
}

bool AGrid::IsWalkableGridIndex(const FVector2D GridIndex)
{
    TOptional<FTileInfo> TileInfo = GetTileInfoFromGridIndex(GridIndex);
    if (!TileInfo.IsSet())
    {
        return false;
    }
    return IsWalkableLocation(TileInfo.GetValue().WorldPosition);
}

void AGrid::GetNeighbors(const FTileInfo& Tile, TArray<FTileInfo>& Neighbors)
{
    /*
       -1  0  1
    -1 [X][X][X]
     0 [X][O][X]
     1 [X][X][X]
    */

    // Current tile coordinates
    int CX = Tile.GridIndex.X;
    int CY = Tile.GridIndex.Y;

    // Neighbor indices
    FVector2D NWIndex = FVector2D(CX - 1.f, CY - 1.f);
    FVector2D NIndex = FVector2D(CX, CY - 1.f);
    FVector2D NEIndex = FVector2D(CX + 1.f, CY - 1.f);
    FVector2D EIndex = FVector2D(CX + 1.f, CY);
    FVector2D SEIndex = FVector2D(CX + 1.f, CY + 1.f);
    FVector2D SIndex = FVector2D(CX, CY + 1.f);
    FVector2D SWIndex = FVector2D(CX - 1.f, CY + 1.f);
    FVector2D WIndex = FVector2D(CX - 1.f, CY);

    // Trace neighbor walkability
    bool bNWWalkable = IsWalkableGridIndex(NWIndex);
    bool bNWalkable = IsWalkableGridIndex(NIndex);
    bool bNEWalkable = IsWalkableGridIndex(NEIndex);
    bool bEWalkable = IsWalkableGridIndex(EIndex);
    bool bSEWalkable = IsWalkableGridIndex(SEIndex);
    bool bSWalkable = IsWalkableGridIndex(SIndex);
    bool bSWWalkable = IsWalkableGridIndex(SWIndex);
    bool bWWalkable = IsWalkableGridIndex(WIndex);

    // [X][ ][ ]
    // [ ][O][ ]
    // [ ][ ][ ]
    if (bNWWalkable && bNWalkable && bWWalkable)
    {
        TOptional<FTileInfo> Neighbor = GetTileInfoFromGridIndex(NWIndex);
        if (Neighbor.IsSet())
        {
            Neighbors.Add(Neighbor.GetValue());
        }
    }

    // [ ][X][ ]
    // [ ][O][ ]
    // [ ][ ][ ]
    if (bNWalkable)
    {
        TOptional<FTileInfo> Neighbor = GetTileInfoFromGridIndex(NIndex);
        if (Neighbor.IsSet())
        {
            Neighbors.Add(Neighbor.GetValue());
        }
    }

    // [ ][ ][X]
    // [ ][O][ ]
    // [ ][ ][ ]
    if (bNEWalkable && bNWalkable && bEWalkable)
    {
        TOptional<FTileInfo> Neighbor = GetTileInfoFromGridIndex(NEIndex);
        if (Neighbor.IsSet())
        {
            Neighbors.Add(Neighbor.GetValue());
        }
    }

    // [ ][ ][ ]
    // [ ][O][X]
    // [ ][ ][ ]
    if (bEWalkable)
    {
        TOptional<FTileInfo> Neighbor = GetTileInfoFromGridIndex(EIndex);
        if (Neighbor.IsSet())
        {
            Neighbors.Add(Neighbor.GetValue());
        }
    }

    // [ ][ ][ ]
    // [ ][O][ ]
    // [ ][ ][X]
    if (bSEWalkable && bSWalkable && bEWalkable)
    {
        TOptional<FTileInfo> Neighbor = GetTileInfoFromGridIndex(SEIndex);
        if (Neighbor.IsSet())
        {
            Neighbors.Add(Neighbor.GetValue());
        }
    }

    // [ ][ ][ ]
    // [ ][O][ ]
    // [ ][X][ ]
    if (bSWalkable)
    {
        TOptional<FTileInfo> Neighbor = GetTileInfoFromGridIndex(SIndex);
        if (Neighbor.IsSet())
        {
            Neighbors.Add(Neighbor.GetValue());
        }
    }

    // [ ][ ][ ]
    // [ ][O][ ]
    // [X][ ][ ]
    if (bSWWalkable && bSWalkable && bWWalkable)
    {
        TOptional<FTileInfo> Neighbor = GetTileInfoFromGridIndex(SWIndex);
        if (Neighbor.IsSet())
        {
            Neighbors.Add(Neighbor.GetValue());
        }
    }

    // [ ][ ][ ]
    // [X][O][ ]
    // [ ][ ][ ]
    if (bWWalkable)
    {
        TOptional<FTileInfo> Neighbor = GetTileInfoFromGridIndex(WIndex);
        if (Neighbor.IsSet())
        {
            Neighbors.Add(Neighbor.GetValue());
        }
    }
}

int AGrid::GetDistance(const FTileInfo& A, const FTileInfo& B)
{
    int DX = abs(A.GridIndex.X - B.GridIndex.X);
    int DY = abs(A.GridIndex.Y - B.GridIndex.Y);

    int Highest = std::max(DX, DY);
    int Lowest = std::min(DX, DY);
    int Delta = Highest - Lowest;

    return (Delta * 10) + (Lowest * 14);
}

TArray<FTileInfo> AGrid::Retrace(FTileInfo& Start, FTileInfo& End)
{
    INFO(FString::Printf(L"Retracing path: %s to %s", *Start.GridIndex.ToString(), *End.GridIndex.ToString()));
    TArray<FTileInfo> Path;
    FTileInfo CurrentTile = End;

    int Count = 0;
    while (CurrentTile != Start)
    {
        Path.Add(CurrentTile);
        int ParentIndex = GetTileIndexFromGridIndex(CurrentTile.Parent);
        CurrentTile = Tiles[ParentIndex];

        Count++;
        if (Count > Tiles.Num())
        {
            WARNING(FString::Printf(L"Hit maximum tile count: %i", Tiles.Num()));
            return Path;
        }
    }

    Algo::Reverse(Path);

    return Path;
}

TArray<FTileInfo> AGrid::RequestPath(const FTileInfo& Start, const FTileInfo& End)
{
    TArray<FTileInfo> OpenSet{Start};
    TArray<FTileInfo> ClosedSet;

    while (!OpenSet.IsEmpty())
    {
        // Get the cheapest node in the open set
        FTileInfo CurrentTile = OpenSet[0];
        for (FTileInfo& Tile : OpenSet)
        {
            bool CompareF = Tile.GetF() <= CurrentTile.GetF();
            bool CompareH = Tile.H < CurrentTile.H;
            if (CompareF && CompareH)
            {
                int NewIndex = GetTileIndexFromGridIndex(Tile.GridIndex);
                CurrentTile = Tiles[NewIndex];
            }
        }

        int CurrentIndex = GetTileIndexFromGridIndex(CurrentTile.GridIndex);

        // Remove the current node from the open set and put it in the closed set
        OpenSet.Remove(CurrentTile);
        ClosedSet.Add(CurrentTile);

        if (CurrentTile == End)
        {
            auto StartIndex = GetTileIndexFromGridIndex(Start.GridIndex);
            auto EndIndex = GetTileIndexFromGridIndex(End.GridIndex);

            return Retrace(Tiles[StartIndex], Tiles[EndIndex]);
        }

        TArray<FTileInfo> Neighbors;
        GetNeighbors(CurrentTile, Neighbors);
        for (FTileInfo Neighbor : Neighbors)
        {
            int NeighborIndex = GetTileIndexFromGridIndex(Neighbor.GridIndex);
            Neighbor = Tiles[NeighborIndex];

            bool InOpen = OpenSet.Contains(Neighbor);
            bool InClosed = ClosedSet.Contains(Neighbor);

            // If we've already visited this neighbor and closed it, we'll skip it
            if (InClosed)
            {
                continue;
            }

            // Calculate distance from current to neighbor
            int Distance = GetDistance(CurrentTile, Neighbor);

            // Calculate the new cost of the neighbor
            int NewCost = CurrentTile.G + Distance;

            // If the new cost is less than the current cost for this neighbor, it's better.
            // Add this neighbor to the open set and set its cost to the new cost.
            // Also set its parent to the current tile.
            if (NewCost < Neighbor.G || !InOpen)
            {

                Neighbor.G = NewCost;
                Neighbor.H = GetDistance(Neighbor, End);
                Neighbor.Parent = CurrentTile.GridIndex;
                Tiles[NeighborIndex] = Neighbor;

                if (!InOpen)
                {
                    OpenSet.Add(Neighbor);
                }
            }
        }
    }

    // Return an empty tile info array if no path is found
    return TArray<FTileInfo>();
}
