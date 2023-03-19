// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Engine/Plugins/Editor/EditorScriptingUtilities/Source/EditorScriptingUtilities/Public/EditorAssetLibrary.h"
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

    static UDataTable* GetItemDataTable(const UObject* Context);
    static FItemDef* GetItemPtrFromId(const UObject* Context, int Id);
    static FDataTableRowHandle GetItemRowHandleFromId(const UObject* Context, int Id);

    UFUNCTION(BlueprintCallable, Category = "Items",
              meta = (HidePin = "Context", DefaultToSelf = "Context", ExpandBoolAsExecs = "ReturnValue"))
    static bool PlayerCanReceiveItemFromDef(const UObject* Context, const FItemDef& Item);

    UFUNCTION(BlueprintCallable, Category = "Items",
              meta = (HidePin = "Context", DefaultToSelf = "Context", ExpandBoolAsExecs = "ReturnValue"))
    static bool PlayerCanReceiveItemFromId(const UObject* Context, int Id);

    UFUNCTION(BlueprintCallable, Category = "Items",
              meta = (HidePin = "Context", DefaultToSelf = "Context", ExpandBoolAsExecs = "ReturnValue"))
    static bool PlayerHasItemByDef(const UObject* Context, const FItemDef& Item);

    UFUNCTION(BlueprintCallable, Category = "Items",
              meta = (HidePin = "Context", DefaultToSelf = "Context", ExpandBoolAsExecs = "ReturnValue"))
    static bool PlayerHasItemById(const UObject* Context, int Id);

    UFUNCTION(BlueprintCallable, Category = "Items",
              meta = (HidePin = "Context", AutoCreateRefTerm = "Item", DefaultToSelf = "Context", Count = "1",
                      ExpandBoolAsExecs = "ReturnValue"))
    static bool GivePlayerItemFromDef(const UObject* Context, const FItemDef& Item, int Count);

    UFUNCTION(BlueprintCallable, Category = "Items",
              meta = (HidePin = "Context", DefaultToSelf = "Context", Count = "1", ExpandBoolAsExecs = "ReturnValue"))
    static bool GivePlayerItemFromId(const UObject* Context, int Id, int Count);

    UFUNCTION(BlueprintCallable, Category = "Items",
              meta = (HidePin = "Context", DefaultToSelf = "Context", Count = "1"))
    static void SpawnItemAtLocationById(const UObject* Context, int Id, FVector Location, int Count);
};
