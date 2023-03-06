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
    for (int i = 0; i < 28; i++)
    {
        FString Index = FString::FromInt(i);
        FString Name = FString::Printf(L"Slot_%i", *Index);
        UInventorySlot* Slot = NewObject<UInventorySlot>(this, UInventorySlot::StaticClass(), FName(*Name));
        if (!Slot)
        {
            FATAL(L"Unable to create UInventorySlot");
            continue;
        }
        Slots.Add(Slot);
    }
}

UInventorySlot* UInventoryComponent::GetOpenSlot(const FItem& Item)
{
    for (UInventorySlot* Slot : Slots)
    {
        // If the slot is empty and the item is NOT stackable
        if (!Slot->bHasItem && !Item.bStackable)
        {
            return Slot;
        }
        // If the slot is NOT empty, but the item IDs match and the item IS stackable
        else if (Slot->Item.Id == Item.Id && Item.bStackable)
        {
            return Slot;
        }
    }

    // If we can't find a valid slot for the item, return nullptr
    return nullptr;
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

bool UInventoryComponent::AddItem(const FItem& Item)
{
    UInventorySlot* OpenSlot = GetOpenSlot(Item);
    if (OpenSlot == nullptr)
    {
        return false;
    }

    OpenSlot->Item = Item;
    OpenSlot->bHasItem = true;

    FString Message = FString::Printf(L"Added item: %s", *Item.Name);
    INFO(*Message);

    return true;
}

bool UInventoryComponent::AddUniqueItem(const FItem& Item)
{
    if (HasItem(Item))
    {
        return false;
    }
    return AddItem(Item);
}

bool UInventoryComponent::RemoveItem(const FItem& Item)
{
    if (!HasItem(Item))
    {
        return false;
    }
    for (UInventorySlot* Slot : Slots)
    {
        if (Slot->Item.Id == Item.Id)
        {
            Slot->Item = Item;
            Slot->bHasItem = false;
            return true;
        }
    }

    return false;
}
