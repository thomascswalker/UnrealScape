// Fill out your copyright notice in the Description page of Project Settings.

#include "USPlayerController.h"

AUSPlayerController::AUSPlayerController()
{
    bShowMouseCursor = true;
    bAttachToPawn = true;

    GameTaskComponent = CreateDefaultSubobject<UGameTaskComponent>(TEXT("GameTaskComponent"));
    AddOwnedComponent(GameTaskComponent);
}

void AUSPlayerController::Tick(float DeltaTime) {}

void AUSPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    InputComponent->BindAction("LeftClick", IE_Released, this, &AUSPlayerController::OnLeftClick);
}

bool AUSPlayerController::LineTraceUnderMouseCursor(FHitResult& HitResult, ECollisionChannel CollisionChannel)
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
        this, Start, End, UEngineTypes::ConvertToTraceType(CollisionChannel), false, ActorsToIgnore,
        EDrawDebugTrace::None, HitResult, true, FLinearColor::Red, FLinearColor::Green, 1.f);

    return BlockingHit;
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
    if (LineTraceUnderMouseCursor(HitResult, COLLISION_INTERACTIVE))
    {
        TargetEntity = Cast<AGameEntity>(HitResult.GetActor());
        if (!TargetEntity)
        {
            return;
        }
        MoveAndInteract(TargetEntity->GetActorLocation());
    }
    // Otherwise if we've hit just normal terrain, we'll just move
    else if (LineTraceUnderMouseCursor(HitResult, COLLISION_TERRAIN))
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

void AUSPlayerController::Move(const FVector Location)
{
    AUSCharacter* ControlledPawn = Cast<AUSCharacter>(GetPawn());
    if (!ControlledPawn)
    {
        return;
    }
    ControlledPawn->NavigatorComponent->NavigateToLocation(Location);
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
    INFO(FString::FromInt(Distance));
    if (Distance < TargetEntity->InteractDistance)
    {
        TargetEntity->Interact(ControlledPawn);
        return;
    }

    if (ControlledPawn->NavigatorComponent->ReachedDestination.IsBound())
    {
        ControlledPawn->NavigatorComponent->ReachedDestination.Clear();
    }
    ControlledPawn->NavigatorComponent->ReachedDestination.AddDynamic(TargetEntity, &AGameEntity::Interact);
    ControlledPawn->NavigatorComponent->NavigateToLocation(Location);
}

void AUSPlayerController::InteractionComplete()
{
    INFO(L"Interaction complete!");
    AUSCharacter* ControlledPawn = Cast<AUSCharacter>(GetPawn());
    if (!ControlledPawn)
    {
        return;
    }
}