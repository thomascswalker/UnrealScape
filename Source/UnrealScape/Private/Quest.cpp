// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest.h"

UWorld* UQuest::GetWorld() const
{
    return GetOuter()->GetWorld();
}

void UQuest::Start()
{
    FString Message = FString::Printf(TEXT("Started quest: %s"), *DisplayName);
    UGlobalFunctionLibrary::AddChatboxMessage(this, FText::FromString(Message));
    QuestStarted.Broadcast(this);
}

void UQuest::Complete()
{
    FString Message = FString::Printf(TEXT("Completed quest: %s"), *DisplayName);
    UGlobalFunctionLibrary::AddChatboxMessage(this, FText::FromString(Message));
    bComplete = true;
    QuestCompleted.Broadcast(this);
}

void UQuest::Update_Implementation(int Step)
{
    FATAL(L"Default construction. Implement this!");
}

void UQuest::Reward_Implementation()
{
    FATAL(L"Default construction. Implement this!");
}