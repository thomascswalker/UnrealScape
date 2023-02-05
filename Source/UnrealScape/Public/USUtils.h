// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define TICK(Message, Key)                                         \
    GEngine->AddOnScreenDebugMessage(Key, 5.0f, FColor::Cyan, \
                                     FString(__FUNCTION__) + "(" + FString::FromInt(__LINE__) + ") " + Message);

#define INFO(Message)                                         \
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, \
                                     FString(__FUNCTION__) + "(" + FString::FromInt(__LINE__) + ") " + Message);

#define SUCCESS(Message)                                      \
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, \
                                     FString(__FUNCTION__) + "(" + FString::FromInt(__LINE__) + ") " + Message);

#define WARNING(Message)                                       \
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, \
                                     FString(__FUNCTION__) + "(" + FString::FromInt(__LINE__) + ") " + Message);

#define FATAL(Message)                                      \
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, \
                                     FString(__FUNCTION__) + "(" + FString::FromInt(__LINE__) + ") " + Message);

#define COLLISION_OBSTACLE ECollisionChannel::ECC_GameTraceChannel1
#define COLLISION_TERRAIN ECollisionChannel::ECC_GameTraceChannel2
#define COLLISION_INTERACTIVE ECollisionChannel::ECC_GameTraceChannel3