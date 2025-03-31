#include "SoundPlayerActor.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"

void ASoundPlayerActor::BeginPlay()
{
	Super::BeginPlay();
	USoundCue* SoundCue = LoadObject<USoundCue>(nullptr, TEXT("/Game/audio/colonna_Cue"));

	if (SoundCue)
	{
		PlaySound(SoundCue);
	}
	else UE_LOG(LogTemp, Warning, TEXT("soundcue non trovata"));
}

// Called every frame
void ASoundPlayerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

