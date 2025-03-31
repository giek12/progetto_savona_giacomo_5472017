#include "Brawlerf.h"
#include "UObject/ConstructorHelpers.h"

ABrawlerf::ABrawlerf() {
	MaxMove = 6;
	MeleeA = true;
	AttRange = 1;
	MinDamage = 1;
	MaxDamage = 6;
	Hp = 40;
	//statistiche

	BrawlerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BrawlerMesh"));
	RootComponent = BrawlerMesh;
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mats(TEXT("/Game/materiali/brawlerm"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mats1(TEXT("/Game/materiali/brawlertargetm"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cylinder"));
	//presa dei materiali

	if (CubeMesh.Succeeded())
	{
		BrawlerMesh->SetStaticMesh(CubeMesh.Object);
	}
	if (Mats.Succeeded())
	{
		MaterialBase = Mats.Object;
		BrawlerMesh->SetMaterial(0, MaterialBase);
	}
	if (Mats1.Succeeded())
	{
		MaterialTarget = Mats1.Object;
	}
	//set dei materiali e della mesh
}

void ABrawlerf::SetTarget() {
	BrawlerMesh->SetMaterial(0, MaterialTarget);
}

void ABrawlerf::ResetTarget() {
	BrawlerMesh->SetMaterial(0, MaterialBase);
}

void ABrawlerf::SetName() {
	if (BelongToPlayer) {
		Nome = TEXT("HP : B");
	}
	else {
		Nome = TEXT("AI : B");
	}
}