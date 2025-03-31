#include "GameCamera.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "PercentageSelector.h"

AGameCamera::AGameCamera()
{
    PrimaryActorTick.bCanEverTick = false;

    // Creazione della CameraComponent
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    RootComponent = CameraComponent;

    CameraComponent->SetWorldRotation(FRotator(-90.0f, 0.0f, -90.0f));
    //set della rotazione
}

void AGameCamera::BeginPlay()
{
    Super::BeginPlay();
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        PlayerController->SetViewTarget(this);
    }
    CameraComponent->SetWorldLocation(FVector(1200.0f, 1150.0f, 2500.0f));
    //posizionamento nel mondo di gioco

    if (ObstacleInputWidgetClass)
    {
        ObstacleWidget = CreateWidget<UPercentageSelector>(GetWorld(), ObstacleInputWidgetClass);
        if (ObstacleWidget)
        {
            ObstacleWidget->AddToViewport();
        }
        //set del widget per la scelta della percentuale
    }

}