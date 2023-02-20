// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "Interactive.generated.h"

USTRUCT(BlueprintType)
struct FAction
{
public:
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
    FString Name;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
    bool bUseInteractionDistance = true;
};

USTRUCT(BlueprintType)
struct FInteractRequest
{
public:
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Interaction")
    AActor* Instigator;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Interaction")
    FAction Action;
};

UENUM(BlueprintType)
enum class EEntityType : uint8
{
    Object UMETA(DisplayName = "Object"),
    Npc UMETA(DisplayName = "Npc"),
    Enemy UMETA(DisplayName = "Enemy"),
    Item UMETA(DisplayName = "Item"),
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractive : public UInterface
{
    GENERATED_BODY()
};

class UNREALSCAPE_API IInteractive
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual void Interact(const FInteractRequest& Request) { return; }
    virtual FVector GetFloor() { return FVector::Zero(); }
};
