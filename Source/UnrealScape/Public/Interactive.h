// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/Interface.h"
#include "InteractiveComponent.h"

#include "Interactive.generated.h"

#define DEFAULT_INTERACT_DISTANCE 112.5f

USTRUCT(BlueprintType, Blueprintable)
struct FInteractOption : public FTableRowBase
{
public:
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
    FString Name;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
    bool bUseInteractionDistance = true;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
    bool bVisible = true;
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

UINTERFACE(BlueprintType, Blueprintable)
class UInteractive : public UInterface
{
    GENERATED_BODY()
};

class UNREALSCAPE_API IInteractive
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Interaction")
    float InteractDistance = DEFAULT_INTERACT_DISTANCE;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Interaction")
    FString Name = "Object";

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Interaction")
    EEntityType Type = EEntityType::Object;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Interaction")
    TArray<FInteractOption> Options;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Interact(const FInteractOption& Option);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    FVector GetFloor();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    APlayerController* GetPlayer();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    FString GetName();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    EEntityType GetType();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    TArray<FInteractOption> GetOptions(bool bVisibleOnly = true);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    float GetInteractDistance();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    UInteractiveComponent* GetInteractiveComponent();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void SetOptionVisibility(const FString& OptionName, bool bVisibility);
};
