// Fill out your copyright notice in the Description page of Project Settings.

#include "USPlayerController.h"

AUSPlayerController::AUSPlayerController()
{
    bShowMouseCursor = true;
}

void AUSPlayerController::Tick(float DeltaTime)
{
    FHitResult HitResult;
    if (LineTraceUnderMouseCursor(HitResult))
    {
        AActor* Actor = HitResult.GetActor();
        ATile* Tile = Cast<ATile>(Actor);
        if (!Tile)
        {
            TargetTile = nullptr;
        }
        else if (Tile != TargetTile)
        {
            TargetTile = Tile;

            if (GEngine)
            {
                FString Message = FString::Printf(L"%s", *Tile->GetName());
                GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, Message);
            }
        }
    }
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
        AActor* Actor = HitResult.GetActor();
        ATile* Tile = Cast<ATile>(Actor);
        if (!Tile)
        {
            CurrentTile = nullptr;
        }
        else if (Tile != CurrentTile)
        {
            CurrentTile = Tile;
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
