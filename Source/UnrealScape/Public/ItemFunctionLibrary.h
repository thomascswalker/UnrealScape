// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "USPlayerController.h"

#include "ItemFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSCAPE_API UItemFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category = "Items", meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static UInventoryComponent* GetInventoryComponent(const UObject* Context);

    static UDataTable* GetItemDataTable();
    static FItem* GetItemPtrFromId(int Id);
    static FDataTableRowHandle GetItemRowHandleFromId(int Id);

    UFUNCTION(BlueprintCallable, Category = "Items",
              meta = (HidePin = "Context", DefaultToSelf = "Context", ExpandBoolAsExecs = "ReturnValue"))
    static bool PlayerCanReceiveItemFromRef(const UObject* Context, const FItem& Item);

    UFUNCTION(BlueprintCallable, Category = "Items",
              meta = (HidePin = "Context", DefaultToSelf = "Context", ExpandBoolAsExecs = "ReturnValue"))
    static bool PlayerCanReceiveItemFromId(const UObject* Context, int Id);

    UFUNCTION(BlueprintCallable, Category = "Items",
              meta = (HidePin = "Context", DefaultToSelf = "Context", ExpandBoolAsExecs = "ReturnValue"))
    static bool PlayerHasItemByRef(const UObject* Context, const FItem& Item);

    UFUNCTION(BlueprintCallable, Category = "Items",
              meta = (HidePin = "Context", DefaultToSelf = "Context", ExpandBoolAsExecs = "ReturnValue"))
    static bool PlayerHasItemById(const UObject* Context, int Id);

    UFUNCTION(BlueprintCallable, Category = "Items",
              meta = (HidePin = "Context", AutoCreateRefTerm = "Item", DefaultToSelf = "Context", Count = "1",
                      ExpandBoolAsExecs = "ReturnValue"))
    static bool GivePlayerItemFromRef(const UObject* Context, const FItem& Item, int Count);

    UFUNCTION(BlueprintCallable, Category = "Items",
              meta = (HidePin = "Context", DefaultToSelf = "Context", Count = "1", ExpandBoolAsExecs = "ReturnValue"))
    static bool GivePlayerItemFromId(const UObject* Context, int Id, int Count);

    UFUNCTION(BlueprintCallable, Category = "Items",
              meta = (HidePin = "Context", DefaultToSelf = "Context", Count = "1"))
    static void SpawnItemAtLocationById(const UObject* Context, int Id, FVector Location, int Count);
};
