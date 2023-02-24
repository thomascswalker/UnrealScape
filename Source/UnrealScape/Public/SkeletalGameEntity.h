// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"
#include "GameEntity.h"
#include "NavigatorComponent.h"
#include "DialogComponent.h"

#include "SkeletalGameEntity.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSCAPE_API ASkeletalGameEntity : public AGameEntity
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ASkeletalGameEntity();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    UPROPERTY(BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* Mesh;

    UPROPERTY(BlueprintReadOnly, Category = "Components")
    UNavigatorComponent* NavigatorComponent;

    UPROPERTY(BlueprintReadOnly, Category = "Components")
    UDialogComponent* DialogComponent;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Dialog")
    UDialogAsset* DialogAsset;

    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
