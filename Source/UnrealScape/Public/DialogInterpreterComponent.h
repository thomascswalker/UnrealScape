// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "CoreMinimal.h"
#include "DialogComponent.h"
#include "DialogInterpreterComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDialogStarted, UDialogAsset*, Dialog, int, InitialStep);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDialogStopped);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALSCAPE_API UDialogInterpreterComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UDialogInterpreterComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers")
    FDialogStarted DialogStarted;

    UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers")
    FDialogStopped DialogStopped;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    TObjectPtr<UDialogAsset> CurrentDialog;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    bool bIsTalking = false;

    UFUNCTION(BlueprintCallable)
    void Start(TSubclassOf<UDialogAsset> DialogClass, int InitialStep);

    UFUNCTION(BlueprintCallable)
    void Stop();
};
