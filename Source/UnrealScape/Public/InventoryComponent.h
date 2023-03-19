// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "CoreMinimal.h"
#include "InventorySlot.h"
#include "GlobalFunctionLibrary.h"

#include "InventoryComponent.generated.h"

UCLASS(BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALSCAPE_API UInventoryComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UInventoryComponent(const FObjectInitializer& ObjectInitializer);

    // Properties
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    TArray<UInventorySlot*> Slots;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemAdded, const FItemDef&, Item, int, Count);
    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatchers")
    FItemAdded ItemAdded;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemRemoved, const FItemDef&, Item, int, Count);
    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatchers")
    FItemRemoved ItemRemoved;

    // Functions
    UFUNCTION(BlueprintCallable)
    void ConstructSlots();

    UInventorySlot* GetOpenSlot(const FItemDef& Item);

    UFUNCTION(BlueprintCallable)
    UInventorySlot* GetSlot(int X, int Y);

    UFUNCTION(BlueprintCallable)
    int GetItemCount();

    UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue"))
    bool IsEmpty();

    UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue"))
    bool IsFull();

    UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue", DisplayName = "Has Item (FItemDef)"))
    bool HasItem(const FItemDef& Item);

    UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue", DisplayName = "Has Item (Id)"))
    bool HasItemId(int Id);

    UFUNCTION(BlueprintCallable)
    bool AddItem(const FItemDef& Item, int Count);

    UFUNCTION(BlueprintCallable)
    bool AddUniqueItem(const FItemDef& Item, int Count);

    UFUNCTION(BlueprintCallable)
    void RemoveItem(const FItemDef& Item, int Count);
};
