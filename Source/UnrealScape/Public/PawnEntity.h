// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"
#include "DialogComponent.h"
#include "Interactive.h"
#include "InteractiveComponent.h"
#include "NavigatorComponent.h"

#include "PawnEntity.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FPawnEntityData : public FEntityData
{
public:
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TSubclassOf<UDialogAsset> DialogAsset;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TObjectPtr<USkeletalMesh> SkeletalMesh;
};

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
    // Properties
    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
    TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
    TObjectPtr<UNavigatorComponent> NavigatorComponent;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
    TObjectPtr<UDialogComponent> DialogComponent;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Components")
    TObjectPtr<UInteractiveComponent> InteractiveComponent;

    // Functions
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void Interact(const FInteractOption& Option);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void Interrupt();

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
    UMeshComponent* GetMeshComponent();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    float GetInteractDistance();

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta = (AutoCreateRefTerm = "OptionName"))
    void SetOptionVisibility(const FString& OptionName, bool bVisibility);
};
