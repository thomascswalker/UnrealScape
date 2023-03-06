// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/SplineComponent.h"
#include "CoreMinimal.h"
#include "DialogInterpreterComponent.h"
#include "GameFramework/PlayerController.h"
#include "Grid.h"
#include "InteractiveComponent.h"
#include "InteractiveData.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "QuestComponent.h"
#include "StaticEntity.h"
#include "Tile.h"
#include "USCharacter.h"
#include "InventoryComponent.h"

#include "USPlayerController.generated.h"

UCLASS()
class UNREALSCAPE_API AUSPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AUSPlayerController();

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
    TObjectPtr<UDialogInterpreterComponent> DialogInterpreterComponent;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
    TObjectPtr<UQuestComponent> QuestComponent;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
    TObjectPtr<UInventoryComponent> InventoryComponent;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
    TObjectPtr<AActor> TargetActor;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
    TScriptInterface<IInteractive> TargetEntity;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    bool bIsInteracting = false;

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
    void ContextMenuRequested(const TArray<TScriptInterface<IInteractive>>& Entities);

    UFUNCTION(BlueprintCallable, Category = "Actions")
    void Move(const FVector Location);

    UFUNCTION(BlueprintCallable, Category = "Actions")
    void MoveAndInteract(const FInteractOption& Option);

    UFUNCTION(BlueprintCallable, Category = "Actions")
    void MovementComplete();

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void InteractionComplete();

    UFUNCTION(BlueprintCallable, Category = "Visual")
    void UpdateFloorVisibility();
};
