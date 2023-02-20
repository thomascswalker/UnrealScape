// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactive.h"
#include "USUtils.h"

#include "GameEntity.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractionComplete, AGameEntity*, Entity);

UCLASS()
class UNREALSCAPE_API AGameEntity : public AActor, public IInteractive
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AGameEntity();

    UPROPERTY(BlueprintReadWrite, Category = "Interaction")
    float InteractDistance = 75.f;  // Slightly more than less than one straight tile distance

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(BlueprintCallable, Category = "Event Dispatchers")
    FInteractionComplete InteractionComplete;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
    FString DisplayName;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
    EEntityType Type;

    UPROPERTY(EditAnywhere, Category = "Interaction")
    TArray<FAction> Actions;

    UFUNCTION(BlueprintNativeEvent)
    void Interact(const FInteractRequest& Request) override;

    UFUNCTION(BlueprintNativeEvent)
    FVector GetFloor() override;
};
