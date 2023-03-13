// Fill out your copyright notice in the Description page of Project Settings.

#include "USPlayerController.h"

AUSPlayerController::AUSPlayerController()
{
    bShowMouseCursor = true;
    bAttachToPawn = true;
    PrimaryActorTick.bCanEverTick = true;
    bEnableMouseOverEvents = true;
    DefaultClickTraceChannel = ECC_Interactive;

    // Create components
    DialogInterpreterComponent =
        CreateDefaultSubobject<UDialogInterpreterComponent>(TEXT("DialogInterpreterComponent"));
    DialogInterpreterComponent->SetupAttachment(RootComponent);

    QuestComponent = CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComponent"));
    QuestComponent->SetupAttachment(RootComponent);

    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
    InventoryComponent->ConstructSlots();
    InventoryComponent->SetupAttachment(RootComponent);

    HintComponent = CreateDefaultSubobject<UHintComponent>(TEXT("HintComponent"));
    HintComponent->SetupAttachment(RootComponent);

    // Widgets
    ConstructorHelpers::FClassFinder<UUserWidget> MainInterfaceAsset(TEXT("/Game/Blueprints/UI/WBP_MainInterface"));
    if (MainInterfaceAsset.Succeeded())
    {
        MainInterfaceClass = MainInterfaceAsset.Class;
    }

    ConstructorHelpers::FClassFinder<UUserWidget> ContextMenuAsset(TEXT("/Game/Blueprints/UI/WBP_ContextMenu"));
    if (ContextMenuAsset.Succeeded())
    {
        ContextMenuClass = ContextMenuAsset.Class;
    }
}

void AUSPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Create the main interface widget
    if (MainInterfaceClass)
    {
        MainInterface = CreateWidget<UUserWidget>(this, MainInterfaceClass, FName(TEXT("MainInterfaceWidget")));
        MainInterface->AddToViewport();
    }
}

void AUSPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
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
        bool bImplements = HitResult.GetActor()->Implements<UInteractive>();
        if (bImplements)
        {
            TargetActor = HitResult.GetActor();
            TargetEntity.SetObject(TargetActor);
            TargetEntity.SetInterface(Cast<IInteractive>(TargetActor));

            if (!TargetActor)
            {
                return;
            }

            TArray<FInteractOption> Options = IInteractive::Execute_GetOptions(TargetEntity.GetObject(), true);
            if (Options.Num() == 0)
            {
                INFO("Target has no options!");
                return;
            }

            MoveAndInteract(Options[0]);
            return;
        }
    }

    // Otherwise if we've hit just normal terrain, we'll just move
    SingleLineTraceUnderMouseCursor(HitResult, ECC_Terrain);
    if (HitResult.bBlockingHit)
    {
        TargetActor = nullptr;
        FVector Location = HitResult.Location;
        Move(Location);
    }
    else
    {
        TargetActor = nullptr;
    }
}

void AUSPlayerController::OnRightClick()
{
    TArray<FHitResult> HitResults;
    TArray<TScriptInterface<IInteractive>> Entities;

    MultiLineTraceUnderMouseCursor(HitResults, ECC_Interactive);
    for (const FHitResult& HitResult : HitResults)
    {
        AActor* Actor = HitResult.GetActor();
        bool bImplements = Actor->Implements<UInteractive>();
        if (!bImplements)
        {
            continue;
        }
        TScriptInterface<IInteractive> Entity;
        Entity.SetObject(Actor);
        Entity.SetInterface(Cast<IInteractive>(Actor));
        if (!Entity)
        {
            continue;
        }

        TArray<FInteractOption> Options = IInteractive::Execute_GetOptions(Entity.GetObject(), true);
        if (Options.Num() == 0)
        {
            continue;
        }

        Entities.Add(Entity);
    }

    if (Entities.Num() > 0)
    {
        EntityContextMenuRequested(Entities);
    }
}

