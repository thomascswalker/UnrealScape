// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/Interface.h"
#include "InteractiveData.h"

#include "Interactive.generated.h"

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
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Interact(const FInteractOption& Option);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Interrupt();

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
    UMeshComponent* GetMeshComponent();

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void SetOptionVisibility(const FString& OptionName, bool bVisibility);
};
