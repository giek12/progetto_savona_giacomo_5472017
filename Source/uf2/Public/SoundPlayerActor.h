// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/AudioComponent.h"          //
#include "Sound/SoundCue.h"                     //
#include "SoundPlayerActor.generated.h"

UCLASS()
class UF2_API ASoundPlayerActor : public AActor
{
	GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, Category = "Audio")
    UAudioComponent* AudioComponent;

    ASoundPlayerActor()
    {
        AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
        RootComponent = AudioComponent;
    }

    void PlaySound(USoundCue* SoundCue)
    {
        if (SoundCue)
        {
            AudioComponent->SetSound(SoundCue);
            AudioComponent->Play();
        }
    }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
