// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySlot.h"
#include "ItemFunctionLibrary.h"

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

void UInventorySlot::AddCount(int NewCount)
{
    Count += NewCount;
    SlotChanged.Broadcast();
}

TArray<EItemOptions> UInventorySlot::GetOptions()
{
    if (!bHasItem)
    {
        return TArray<EItemOptions>();
    }

    TArray<EItemOptions> Options = Item.Options;
    if (Item.bEquipable)
    {
        switch (Item.EquipType)
        {
            case EItemEquipType::Weapon:
                Options.Add(EItemOptions::Wield);
                break;
            case EItemEquipType::Armor:
                Options.Add(EItemOptions::Equip);
                break;
            default:
                break;
        }
    }

    Options.Add(EItemOptions::Use);
    Options.Add(EItemOptions::Drop);
    Options.Add(EItemOptions::Examine);

    return Options;
}

void UInventorySlot::OptionPressed_Implementation(const EItemOptions Option)
{
    INFO(L"Dropping item.");
    switch (Option)
    {
        case EItemOptions::Drop:
        {
            SlotDropped.Broadcast(Item, Count);
            FVector PlayerLocation = UGlobalFunctionLibrary::GetPlayerLocation(this);
            UItemFunctionLibrary::SpawnItemAtLocationById(this, Item.Id, PlayerLocation, Count);
            break;
        }
        default: break;
    }
}
