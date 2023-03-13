// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlobalFunctionLibrary.h"
#include "Interactive.h"
#include "InteractiveComponent.h"
#include "Item.h"
#include "ItemFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "USUtils.h"

#include "ItemEntity.generated.h"

UCLASS()
class UNREALSCAPE_API AItemEntity : public AActor, public IInteractive
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AItemEntity();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;

public:
    // Properties
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Components")
    TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    TObjectPtr<UInteractiveComponent> InteractiveComponent;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ShowOnlyInnerProperties))
    FDataTableRowHandle ItemRow;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int Count = 1;

    // Functions
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void Interact(const FInteractOption& Option);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void Interrupt();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    FVector GetFloor();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    APlayerController* GetPlayer();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    FString GetName();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    EEntityType GetType();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    TArray<FInteractOption> GetOptions(bool bVisibleOnly = true);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    float GetInteractDistance();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    UInteractiveComponent* GetInteractiveComponent();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    UMeshComponent* GetMeshComponent();

    FItem* GetItem();
};
