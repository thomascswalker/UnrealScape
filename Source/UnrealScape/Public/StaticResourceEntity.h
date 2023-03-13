// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "StaticEntity.h"

#include "StaticResourceEntity.generated.h"

UENUM(BlueprintType)
enum class EResourceRequiredItem : uint8
{
    Net,
    Axe,
    Pickaxe
};

/**
 * https://www.tomlooman.com/unreal-engine-cpp-timers/
 */
UCLASS()
class UNREALSCAPE_API AStaticResourceEntity : public AActor, public IInteractive
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AStaticResourceEntity();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Properties
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
    TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    TObjectPtr<UInteractiveComponent> InteractiveComponent;

    FTimerHandle GatherTimerHandle;

    UPROPERTY(EditDefaultsOnly)
    float GatherRate = 1.f;

    // Functions
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void Interact(const FInteractOption& Option);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void Interrupt();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void OnGather();

    UFUNCTION(BlueprintCallable)
    void StartGather();

    UFUNCTION(BlueprintCallable)
    void EndGather();

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

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (AutoCreateRefTerm = "OptionName"))
    void SetOptionVisibility(const FString& OptionName, bool bVisibility);
};