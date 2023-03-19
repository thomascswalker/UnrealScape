// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySlot.h"
#include "ItemFunctionLibrary.h"

void UInventorySlot::SetItem(const FItemDef& NewItem)
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

FString UInventorySlot::GetName_Implementation()
{
    if (bHasItem)
    {
        return Item.Name;
    }
    else
    {
        return "";
    }
}

TArray<FInteractOption> UInventorySlot::GetOptions_Implementation(bool bVisibleOnly)
{
    if (!bHasItem)
    {
        return TArray<FInteractOption>();
    }

    TArray<FInteractOption> Options;
    if (Item.bEquipable)
    {
        switch (Item.EquipType)
        {
            case EItemEquipType::Weapon:
                Options.Add(FInteractOption::Wield());
                break;
            case EItemEquipType::Armor:
                Options.Add(FInteractOption::Equip());
                break;
            default:
                break;
        }
    }

    Options.Add(FInteractOption::Use());
    Options.Add(FInteractOption::Drop());
    Options.Add(FInteractOption::Examine());

    return Options;
}

void UInventorySlot::OptionPressed_Implementation(const FInteractOption& Option)
{
    if (Option.Name == "Drop")
    {
        SlotDropped.Broadcast(Item, Count);
        FVector PlayerLocation = UGlobalFunctionLibrary::GetPlayerLocation(this);
        UItemFunctionLibrary::SpawnItemAtLocationById(this, Item.Id, PlayerLocation, Count);
    }
    else if (Option.Name == "Examine")
    {
        UGlobalFunctionLibrary::AddChatboxMessage(this, FText::FromString(Item.Examine));
    }
    else
    {
        WARNING(L"No implementation for this option.");
    }
}
