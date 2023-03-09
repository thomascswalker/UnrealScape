// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Interactive.h"
#include "InventorySlot.h"

#include "ContextMenu.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSCAPE_API UContextMenu : public UUserWidget
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void AddEntityOption(const TScriptInterface<IInteractive>& Entity, const FInteractOption Option);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void AddInventorySlotOption(const UInventorySlot* InventorySlot, const EItemOptions Option);
};
