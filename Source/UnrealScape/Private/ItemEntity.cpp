// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemEntity.h"

// Sets default values
AItemEntity::AItemEntity()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
    StaticMeshComponent->SetupAttachment(RootComponent);

    InteractiveComponent = CreateDefaultSubobject<UInteractiveComponent>(TEXT("InteractiveComponent"));
    
    InteractiveComponent->SetupAttachment(RootComponent);

    ItemRow.DataTable = UItemFunctionLibrary::GetItemDataTable();
}

// Called when the game starts or when spawned
void AItemEntity::BeginPlay()
{
    Super::BeginPlay();
}

void AItemEntity::OnConstruction(const FTransform& Transform)
{
    INFO(*ItemRow.DataTable.GetName());
    FItem* ItemPtr = ItemRow.DataTable->FindRow<FItem>(ItemRow.RowName, TEXT(""));
    INFO(*ItemRow.RowName.ToString());
    if (ItemPtr)
    {
        FItem Item = *ItemPtr;
        StaticMeshComponent->SetStaticMesh(Item.Mesh);
        StaticMeshComponent->SetCollisionProfileName(TEXT("InteractiveOverlap"));

        InteractiveComponent->InteractDistance = INTERACT_SAME_TILE;
        InteractiveComponent->Name = Item.Name;
        InteractiveComponent->Type = EEntityType::Item;
    }
    else
    {
        WARNING(L"Unable to load item.");
    }
}

// Called every frame
void AItemEntity::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AItemEntity::Interact_Implementation(const FInteractOption& Option)
{
    FItem* ItemPtr = GetItem();
    if (!ItemPtr)
    {
        return;
    }
    FItem Item = *ItemPtr;
    if (Option.Name == "Pickup")
    {
        bool Result = UItemFunctionLibrary::GivePlayerItemFromId(this, Item.Id, Count);
        if (!Result)
        {
            return;
        }
        InteractiveComponent->Complete.Broadcast();
        Destroy();
    }
    else if (Option.Name == "Examine")
    {
        FText Message = FText::FromString(Item.Examine);
        UGlobalFunctionLibrary::AddChatboxMessage(this, Message);
    }
}
void AItemEntity::Interrupt_Implementation() {}

FVector AItemEntity::GetFloor_Implementation()
{
    FVector Origin;
    FVector Extent;
    GetActorBounds(true, Origin, Extent);
    return Origin;
}

APlayerController* AItemEntity::GetPlayer_Implementation()
{
    UWorld* World = GetWorld();
    APlayerController* Controller = UGameplayStatics::GetPlayerController(World, 0);
    return Controller;
}

FString AItemEntity::GetName_Implementation()
{
    return InteractiveComponent->Name;
}

EEntityType AItemEntity::GetType_Implementation()
{
    return InteractiveComponent->Type;
}

TArray<FInteractOption> AItemEntity::GetOptions_Implementation(bool bVisibleOnly)
{
    if (!bVisibleOnly)
    {
        return InteractiveComponent->Options;
    }
    TArray<FInteractOption> VisibleOptions;
    VisibleOptions.Add(FInteractOption::Pickup());
    VisibleOptions.Add(FInteractOption::Examine());
    for (FInteractOption& Option : InteractiveComponent->Options)
    {
        if (Option.bVisible)
        {
            VisibleOptions.Add(Option);
        }
    }
    return VisibleOptions;
}

float AItemEntity::GetInteractDistance_Implementation()
{
    return InteractiveComponent->InteractDistance;
}

UInteractiveComponent* AItemEntity::GetInteractiveComponent_Implementation()
{
    return InteractiveComponent;
}

UMeshComponent* AItemEntity::GetMeshComponent_Implementation()
{
    return StaticMeshComponent;
}

FItem* AItemEntity::GetItem()
{
    FItem* ItemPtr = ItemRow.DataTable->FindRow<FItem>(ItemRow.RowName, TEXT(""));
    return ItemPtr;
}
