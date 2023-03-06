// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySlot.h"

void UInventorySlot::SetItem(const FItem& NewItem)
{
    Item = NewItem;
    bHasItem = true;
    SlotChanged.Broadcast();
}

void UInventorySlot::Clear()
{
    bHasItem = false;
    SlotChanged.Broadcast();
}

void UInventorySlot::SetCount(int NewCount)
{
    Count = NewCount;
    SlotChanged.Broadcast();
}
