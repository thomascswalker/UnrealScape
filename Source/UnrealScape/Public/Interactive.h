// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Engine/DataTable.h"

#include "Interactive.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FInteractOption : public FTableRowBase
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
    AActor* Player;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Interaction")
    FInteractOption Option;
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

    TArray<FInteractOption> DefaultOptions;

public:
    virtual void Interact(const FInteractOption& Option) { return; }
    virtual FVector GetFloor() { return FVector::Zero(); }

    virtual APlayerController* GetPlayer() { return nullptr; }
    virtual FString GetName() { return FString(); }
    virtual EEntityType GetType() { return EEntityType::Object; }
    virtual TArray<FInteractOption> GetOptions() { return DefaultOptions; }
};
