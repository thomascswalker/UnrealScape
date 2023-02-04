// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "GameTask.h"

#include "GameTaskComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALSCAPE_API UGameTaskComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UGameTaskComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                               FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Tasks")
    TArray<UGameTask*> Tasks;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Tasks")
    UGameTask* CurrentTask;

    UFUNCTION(BlueprintCallable, Category = "Game Tasks", meta = (GameTaskClass = "GameTask"))
    void Add(UPARAM(ref) UGameTask* Task);

    UFUNCTION(BlueprintCallable, Category = "Game Tasks")
    void Pop(UGameTask* Task);

    UFUNCTION(BlueprintCallable, Category = "Game Tasks")
    void Print(UGameTask* Task);

    UFUNCTION(BlueprintCallable, Category = "Game Tasks")
    void Stop(UGameTask* Task);

    UFUNCTION(BlueprintCallable, Category = "Game Tasks")
    void StopAll();

    UFUNCTION(BlueprintCallable, Category = "Game Tasks")
    void Start();
};
