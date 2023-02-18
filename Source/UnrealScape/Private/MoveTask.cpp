// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveTask.h"

UMoveTask::UMoveTask()
{
    Name = "Move";
}

void UMoveTask::Execute()
{
#ifdef UE_BUILD_DEBUG
    FString Message = FString::Printf(L"Moving to location: %s", *Location.ToString());
    INFO(Message);
#endif

    Navigator = TaskExecutor->GetNavigatorComponent();
    if (Navigator == nullptr)
    {
#ifdef UE_BUILD_DEBUG
        FATAL(L"Failed to find Navigator Component from Task Executor.");
#endif
        return;
    }

    Unbind();
    Bind();

    //Navigator->NavigateToLocation(Location);
}

void UMoveTask::Bind()
{
    //Navigator->ReachedDestination.AddDynamic(this, &UMoveTask::OnCompleted);
    //Navigator->Stopped.AddDynamic(this, &UMoveTask::OnCancelled);
}
void UMoveTask::Unbind()
{
    //Navigator->ReachedDestination.RemoveDynamic(this, &UMoveTask::OnCompleted);
    //Navigator->Stopped.RemoveDynamic(this, &UMoveTask::OnCancelled);
}

void UMoveTask::OnStarted()
{
    Status = ETaskStatus::Active;
    Started.Broadcast(this);
}

void UMoveTask::OnCompleted(AActor* Actor)
{
    FString Message = FString::Printf(L"%s reached target location", *Actor->GetName());
    SUCCESS(Message);
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
