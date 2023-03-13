// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    // ...
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // ...
}

void UInventoryComponent::ConstructSlots()
{
    static ConstructorHelpers::FClassFinder<UInventorySlot> InventorySlotAsset(
        TEXT("/Game/Blueprints/Items/BP_InventorySlot"));
    if (!InventorySlotAsset.Succeeded())
    {
        return;
    }

    for (int i = 0; i < 28; i++)
    {
        FString Index = FString::FromInt(i);
        FString Name = FString::Printf(L"Slot_%i", *Index);
        UInventorySlot* Slot = NewObject<UInventorySlot>(this, InventorySlotAsset.Class, FName(*Name));
        if (!Slot)
        {
            FATAL(L"Unable to create UInventorySlot");
            continue;
        }
        Slots.Add(Slot);
        Slot->SlotDropped.AddDynamic(this, &UInventoryComponent::RemoveItem);
    }
}

UInventorySlot* UInventoryComponent::GetOpenSlot(const FItem& Item)
{
    if (IsFull())
    {
        return nullptr;
    }

    for (UInventorySlot* Slot : Slots)
    {
        bool IdMatch = Slot->Item.Id == Item.Id;
        // If the slot is empty and the item is NOT stackable
        if (!Slot->bHasItem && !Item.bStackable)
        {
            return Slot;
        }
        // If the slot is NOT empty, but the item IDs match and the item IS stackable
        else if (Slot->bHasItem && IdMatch && Item.bStackable)
        {
            return Slot;
        }
    }

    // If we can't find a valid slot for the item, return nullptr
    return nullptr;
}

UInventorySlot* UInventoryComponent::GetSlot(int X, int Y)
{
    int Index = (Y * 4) + X;
    if (Index > Slots.Num() - 1)
    {
        return nullptr;
    }
    return Slots[Index];
}

int UInventoryComponent::GetItemCount()
{
    int Count = 0;

    for (UInventorySlot* Slot : Slots)
    {
        if (Slot->bHasItem)
        {
            Count++;
        }
    }

    return Count;
}

bool UInventoryComponent::IsEmpty()
{
    return GetItemCount() == 0;
}

bool UInventoryComponent::IsFull()
{
    return GetItemCount() == 28;
}

bool UInventoryComponent::HasItem(const FItem& Item)
{
    for (UInventorySlot* Slot : Slots)
    {
        if (Slot->Item.Id == Item.Id)
        {
            return true;
        }
    }

    return false;
}

bool UInventoryComponent::HasItemId(int Id)
{
    for (UInventorySlot* Slot : Slots)
    {
        if (Slot->Item.Id == Id)
        {
            return true;
        }
    }

    return false;
}

bool UInventoryComponent::AddItem(const FItem& Item, int Count)
{
    UInventorySlot* Slot = GetOpenSlot(Item);
    
    // If the slot exists, add to it
    if (IsValid(Slot))
    {
        // If the item exists and is stackable, add to its current count
        if (Slot->bHasItem && Item.bStackable)
        {
            Slot->AddCount(Count);
            ItemAdded.Broadcast(Item, Count);
        }
        // Otherwise add the new item and set its count if it's stackable
        else
        {
            Slot->SetItem(Item);
            if (Item.bStackable)
            {
                Slot->SetCount(Count);
            }
            ItemAdded.Broadcast(Item, Count);
        }
        return true;
    }

    // Otherwise if the slot isn't found, our inventory is full
    FText Message = FText::FromString(TEXT("Inventory is full."));
    UGlobalFunctionLibrary::AddChatboxMessage(this, Message);

    return false;
}

bool UInventoryComponent::AddUniqueItem(const FItem& Item, int Count)
{
    if (HasItem(Item))
    {
        return false;
    }
    return AddItem(Item, Count);
}

void UInventoryComponent::RemoveItem(const FItem& Item, int Count)
{
    if (!HasItem(Item))
    {
        return;
    }
    for (UInventorySlot* Slot : Slots)
    {
        if (Slot->Item.Id == Item.Id)
        {
            Slot->Clear();
            ItemRemoved.Broadcast(Item, Count);
            return;
        }
    }
}
