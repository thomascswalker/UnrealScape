// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveTask.h"

UMoveTask::UMoveTask()
{
    Name = "Move";
}

void UMoveTask::Execute()
{
    FString Message = FString::Printf(L"Moving to location: %s", *Location.ToString());
    INFO(Message);

    Navigator = TaskExecutor->GetNavigatorComponent();
    if (Navigator == nullptr)
    {
        FATAL(L"Failed to find Navigator Component from Task Executor.");
        return;
    }

    Unbind();
    Bind();

    Navigator->NavigateToLocation(Location);
}

void UMoveTask::Bind()
{
    Navigator->ReachedDestination.AddDynamic(this, &UMoveTask::OnCompleted);
    //Navigator->Stopped.AddDynamic(this, &UMoveTask::OnCancelled);
}
void UMoveTask::Unbind()
{
    Navigator->ReachedDestination.RemoveDynamic(this, &UMoveTask::OnCompleted);
    //Navigator->Stopped.RemoveDynamic(this, &UMoveTask::OnCancelled);
}

void UMoveTask::OnStarted()
{
    Status = ETaskStatus::Active;
    Started.Broadcast(this);
}

void UMoveTask::OnCompleted()
{
    SUCCESS(L"Reached target location!");
    Status = ETaskStatus::Completed;
    Unbind();
    Completed.Broadcast(this);
}

void UMoveTask::OnCancelled()
{
    WARNING(L"Cancelled task!");
    Status = ETaskStatus::Cancelled;
    Unbind();
    Cancelled.Broadcast(this);
}
