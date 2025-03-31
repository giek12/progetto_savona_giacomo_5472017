#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerTurnManager.generated.h"

UCLASS()
class UF2_API APlayerTurnManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlayerTurnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};