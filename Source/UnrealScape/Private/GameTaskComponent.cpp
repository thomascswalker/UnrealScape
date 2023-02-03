// Fill out your copyright notice in the Description page of Project Settings.

#include "GameTaskComponent.h"

// Sets default values for this component's properties
UGameTaskComponent::UGameTaskComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGameTaskComponent::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void UGameTaskComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UGameTaskComponent::Add(UPARAM(ref) UGameTask* Task)
{
    ITaskExecutor* TaskExecutor = Cast<ITaskExecutor>(GetOwner());
    if (TaskExecutor == NULL)
    {
        return;
    }
    Task->TaskExecutor = TaskExecutor;
    Tasks.Add(Task);
    FString Message = FString::Printf(L"Adding Task: %s; %i", *Task->Name, Tasks.Num());
    INFO(Message);
}

void UGameTaskComponent::Pop(UGameTask* Task)
{
    Tasks.Remove(Task);
    Task->MarkAsGarbage();

    if (Tasks.Num() > 0)
    {
        Start();
    }
}

void UGameTaskComponent::Print(UGameTask* Task) {}

void UGameTaskComponent::Stop(UGameTask* Task)
{
    Task->Unbind();
}

void UGameTaskComponent::StopAll()
{
    for (auto Task : Tasks)
    {
        Task->OnCancelled();
    }
    Tasks.Empty();
}

// https://benui.ca/unreal/delegates-intro/
void UGameTaskComponent::Start()
{
    if (CurrentTask != nullptr)
    {
        CurrentTask->Started.RemoveAll(this);
        CurrentTask->Completed.RemoveAll(this);
        CurrentTask->Cancelled.RemoveAll(this);
    }
    CurrentTask = Tasks[0];
    CurrentTask->Started.AddUniqueDynamic(this, &UGameTaskComponent::Print);
    CurrentTask->Completed.AddUniqueDynamic(this, &UGameTaskComponent::Pop);
    CurrentTask->Cancelled.AddUniqueDynamic(this, &UGameTaskComponent::Stop);
    CurrentTask->Execute();
}