// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.h"
#include "Interactive.h"

#include "InventorySlot.generated.h"

UCLASS(DefaultToInstanced, EditInlineNew, Blueprintable, BlueprintType, Meta = (ShowWorldContextPin))
class UNREALSCAPE_API UInventorySlot : public UObject, public IInteractive
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

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSlotDropped, const FItemDef&, OutItem, int, OutCount);
    UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Event Dispatchers")
    FSlotDropped SlotDropped;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    FItemDef Item;

    UFUNCTION(BlueprintCallable)
    void SetItem(const FItemDef& NewItem);

    UFUNCTION(BlueprintCallable)
    void Clear();

    UFUNCTION(BlueprintCallable)
    void SetCount(int NewCount);

    UFUNCTION(BlueprintCallable)
    void AddCount(int NewCount);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    TArray<FInteractOption> GetOptions(bool bVisibleOnly = true);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void OptionPressed(const FInteractOption& Option);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    FString GetName();
};
