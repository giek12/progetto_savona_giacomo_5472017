#include "PlayerTurnManager.h"

// Sets default values
APlayerTurnManager::APlayerTurnManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerTurnManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerTurnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); // Assicura il funzionamento della classe base
}