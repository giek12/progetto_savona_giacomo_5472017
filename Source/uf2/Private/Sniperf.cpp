#include "Sniperf.h"
#include "UObject/ConstructorHelpers.h"

ASniperf::ASniperf() {
	MaxMove = 3;
	MeleeA = false;
	AttRange = 10;
	MinDamage = 4;
	MaxDamage = 8;
	Hp = 20;

	SniperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperMesh"));
	RootComponent = SniperMesh;
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mats(TEXT("/Game/materiali/sniperm"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mats1(TEXT("/Game/materiali/snipertargetm"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cylinder"));


    if (CubeMesh.Succeeded())
    {
        SniperMesh->SetStaticMesh(CubeMesh.Object);
    }
    if (Mats.Succeeded())
    {
        MaterialBase = Mats.Object;
        SniperMesh->SetMaterial(0, MaterialBase);
    }
    if (Mats1.Succeeded())
    {
        MaterialTarget = Mats1.Object;
    }
}

void ASniperf::SetTarget() {
    SniperMesh->SetMaterial(0, MaterialTarget);
}

void ASniperf::ResetTarget() {
    SniperMesh->SetMaterial(0, MaterialBase);
}

void ASniperf::SetName() {
    if (BelongToPlayer) {
        Nome = TEXT("HP : S");
    }
    else {
        Nome = TEXT("AI : S");
    }
}