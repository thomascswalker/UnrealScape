// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Item.h"

#include "InventorySlot.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable, BlueprintType, Meta = (ShowWorldContextPin))
class UNREALSCAPE_API UInventorySlot : public UObject
{
	GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool bHasItem = false;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    int Count;

    // Events
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotChanged);
    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatchers")
    FSlotChanged SlotChanged;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    FItem Item;

    UFUNCTION(BlueprintCallable)
    void SetItem(const FItem& NewItem);

    UFUNCTION(BlueprintCallable)
    void Clear();

    UFUNCTION(BlueprintCallable)
    void SetCount(int NewCount);

};
