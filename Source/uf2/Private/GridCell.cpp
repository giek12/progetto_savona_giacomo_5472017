#include "GridCell.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "BaseCharacter.h"

AGridCell::AGridCell()
{
    PrimaryActorTick.bCanEverTick = true;

    CellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CellMesh"));
    RootComponent = CellMesh;

    CellMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CellMesh->SetGenerateOverlapEvents(true);
    CellMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    //set per il rilevamento del click e del mouse_over

    CellMesh->OnBeginCursorOver.AddDynamic(this, &AGridCell::OnBeginCursorOver);
    CellMesh->OnEndCursorOver.AddDynamic(this, &AGridCell::OnEndCursorOver);

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mats(TEXT("/Game/materiali/pratoselm"));
    if (Mats.Succeeded())
    {
        MaterialSel = Mats.Object;
    }
    //presa del materiale applicato quando mouse_over

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mats1(TEXT("/Game/materiali/moveselm"));
    if (Mats1.Succeeded())
    {
        FootSelMat = Mats1.Object;
    }
    //presa del materiale applicato quando si passa con il mouse sopra una cella che appartiene all'array delle celle raggiungibili da un personaggio

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mats2(TEXT("/Game/materiali/movem"));
    if (Mats2.Succeeded())
    {
        FootBaseMat = Mats2.Object;
    }
    //presa del materiale applicato a una cella che appartiene all'array delle celle raggiungibili da un personaggio

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Plane"));
    if (CubeMesh.Succeeded())
    {
        CellMesh->SetStaticMesh(CubeMesh.Object);
    }
    //mesh base
    
    character = nullptr;
}

//set del materiale
void AGridCell::SetMaterial(UMaterialInterface* Material)
{
    if (Material)
    {
        CellMesh->SetMaterial(0, Material);
    }
}

void AGridCell::BeginPlay()
{
    Super::BeginPlay();
}

void AGridCell::OnCellSelected()
{
    if (avaiable) {
    OnCellClicked.Broadcast(this);
    }
    //funzione delagate per il click
}

void AGridCell::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

//funzione begin mouse over
void AGridCell::OnBeginCursorOver(UPrimitiveComponent* TouchedComponent)
{
    if (avaiable) {
        if (fs) {
            CellMesh->SetMaterial(0, FootSelMat);
        }
        
        else if (MaterialSel) {
            CellMesh->SetMaterial(0, MaterialSel);
        }
    }
    over = true;
}

//funzione end mouse over
void AGridCell::OnEndCursorOver(UPrimitiveComponent* TouchedComponent)
{
    if (avaiable) {
        if (fs) {
            CellMesh->SetMaterial(0, FootBaseMat);
        }

        else if (MaterialBase) {
            CellMesh->SetMaterial(0, MaterialBase);
        }
    }
    over = false;
}

FVector AGridCell::GetPosition() const {
    return GetActorLocation();
}