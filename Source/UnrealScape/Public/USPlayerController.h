// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Components/SplineComponent.h"
#include "ContextMenu.h"
#include "CoreMinimal.h"
#include "DialogInterpreterComponent.h"
#include "GameFramework/PlayerController.h"
#include "Grid.h"
#include "HintComponent.h"
#include "InteractiveComponent.h"
#include "InteractiveData.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "QuestComponent.h"
#include "StaticEntity.h"
#include "Tile.h"
#include "USCharacter.h"

#include "USPlayerController.generated.h"

UCLASS()
class UNREALSCAPE_API AUSPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AUSPlayerController();

    // Components
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Components")
    TObjectPtr<UDialogInterpreterComponent> DialogInterpreterComponent;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Components")
    TObjectPtr<UQuestComponent> QuestComponent;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Components")
    TObjectPtr<UInventoryComponent> InventoryComponent;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Components")
    TObjectPtr<UHintComponent> HintComponent;

    // Widgets
    TSubclassOf<UUserWidget> MainInterfaceClass;
    TSubclassOf<UUserWidget> ContextMenuClass;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Interface")
    TObjectPtr<UUserWidget> MainInterface;

    // Interaction
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
    TObjectPtr<AActor> TargetActor;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
    TScriptInterface<IInteractive> TargetEntity;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    bool bIsInteracting = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interaction")
    FInteractRequest CurrentInteractionRequest;

    // Overrides
    virtual void BeginPlay() override;
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

    void ContextMenuRequested(const TArray<TScriptInterface<IInteractive>>& Entities);
};
