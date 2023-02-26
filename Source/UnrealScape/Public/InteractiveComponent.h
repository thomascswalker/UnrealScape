// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractiveComponent.generated.h"



UCLASS(BlueprintType, Blueprintable, meta=(BlueprintSpawnableComponent) )
class UNREALSCAPE_API UInteractiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractiveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FComplete);
    UPROPERTY(BlueprintAssignable, BlueprintCallable, EditDefaultsOnly, Category = "Event Dispatchers")
    FComplete Complete;
};
