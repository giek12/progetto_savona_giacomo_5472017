// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"          //
#include "Sound/SoundCue.h"                     //
#include "SfxActor.generated.h"

UCLASS()
class UF2_API ASfxActor : public AActor
{
	GENERATED_BODY()
	
private:
	USoundCue* Death;
	USoundCue* Shot;
	USoundCue* Punch;

public:	
	// Sets default values for this actor's properties
	ASfxActor();
	UPROPERTY(VisibleAnywhere, Category = "Audio")
	UAudioComponent* AudioComponent;

	void PlayDeath();
	void PlayShot();
	void PlayPunch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
