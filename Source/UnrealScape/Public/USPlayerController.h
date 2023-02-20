// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/SplineComponent.h"
#include "CoreMinimal.h"
#include "GameEntity.h"
#include "GameFramework/PlayerController.h"
#include "GameTaskComponent.h"
#include "Grid.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TaskExecutor.h"
#include "Tile.h"
#include "USCharacter.h"

#include "USPlayerController.generated.h"

USTRUCT(BlueprintType)
struct FContextMenuRequest
{
public:
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Interaction")
    AGameEntity* Entity;

    UPROPERTY(BlueprintReadWrite, Category = "Interaction")
    TArray<FAction> Actions;
};

/**
 * 
 */
UCLASS()
class UNREALSCAPE_API AUSPlayerController : public APlayerController, public ITaskExecutor
{
    GENERATED_BODY()

public:
    AUSPlayerController();

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Components")
    UGameTaskComponent* GameTaskComponent;

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

    UNavigatorComponent* GetNavigatorComponent() override;

    UFUNCTION(BlueprintCallable, Category = "Actions")
    void OnLeftClick();

    UFUNCTION(BlueprintCallable, Category = "Actions")
    void OnRightClick();

    UFUNCTION(BlueprintNativeEvent, Category = "Actions")
    void ContextMenuRequested(const TArray<FContextMenuRequest>& Requests);

    UFUNCTION(BlueprintCallable, Category = "Actions")
    void Move(const FVector Location);

    UFUNCTION(BlueprintCallable, Category = "Actions")
    void MoveAndInteract(const FVector Location);

    UFUNCTION(BlueprintCallable, Category = "Actions")
    void MovementComplete();

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void InteractionComplete(AGameEntity* Entity);

    UFUNCTION(BlueprintCallable, Category = "Visual")
    void UpdateFloorVisibility();
};
