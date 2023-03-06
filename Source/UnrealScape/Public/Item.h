// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InteractiveData.h"
#include "USUtils.h"

#include "Item.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FItem : public FTableRowBase
{
public:
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    FString Name;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    int Id;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    bool bEquipable;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    bool bStackable;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    FString Examine;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    TObjectPtr<UTexture2D> Icon;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    TArray<FInteractOption> Options;
};