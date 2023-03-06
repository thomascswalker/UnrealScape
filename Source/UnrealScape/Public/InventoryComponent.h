// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "CoreMinimal.h"

#include "InventorySlot.h"

#include "InventoryComponent.generated.h"

UCLASS(BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALSCAPE_API UInventoryComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UInventoryComponent(const FObjectInitializer& ObjectInitializer);

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(VisibleAnywhere)
    TArray<UInventorySlot*> Slots;

    UFUNCTION(BlueprintCallable)
    void ConstructSlots();

    UInventorySlot* GetOpenSlot(const FItem& Item);

    UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue", DisplayName = "Has Item (FItem)"))
    bool HasItem(const FItem& Item);

    UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue", DisplayName = "Has Item (Id)"))
    bool HasItemId(int Id);

    UFUNCTION(BlueprintCallable)
    bool AddItem(const FItem& Item);

    UFUNCTION(BlueprintCallable)
    bool AddUniqueItem(const FItem& Item);

    UFUNCTION(BlueprintCallable)
    bool RemoveItem(const FItem& Item);
};
