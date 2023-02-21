// Fill out your copyright notice in the Description page of Project Settings.

#include "USPlayerController.h"

AUSPlayerController::AUSPlayerController()
{
    bShowMouseCursor = true;
    bAttachToPawn = true;
}

void AUSPlayerController::Tick(float DeltaTime)
{
    UpdateFloorVisibility();
}

void AUSPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("LeftClick", IE_Released, this, &AUSPlayerController::OnLeftClick);
    InputComponent->BindAction("RightClick", IE_Released, this, &AUSPlayerController::OnRightClick);
}

bool AUSPlayerController::SingleLineTraceUnderMouseCursor(FHitResult& HitResult, ECollisionChannel CollisionChannel)
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
    FCollisionQueryParams TraceParams;
    const bool bBlockingHit =
        GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, CollisionChannel, TraceParams);
    return bBlockingHit;
}

void AUSPlayerController::MultiLineTraceUnderMouseCursor(TArray<FHitResult>& HitResults,
                                                         ECollisionChannel CollisionChannel)
{
    FVector WorldLocation;
    FVector WorldDirection;
    bool Result = DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

    if (!Result)
    {
        return;
    }

    FVector Start = WorldLocation;
    FVector End = (WorldDirection * 10000.f) + Start;

    TArray<AActor*> ActorsToIgnore;
    FCollisionQueryParams TraceParams;
    GetWorld()->LineTraceMultiByChannel(HitResults, Start, End, CollisionChannel, TraceParams);
}

void AUSPlayerController::OnLeftClick()
{
    FHitResult HitResult;

    // If we hit an interactive object, move to it, then interact
    SingleLineTraceUnderMouseCursor(HitResult, ECC_Interactive);
    if (HitResult.bBlockingHit)
    {
        TargetEntity = Cast<AGameEntity>(HitResult.GetActor());
        if (!TargetEntity)
        {
            return;
        }

        if (TargetEntity->GetOptions().Num() == 0)
        {
            INFO("Target has no options!");
            return;
        }

        MoveAndInteract(TargetEntity, TargetEntity->GetOptions()[0]);
        return;
    }

    // Otherwise if we've hit just normal terrain, we'll just move
    SingleLineTraceUnderMouseCursor(HitResult, ECC_Terrain);
    if (HitResult.bBlockingHit)
    {
        TargetEntity = nullptr;
        FVector Location = HitResult.Location;
        Move(Location);
    }
    else
    {
        TargetEntity = nullptr;
    }
}

void AUSPlayerController::OnRightClick()
{
    TArray<FHitResult> HitResults;
    TArray<AGameEntity*> Entities;

    MultiLineTraceUnderMouseCursor(HitResults, ECC_Interactive);
    for (const FHitResult& Result : HitResults)
    {
        AGameEntity* Entity = Cast<AGameEntity>(Result.GetActor());
        if (!Entity)
        {
            continue;
        }

        if (Entity->GetOptions().Num() == 0)
        {
            continue;
        }

        Entities.Add(Entity);
    }

    if (Entities.Num() > 0)
    {
        ContextMenuRequested(Entities);
    }
}

void AUSPlayerController::Move(const FVector Location)
{
    AUSCharacter* ControlledPawn = Cast<AUSCharacter>(GetPawn());
    if (!ControlledPawn)
    {
        return;
    }
    FNavigationRequest Request;

    // Offset a small amount towards to the player so we can account for positions right on the line
    // between two tiles. Should be about 5 units in the direction of the player.
    FVector Offset = (Location - ControlledPawn->GetActorLocation()).GetSafeNormal2D() * 5.f;
    Request.End = Location - Offset;
    if (ControlledPawn->NavigatorComponent->CanMoveToLocation(Request))
    {
        ControlledPawn->NavigatorComponent->Navigate(Request);
    }
}

void AUSPlayerController::MoveAndInteract(const AGameEntity* Entity, const FOption& Option)
{
    AUSCharacter* ControlledPawn = Cast<AUSCharacter>(GetPawn());
    if (!ControlledPawn)
    {
        return;
    }

    if (ControlledPawn->NavigatorComponent->ReachedDestination.IsBound())
    {
        ControlledPawn->NavigatorComponent->ReachedDestination.Clear();
    }

    if (TargetEntity->InteractionComplete.IsBound())
    {
        TargetEntity->InteractionComplete.Clear();
    }

    FVector Location = TargetEntity->GetFloor();

    // Offset towards the player
    FVector Direction = (TargetEntity->GetActorLocation() - GetPawn()->GetActorLocation()).GetSafeNormal();
    Location = TargetEntity->GetActorLocation() - (Direction * TargetEntity->InteractDistance);

    ControlledPawn->NavigatorComponent->UpdateCurrentGrid();
    ControlledPawn->NavigatorComponent->UpdateCurrentTile();

    float Distance = FVector::Distance(ControlledPawn->NavigatorComponent->CurrentTile.WorldPosition, Location);
    bool bCloseEnough = Distance < TargetEntity->InteractDistance;
    if (bCloseEnough || Option.bUseInteractionDistance == false)
    {
        TargetEntity->Interact(Option);
        return;
    }

    FNavigationRequest Request;
    Request.End = Location;
    if (ControlledPawn->NavigatorComponent->CanMoveToLocation(Request))
    {
        if (ControlledPawn->NavigatorComponent->ReachedDestination.IsBound())
        {
            ControlledPawn->NavigatorComponent->ReachedDestination.Clear();
        }
        ControlledPawn->NavigatorComponent->ReachedDestination.AddDynamic(this, &AUSPlayerController::MovementComplete);
        ControlledPawn->NavigatorComponent->Navigate(Request);
    }
}

void AUSPlayerController::MovementComplete()
{
    AUSCharacter* ControlledPawn = Cast<AUSCharacter>(GetPawn());
    ControlledPawn->NavigatorComponent->ReachedDestination.RemoveDynamic(this, &AUSPlayerController::MovementComplete);

    if (TargetEntity->InteractionComplete.IsBound())
    {
        TargetEntity->InteractionComplete.Clear();
    }
    TargetEntity->InteractionComplete.AddDynamic(this, &AUSPlayerController::InteractionComplete);

    // TODO: Update this!
    TargetEntity->Interact(TargetEntity->GetOptions()[0]);
}

void AUSPlayerController::InteractionComplete(AGameEntity* Entity)
{
    AUSCharacter* ControlledPawn = Cast<AUSCharacter>(GetPawn());
    Entity->InteractionComplete.RemoveDynamic(this, &AUSPlayerController::InteractionComplete);
}

void AUSPlayerController::UpdateFloorVisibility()
{
    AUSCharacter* ControlledPawn = Cast<AUSCharacter>(GetPawn());
    if (!ControlledPawn)
    {
        return;
    }

    bool bInsideBuilding = ControlledPawn->NavigatorComponent->bInsideBuilding;
    TArray<AActor*> Z1Actors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Z1"), Z1Actors);

    TArray<AActor*> Z2Actors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Z2"), Z2Actors);

    for (AActor* Actor : Z1Actors)
    {
        Actor->SetActorHiddenInGame(bInsideBuilding);
    }
    for (AActor* Actor : Z2Actors)
    {
        Actor->SetActorHiddenInGame(bInsideBuilding);
    }
}

void AUSPlayerController::ContextMenuRequested_Implementation(const TArray<AGameEntity*>& Entities) {}
