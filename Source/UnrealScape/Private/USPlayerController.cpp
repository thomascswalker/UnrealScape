// Fill out your copyright notice in the Description page of Project Settings.

#include "USPlayerController.h"

AUSPlayerController::AUSPlayerController()
{
    bShowMouseCursor = true;
    bAttachToPawn = true;

    GameTaskComponent = CreateDefaultSubobject<UGameTaskComponent>(TEXT("GameTaskComponent"));
    AddOwnedComponent(GameTaskComponent);
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

UNavigatorComponent* AUSPlayerController::GetNavigatorComponent()

{
    AUSCharacter* ControlledPawn = Cast<AUSCharacter>(GetPawn());
    if (ControlledPawn == nullptr)
    {
#ifdef UE_BUILD_DEBUG
        FATAL(L"Failed to get NavigatorComponent.");
#endif
        return nullptr;
    }
    return ControlledPawn->NavigatorComponent;
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

        if (TargetEntity->Actions.Num() == 0)
        {
            INFO("Target has no actions!");
            return;
        }
        CurrentInteractionRequest.Action = TargetEntity->Actions[0];

        FVector Location = TargetEntity->GetFloor();

        // Offset towards the player
        FVector Direction = (TargetEntity->GetActorLocation() - GetPawn()->GetActorLocation()).GetSafeNormal();
        Location = TargetEntity->GetActorLocation() - (Direction * TargetEntity->InteractDistance);

        MoveAndInteract(Location);
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
    TArray<FContextMenuRequest> Requests;

    MultiLineTraceUnderMouseCursor(HitResults, ECC_Interactive);
    for (const FHitResult& Result : HitResults)
    {
        AGameEntity* Entity = Cast<AGameEntity>(Result.GetActor());
        if (!Entity)
        {
            continue;
        }

        if (Entity->Actions.Num() == 0)
        {
            continue;
        }

        FContextMenuRequest Request;
        Request.Entity = Entity;

        for (FAction& Action : Entity->Actions)
        {
            Request.Actions.Add(Action);
        }
        Requests.Add(Request);
    }

    if (Requests.Num() > 0)
    {
        ContextMenuRequested(Requests);
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

void AUSPlayerController::MoveAndInteract(const FVector Location)
{
    AUSCharacter* ControlledPawn = Cast<AUSCharacter>(GetPawn());
    if (!ControlledPawn)
    {
        return;
    }

    ControlledPawn->NavigatorComponent->UpdateCurrentGrid();
    ControlledPawn->NavigatorComponent->UpdateCurrentTile();

    float Distance = FVector::Distance(ControlledPawn->NavigatorComponent->CurrentTile.WorldPosition, Location);
    if (Distance < TargetEntity->InteractDistance)
    {
        TargetEntity->Interact(CurrentInteractionRequest);
        return;
    }

    FNavigationRequest Request;
    Request.End = Location;
    if (ControlledPawn->NavigatorComponent->CanMoveToLocation(Request))
    {
        ControlledPawn->NavigatorComponent->ReachedDestination.AddDynamic(this, &AUSPlayerController::MovementComplete);
        ControlledPawn->NavigatorComponent->Navigate(Request);
    }
}

void AUSPlayerController::MovementComplete()
{
    AUSCharacter* ControlledPawn = Cast<AUSCharacter>(GetPawn());
    ControlledPawn->NavigatorComponent->ReachedDestination.RemoveDynamic(this, &AUSPlayerController::MovementComplete);
    TargetEntity->InteractionComplete.AddDynamic(this, &AUSPlayerController::InteractionComplete);
    TargetEntity->Interact(CurrentInteractionRequest);
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

void AUSPlayerController::ContextMenuRequested_Implementation(const TArray<FContextMenuRequest>& Requests) {}
