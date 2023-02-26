// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"
#include "DialogComponent.h"
#include "Interactive.h"
#include "NavigatorComponent.h"

#include "PawnEntity.generated.h"

/**
 * 
 */
UCLASS()
class UNREALSCAPE_API APawnEntity : public APawn, public IInteractive
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    APawnEntity();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Components
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
    UNavigatorComponent* NavigatorComponent;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
    UDialogComponent* DialogComponent;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
    UInteractiveComponent* InteractiveComponent;

    // Variables
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Interaction")
    float InteractDistance = DEFAULT_INTERACT_DISTANCE;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Interaction")
    FString Name = "Object";

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Interaction")
    EEntityType Type = EEntityType::Object;

    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Interaction")
    TArray<FInteractOption> Options;

    // Functions
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void Interact(const FInteractOption& Option);

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
    UInteractiveComponent* GetInteractiveComponent();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    float GetInteractDistance();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (AutoCreateRefTerm = "OptionName"))
    void SetOptionVisibility(const FString& OptionName, bool bVisibility);
};
