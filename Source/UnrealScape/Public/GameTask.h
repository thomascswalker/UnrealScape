// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TaskExecutor.h"
#include "UObject/NoExportTypes.h"
#include "USUtils.h"

#include "GameTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStartedSignature, UGameTask*, GameTask);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCancelledSignature, UGameTask*, GameTask);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFailedSignature, UGameTask*, GameTask);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCompletedSignature, UGameTask*, GameTask);

UENUM(BlueprintType)
enum class ETaskStatus : uint8
{
    Queued UMETA(DisplayName = "Queued"),
    Active UMETA(DisplayName = "Active"),
    Paused UMETA(DisplayName = "Paused"),
    Failed UMETA(DisplayName = "Failed"),
    Cancelled UMETA(DisplayName = "Cancelled"),
    Completed UMETA(DisplayName = "Completed"),
};

UCLASS(BlueprintType)
class UNREALSCAPE_API UGameTask : public UObject
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers")
    FStartedSignature Started;

    UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers")
    FCompletedSignature Completed;

    UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers")
    FFailedSignature Failed;

    UPROPERTY(BlueprintAssignable, Category = "Event Dispatchers")
    FCancelledSignature Cancelled;

    ITaskExecutor* TaskExecutor;
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Tasks")
    FString Name = "";
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Tasks")
    ETaskStatus Status = ETaskStatus::Queued;

    UFUNCTION(BlueprintCallable, Category = "Game Tasks")
    virtual void Execute() { return; }
    virtual void Bind() { return; }
    virtual void Unbind() { return; }


    virtual void OnStarted() { return; }
    virtual void OnCompleted(AActor* Actor) { return; }
    virtual void OnCancelled() { return; }
};
