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
    FString Message = FString::Printf(L"Number of tasks: %i", Tasks.Num());
    TICK(Message, 8);

}

void UGameTaskComponent::Add(UPARAM(ref) UGameTask* Task)
{
    ITaskExecutor* TaskExecutor = Cast<ITaskExecutor>(GetOwner());
    if (TaskExecutor == NULL)
    {
        return;
    }
    Task->TaskExecutor = TaskExecutor;
    Tasks.Add(Task); // Add to end of task list
    FString Message = FString::Printf(L"Adding Task: %s; %i", *Task->Name, Tasks.Num());
    INFO(Message);
}

void UGameTaskComponent::Pop(UGameTask* Task)
{
    FString Message = FString::Printf(L"Popping task: %s", *Task->Name);
    INFO(Message);

    Stop(Task);
    // If there's more tasks, start the next one
    if (Tasks.Num() > 0)
    {
        Start();
    }
}

void UGameTaskComponent::Print(UGameTask* Task) {}

void UGameTaskComponent::Stop(UGameTask* Task)
{
    Task->Completed.RemoveDynamic(this, &UGameTaskComponent::Pop);
    //Task->Cancelled.RemoveDynamic(this, &UGameTaskComponent::Stop);
    int32 Index;
    if (Tasks.Find(Task, Index))
    {
        Tasks.RemoveAt(Index);
    }
}

void UGameTaskComponent::StopAll()
{
    for (auto Task : Tasks)
    {
        Stop(Task);
    }
    Tasks.Empty();
}

// https://benui.ca/unreal/delegates-intro/
void UGameTaskComponent::Start()
{
    if (Tasks.Num() == 0)
    {
        WARNING(L"No tasks to execute!");
        return;
    }

    CurrentTask = Tasks[0];
    FString Message = FString::Printf(L"New task: %s", *CurrentTask->Name);
    INFO(Message);

    if (CurrentTask != nullptr)
    {
        CurrentTask->Completed.RemoveDynamic(this, &UGameTaskComponent::Pop);
    }
    CurrentTask->Completed.AddDynamic(this, &UGameTaskComponent::Pop);
    CurrentTask->Execute();
}