// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactive.h"
#include "USUtils.h"

#include "GameEntity.generated.h"

UCLASS()
class UNREALSCAPE_API AGameEntity : public AActor, public IInteractive
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AGameEntity();

    UStaticMeshComponent* StaticMeshComponent;
    EEntityType Type;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Interaction")
    float InteractDistance = 175.f; // Slightly more than 1 diagonal tile distance, but less than 2 straight tile distance

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void Interact() override;
};
