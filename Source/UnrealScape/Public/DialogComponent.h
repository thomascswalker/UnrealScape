// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "DialogAsset.h"
#include "DialogComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALSCAPE_API UDialogComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UDialogComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UDialogAsset* DialogAsset;

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;
};
