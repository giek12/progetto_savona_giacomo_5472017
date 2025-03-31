#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PercentageSelector.h"
#include "Camera/CameraComponent.h"
#include "GameCamera.generated.h"

UCLASS()
class UF2_API AGameCamera : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> ObstacleInputWidgetClass;

	AGameCamera();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComponent;

private:
	UPercentageSelector* ObstacleWidget;
};