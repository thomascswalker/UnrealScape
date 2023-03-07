// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Algo/Reverse.h"

#include "HintComponent.generated.h"


//#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
//#include <../Plugins/FX/Niagara/Source/Niagara/Private/NiagaraSystem.cpp>
//#include <../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALSCAPE_API UHintComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UHintComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;

    UNiagaraSystem* HintSystem;
    TArray<UNiagaraComponent*> Hints;

    UFUNCTION(BlueprintCallable)
    void CreateHint(FVector Location, bool bClearHints);

    UFUNCTION(BlueprintCallable)
    void RemoveHint(int Index);

    UFUNCTION(BlueprintCallable)
    void ClearHints();
};