void AUSPlayerController::Move(const FVector Location)
{

    AUSCharacter* ControlledPawn = Cast<AUSCharacter>(GetPawn());
    if (!ControlledPawn)
    {
        return;
    }

    // Stop any existing dialog
    DialogInterpreterComponent->Stop();
    // Stop any existing interaction
    if (bIsInteracting)
    {
        INFO(L"Cancelling interaction.");
        InteractionComplete();
        if (TargetEntity)
        {
            IInteractive::Execute_Interrupt(TargetEntity.GetObject());
        }
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

void AUSPlayerController::MoveAndInteract(const FInteractOption& Option)
{
    INFO(Option.Name);
    AUSCharacter* ControlledPawn = Cast<AUSCharacter>(GetPawn());
    if (!ControlledPawn)
    {
        return;
    }

    // Stop any existing dialog
    DialogInterpreterComponent->Stop();

    // Stop any existing interaction
    if (bIsInteracting)
    {
        INFO(L"Cancelling interaction.");
        InteractionComplete();
        if (TargetEntity)
        {
            IInteractive::Execute_Interrupt(TargetEntity.GetObject());
        }
    }

    if (ControlledPawn->NavigatorComponent->ReachedDestination.IsBound())
    {
        ControlledPawn->NavigatorComponent->ReachedDestination.Clear();
    }

    UInteractiveComponent* InteractiveComponent =
        IInteractive::Execute_GetInteractiveComponent(TargetEntity.GetObject());
    if (InteractiveComponent->Complete.IsBound())
    {
        InteractiveComponent->Complete.Clear();
    }

    FVector Location = IInteractive::Execute_GetFloor(TargetEntity.GetObject());

    // Offset towards the player
    FVector Direction = (Location - GetPawn()->GetActorLocation()).GetSafeNormal();

    float InteractDistance = IInteractive::Execute_GetInteractDistance(TargetEntity.GetObject());
    Location -= (Direction * InteractDistance);

    ControlledPawn->NavigatorComponent->UpdateCurrentGrid();
    ControlledPawn->NavigatorComponent->UpdateCurrentTile();

    float Distance = FVector::Distance(ControlledPawn->NavigatorComponent->CurrentTile.WorldPosition, Location);
    bool bCloseEnough = Distance < InteractDistance;
    if (bCloseEnough || Option.bUseInteractionDistance == false)
    {
        bIsInteracting = true;
        IInteractive::Execute_Interact(TargetEntity.GetObject(), Option);
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

    UInteractiveComponent* InteractiveComponent =
        IInteractive::Execute_GetInteractiveComponent(TargetEntity.GetObject());

    if (InteractiveComponent->Complete.IsBound())
    {
        InteractiveComponent->Complete.Clear();
    }
    InteractiveComponent->Complete.AddDynamic(this, &AUSPlayerController::InteractionComplete);

    // TODO: Update this!
    bIsInteracting = true;
    TArray<FInteractOption> Options = IInteractive::Execute_GetOptions(TargetEntity.GetObject(), true);
    IInteractive::Execute_Interact(TargetEntity.GetObject(), Options[0]);
}

void AUSPlayerController::InteractionComplete()
{
    bIsInteracting = false;
    AUSCharacter* ControlledPawn = Cast<AUSCharacter>(GetPawn());

    UInteractiveComponent* InteractiveComponent =
        IInteractive::Execute_GetInteractiveComponent(TargetEntity.GetObject());
    if (InteractiveComponent->Complete.IsBound())
    {
        InteractiveComponent->Complete.RemoveDynamic(this, &AUSPlayerController::InteractionComplete);
    }
    IInteractive::Execute_Interrupt(TargetEntity.GetObject());
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

void AUSPlayerController::EntityContextMenuRequested(const TArray<TScriptInterface<IInteractive>>& Entities)
{
    // Create a new context menu widget
    FName Name = FName(TEXT("ContextMenuWidget"));
    UContextMenu* ContextMenu = CreateWidget<UContextMenu>(this, ContextMenuClass, Name);

    for (const TScriptInterface<IInteractive>& Entity : Entities)
    {
        TArray<FInteractOption> Options = IInteractive::Execute_GetOptions(Entity.GetObject(), true);
        for (FInteractOption& Option : Options)
        {
            ContextMenu->AddEntityOption(Entity.GetObject(), Option);
        }
    }

    ContextMenu->AddToViewport();
}

void AUSPlayerController::ItemContextMenuRequested(UInventorySlot* InventorySlot)
{
    // Create a new context menu widget
    FName Name = FName(TEXT("ContextMenuWidget"));
    UContextMenu* ContextMenu = CreateWidget<UContextMenu>(this, ContextMenuClass, Name);

    for (EItemOptions Option : InventorySlot->GetOptions())
    {
        ContextMenu->AddInventorySlotOption(InventorySlot, Option);
    }

    ContextMenu->AddToViewport();
}
