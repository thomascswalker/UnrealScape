// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactive.h"
#include "InteractiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "USUtils.h"

#include "StaticEntity.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FStaticEntityData : public FEntityData
{
public:
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TObjectPtr<UStaticMesh> StaticMesh;
};

UCLASS()
class UNREALSCAPE_API AStaticEntity : public AActor, public IInteractive
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AStaticEntity();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Properties
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
    TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    TObjectPtr<UInteractiveComponent> InteractiveComponent;

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

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (AutoCreateRefTerm = "OptionName"))
    void SetOptionVisibility(const FString& OptionName, bool bVisibility);
};
