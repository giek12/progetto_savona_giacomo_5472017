// Fill out your copyright notice in the Description page of Project Settings.


#include "SfxActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASfxActor::ASfxActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	RootComponent = AudioComponent;
	Death = LoadObject<USoundCue>(nullptr, TEXT("/Game/audio/Death_Cue"));
	Shot = LoadObject<USoundCue>(nullptr, TEXT("/Game/audio/shot_Cue"));
	Punch = LoadObject<USoundCue>(nullptr, TEXT("/Game/audio/Punch_Cue"));

}

// Called when the game starts or when spawned
void ASfxActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASfxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASfxActor::PlayDeath() {
	UGameplayStatics::PlaySound2D(GetWorld(), Death);
}

void ASfxActor::PlayShot() {
	UGameplayStatics::PlaySound2D(GetWorld(), Shot);
}

void ASfxActor::PlayPunch() {
	UGameplayStatics::PlaySound2D(GetWorld(), Punch);
}