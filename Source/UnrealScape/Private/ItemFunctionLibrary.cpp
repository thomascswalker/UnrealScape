// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemFunctionLibrary.h"

UInventoryComponent* UItemFunctionLibrary::GetInventoryComponent(const UObject* Context)
{
    if (!IsValid(Context) || !Context->ImplementsGetWorld())
    {
        FString Message =
            FString::Printf(L"Object %s is not valid or does not implement GetWorld().", *Context->GetName());
        FATAL(Message);
        return nullptr;
    }
    AUSPlayerController* Controller =
        Cast<AUSPlayerController>(UGameplayStatics::GetPlayerController(Context->GetWorld(), 0));
    if (!Controller)
    {
        return nullptr;
    }
    return Controller->InventoryComponent;
}

UDataTable* UItemFunctionLibrary::GetItemDataTable()
{
    UDataTable* Table;
    const wchar_t* Path = TEXT("/Game/Blueprints/Items/DT_Items");
    FSoftObjectPath TablePath = FSoftObjectPath(Path);
    Table = Cast<UDataTable>(TablePath.ResolveObject());
    if (Table)
    {
        return Table;
    }
    Table = Cast<UDataTable>(TablePath.TryLoad());
    if (Table)
    {
        return Table;
    }
    FATAL(L"Item DataTable not found.");
    return nullptr;
}

FItem* UItemFunctionLibrary::GetItemRowFromId(int Id)
{
    UDataTable* ItemTable = GetItemDataTable();
    if (!ItemTable)
    {
        return nullptr;
    }
    for (FName RowName : ItemTable->GetRowNames())
    {
        FItem* Row = ItemTable->FindRow<FItem>(RowName, FString(""));
        if (!Row)
        {
            continue;
        }
        if (Row->Id == Id)
        {
            return Row;
        }
    }
    return nullptr;
}

bool UItemFunctionLibrary::PlayerCanReceiveItemFromRef(const UObject* Context, const FItem& Item)
{
    UInventoryComponent* Component = GetInventoryComponent(Context);
    if (!Component)
    {
        return false;
    }
    return Component->GetOpenSlot(Item) != nullptr;
}

bool UItemFunctionLibrary::PlayerCanReceiveItemFromId(const UObject* Context, int Id)
{
    FItem* Item = GetItemRowFromId(Id);
    if (!Item)
    {
        return false;
    }
    return PlayerCanReceiveItemFromRef(Context, *Item);
}

bool UItemFunctionLibrary::PlayerHasItemByRef(const UObject* Context, const FItem& Item)
{
    UInventoryComponent* Component = GetInventoryComponent(Context);
    if (!Component)
    {
        return false;
    }
    return Component->HasItem(Item);
}

bool UItemFunctionLibrary::PlayerHasItemById(const UObject* Context, int Id)
{
    UInventoryComponent* Component = GetInventoryComponent(Context);
    if (!Component)
    {
        return false;
    }
    return Component->HasItemId(Id);
}

bool UItemFunctionLibrary::GivePlayerItemFromRef(const UObject* Context, const FItem& Item, int Count)
{
    UInventoryComponent* Component = GetInventoryComponent(Context);
    if (!Component)
    {
        return false;
    }
    return Component->AddItem(Item, Count);
}

bool UItemFunctionLibrary::GivePlayerItemFromId(const UObject* Context, int Id, int Count)
{
    FItem* Item = GetItemRowFromId(Id);
    if (!Item)
    {
        return false;
    }
    return GivePlayerItemFromRef(Context, *Item, Count);
}
