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
        return -1;
    }
    return Index;
}

int AGrid::GetTileIndexFromGridIndex(int X, int Y)
{
    return GetTileIndexFromGridIndex(FVector2D(X, Y));
}

FTileInfo& AGrid::GetTileInfoFromGridIndex(FVector2D GridIndex)
{
    int Index = GetTileIndexFromGridIndex(GridIndex);
    check(Tiles[Index].GridIndex == GridIndex) return Tiles[Index];
}

FTileInfo& AGrid::GetTileInfoFromLocation(const FVector Location)
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
    return Tiles[Index];
}

FTileInfo& AGrid::GetTileInfoFromTileActor(const ATile* Tile)
{
    int Index = 0;
    for (FTileInfo& Info : Tiles)
    {
        if (Info.Actor == Tile)
        {
            Index = GetTileIndexFromGridIndex(Info.GridIndex);
        }
    }

    check(Index < Tiles.Num());
    return Tiles[Index];
}

inline bool AGrid::IsWalkable(const FVector& Location)
{
    FVector Start = Location;
    FVector End = Start + FVector(0, 0, 25);

    TArray<AActor*> ActorsToIgnore;
    FHitResult HitResult;
    const bool BlockingHit = UKismetSystemLibrary::SphereTraceSingle(
        this, Start, End, 25.f, UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1), false, ActorsToIgnore,
        EDrawDebugTrace::ForDuration, HitResult, true, FLinearColor::Red, FLinearColor::Green, 2.f);

    return !BlockingHit;
}

bool AGrid::IsWalkable(const FTileInfo& Tile)
{
    return IsWalkable(Tile.WorldPosition);
}

bool AGrid::IsWalkable(const FVector2D Index)
{
    int GridIndex = GetTileIndexFromGridIndex(Index.X, Index.Y);
    if (GridIndex > Tiles.Num() || GridIndex < 0)
    {
        return false;
    }
    FTileInfo Tile = Tiles[GridIndex];
    return IsWalkable(Tile);
}

void AGrid::GetNeighbors(const FTileInfo& Tile, TArray<FTileInfo>& Neighbors)
{
    /*
       -1  0  1
    -1 [X][X][X]
     0 [X][O][X]
     1 [X][X][X]
    */

    // Original coordinates
    int OX = Tile.GridIndex.X;
    int OY = Tile.GridIndex.Y;

    // Neighbor indices
    FVector2D NWIndex = FVector2D(OX - 1.f, OY - 1.f);
    FVector2D NIndex = FVector2D(OX, OY - 1.f);
    FVector2D NEIndex = FVector2D(OX + 1.f, OY - 1.f);
    FVector2D EIndex = FVector2D(OX + 1.f, OY);
    FVector2D SEIndex = FVector2D(OX + 1.f, OY + 1.f);
    FVector2D SIndex = FVector2D(OX, OY + 1.f);
    FVector2D SWIndex = FVector2D(OX - 1.f, OY + 1.f);
    FVector2D WIndex = FVector2D(OX - 1.f, OY);

    // Trace neighbor walkability
    bool bNWWalkable = IsWalkable(NWIndex);
    bool bNWalkable = IsWalkable(NIndex);
    bool bNEWalkable = IsWalkable(NEIndex);
    bool bEWalkable = IsWalkable(EIndex);
    bool bSEWalkable = IsWalkable(SEIndex);
    bool bSWalkable = IsWalkable(SIndex);
    bool bSWWalkable = IsWalkable(SWIndex);
    bool bWWalkable = IsWalkable(WIndex);

    // [X][ ][ ]
    // [ ][O][ ]
    // [ ][ ][ ]
    if (bNWWalkable && bNWalkable && bWWalkable)
    {
        Neighbors.Add(GetTileInfoFromGridIndex(NWIndex));
    }

    // [ ][X][ ]
    // [ ][O][ ]
    // [ ][ ][ ]
    if (bNWalkable)
    {
        Neighbors.Add(GetTileInfoFromGridIndex(NIndex));
    }

    // [ ][ ][X]
    // [ ][O][ ]
    // [ ][ ][ ]
    if (bNEWalkable && bNWalkable && bEWalkable)
    {
        Neighbors.Add(GetTileInfoFromGridIndex(NEIndex));
    }

    // [ ][ ][ ]
    // [ ][O][X]
    // [ ][ ][ ]
    if (bEWalkable)
    {
        Neighbors.Add(GetTileInfoFromGridIndex(EIndex));
    }

    // [ ][ ][ ]
    // [ ][O][ ]
    // [ ][ ][X]
    if (bSEWalkable && bSWalkable && bEWalkable)
    {
        Neighbors.Add(GetTileInfoFromGridIndex(SEIndex));
    }

    // [ ][ ][ ]
    // [ ][O][ ]
    // [ ][X][ ]
    if (bSWalkable)
    {
        Neighbors.Add(GetTileInfoFromGridIndex(SIndex));
    }

    // [ ][ ][ ]
    // [ ][O][ ]
    // [X][ ][ ]
    if (bSWWalkable && bSWalkable && bWWalkable)
    {
        Neighbors.Add(GetTileInfoFromGridIndex(SWIndex));
    }

    // [ ][ ][ ]
    // [X][O][ ]
    // [ ][ ][ ]
    if (bWWalkable)
    {
        Neighbors.Add(GetTileInfoFromGridIndex(WIndex));
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
    if (GEngine)
    {
        FString Message = FString::Printf(L"%s, %s", *Start.GridIndex.ToString(), *End.GridIndex.ToString());
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Message);
    }
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
