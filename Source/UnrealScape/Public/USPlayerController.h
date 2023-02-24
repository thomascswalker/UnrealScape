// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/SplineComponent.h"
#include "CoreMinimal.h"
#include "DialogInterpreterComponent.h"
#include "GameEntity.h"
#include "GameFramework/PlayerController.h"
#include "Grid.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tile.h"
#include "USCharacter.h"

#include "USPlayerController.generated.h"

UCLASS()
class UNREALSCAPE_API AUSPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AUSPlayerController();

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
    UDialogInterpreterComponent* DialogInterpreterComponent;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    bool bIsInteracting = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
    AGameEntity* TargetEntity;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
    FInteractRequest CurrentInteractionRequest;

    // Overrides
    virtual void Tick(float DeltaTime) override;
    virtual void SetupInputComponent() override;

    // Methods
    UFUNCTION(BlueprintCallable, Category = "Utility")
    bool SingleLineTraceUnderMouseCursor(FHitResult& HitResult, ECollisionChannel CollisionChannel);

    UFUNCTION(BlueprintCallable, Category = "Utility")
    void MultiLineTraceUnderMouseCursor(TArray<FHitResult>& HitResults, ECollisionChannel CollisionChannel);

    UFUNCTION(BlueprintCallable, Category = "Actions")
    void OnLeftClick();

    UFUNCTION(BlueprintCallable, Category = "Actions")
    void OnRightClick();

    UFUNCTION(BlueprintNativeEvent, Category = "Actions")
    void ContextMenuRequested(const TArray<AGameEntity*>& Entities);

    UFUNCTION(BlueprintCallable, Category = "Actions")
    void Move(const FVector Location);

    UFUNCTION(BlueprintCallable, Category = "Actions")
    void MoveAndInteract(const FOption& Option);

    UFUNCTION(BlueprintCallable, Category = "Actions")
    void MovementComplete();

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void InteractionComplete();

    UFUNCTION(BlueprintCallable, Category = "Visual")
    void UpdateFloorVisibility();
};
