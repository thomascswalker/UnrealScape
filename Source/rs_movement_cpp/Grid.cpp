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

    for (int Y = 0; Y < SizeY; Y++)
    {
        for (int X = 0; X < SizeX; X++)
        {
            FVector SpawnLocation = FVector(X * TileSize, Y * TileSize, 0.f);

            FTransform SpawnTransform;
            SpawnTransform.SetLocation(SpawnLocation);

            // Construct the tile
            AActor* Actor = UGameplayStatics::BeginDeferredActorSpawnFromClass(this, TileClass, SpawnTransform);
            ATile* Tile = Cast<ATile>(Actor);
            FTileInfo TileInfo;

            // If it's constructed successfully, set the grid and world positions
            if (Tile)
            {
                TileInfo.Actor = Tile;

                TileInfo.GridIndex.X = X;
                TileInfo.GridIndex.Y = Y;

                TileInfo.WorldPosition.X = SpawnTransform.GetLocation().X;
                TileInfo.WorldPosition.Y = SpawnTransform.GetLocation().Y;

                Tiles.Add(TileInfo);
                FText Text = FText::FromString(FString::Printf(L"[%i, %i]", X, Y));
                Tile->SetText(Text);
            }

            // Add to Tiles array and attach to this
            Tile->FinishSpawning(SpawnTransform);
            Tile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
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

void AGrid::GetNeighbors(const FTileInfo& Tile, TArray<FTileInfo>& Neighbors)
{
    for (int Y = -1; Y <= 1; Y++)
    {
        for (int X = -1; X <= 1; X++)
        {
            // Get actual index
            int CX = Tile.GridIndex.X + X;
            int CY = Tile.GridIndex.Y + Y;

            // If the actual index is not within the grid, continue
            if (!IsGridIndexValid(CX, CY))
            {
                continue;
            }

            // If the relative index is 0:0, we're on the origin tile; continue
            if (X == 0 && Y == 0)
            {
                continue;
            }

            // Get the possible neighbor, and determine if it's walkable
            int Index = GetTileIndexFromGridIndex(CX, CY);
            FTileInfo PossibleNeighbor = Tiles[Index];
            if (!PossibleNeighbor.Walkable)
            {
                continue;
            }

            // Add this tile to the list
            Neighbors.Add(Tiles[Index]);
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
