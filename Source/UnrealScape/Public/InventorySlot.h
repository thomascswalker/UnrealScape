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

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FItem Item;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int Count;

};
